
#include "bmp.h"

BMPImage *read_image(const char *filename, char **error);
void write_image(const char *filename, BMPImage *image, char **error);
FILE *_open_file(const char *filename, const char *mode);
void _handle_error(char **error, FILE *fp, BMPImage *image);
void _clean_up(FILE *fp, BMPImage *image, char **error);

#define __JETBRAINS_IDE__ true

// https://stackoverflow.com/questions/26177390/how-to-create-a-c-project-with-clion/26177555


//   Return the size of an image row in bytes.
// - Precondition: the header must have the width of the image in pixels.
uint32_t computeImageSize(BMPHeader *bmp_header)
{
    uint32_t bytes_per_pixel = bmp_header->bits_per_pixel / BITS_PER_BYTE;
    uint32_t bytes_per_row_without_padding = bmp_header->width_px * bytes_per_pixel;
    uint32_t padding =  (4 - (bmp_header->width_px * bytes_per_pixel) % 4) % 4;

    uint32_t row_size_bytes = bytes_per_row_without_padding + padding;

    return row_size_bytes* bmp_header->height_px;
}


static BMPImage * CloneImage(int32_t w, int32_t h, uint8_t* scan0)
{
    BMPImage *new_image = (BMPImage *) malloc(sizeof(*new_image));
    BMPHeader *header = (BMPHeader *) malloc(sizeof(*header));

    new_image->header = *header;
    new_image->header.type = MAGIC_VALUE;
    new_image->header.bits_per_pixel = BITS_PER_PIXEL;
    new_image->header.width_px = w;
    new_image->header.height_px = h;
    new_image->header.image_size_bytes = computeImageSize(&new_image->header);
    new_image->header.size = BMP_HEADER_SIZE + new_image->header.image_size_bytes;
    new_image->header.dib_header_size = DIB_HEADER_SIZE;
    new_image->header.offset = (uint32_t) sizeof(BMPHeader);
    new_image->header.num_planes = 1;
    new_image->header.compression = 0;
    new_image->header.reserved1 = 0;
    new_image->header.reserved2 = 0;
    new_image->header.num_colors = 0;
    new_image->header.important_colors = 0;

    new_image->header.x_resolution_ppm = 3780; // image->header.x_resolution_ppm;
    new_image->header.y_resolution_ppm = 3780; // image->header.y_resolution_ppm;

    new_image->data = (uint8_t*) malloc(sizeof(*new_image->data) * new_image->header.image_size_bytes);
    memcpy(new_image->data, scan0, new_image->header.image_size_bytes);

    return new_image;
}



int old_main()
{
    char *error = NULL;

    // https://youtrack.jetbrains.com/issue/CPP-1296#comment=27-1846360
#ifdef __JETBRAINS_IDE__
    BMPImage *image = read_image("../6x6_24bit.bmp", &error);
#else
    BMPImage *image = read_image("6x6_24bit.bmp", &error);
#endif


    sizeof(BMPHeader);



    printf("type %d\n", image->header.type);
    printf("size %d\n", image->header.size);
    printf("reserved1 %d\n", image->header.reserved1);
    printf("reserved2 %d\n", image->header.reserved2);
    printf("offset %d\n", image->header.offset);
    printf("dib_header_size %d\n", image->header.dib_header_size);
    printf("width_px %d\n", image->header.width_px);
    printf("height_px %d\n", image->header.height_px);
    printf("num_planes %d\n", image->header.num_planes);
    printf("bits_per_pixel %d\n", image->header.bits_per_pixel);
    printf("compression %d\n", image->header.compression);
    printf("image_size_bytes %d\n", image->header.image_size_bytes);
    printf("x_resolution_ppm %d\n", image->header.x_resolution_ppm);
    printf("y_resolution_ppm %d\n", image->header.y_resolution_ppm);
    printf("num_colors %d\n", image->header.num_colors);
    printf("important_colors %d\n", image->header.important_colors);

    int byteSize = image->header.width_px* image->header.height_px*4 + BMP_HEADER_SIZE + DIB_HEADER_SIZE;
    printf("filesize check %d\n", byteSize);
    printf("Check BMP Header size %zu\n", sizeof(BMPHeader));
    printf("Not the DIB Header size %zu\n", sizeof(BMPImage));

    // https://docs.microsoft.com/en-us/windows/desktop/gdi/bitmap-header-types
    // https://d3s.mff.cuni.cz/legacy/teaching/principles_of_computers/Zkouska%20Principy%20pocitacu%202017-18%20-%20varianta%2002%20-%20priloha%20-%20format%20BMP%20z%20Wiki.pdf

    BMPImage * new_image = CloneImage(image->header.width_px
            , image->header.height_px, image->data);
    // new_image->header.x_resolution_ppm = image->header.x_resolution_ppm;
    // new_image->header.y_resolution_ppm = image->header.y_resolution_ppm;

    write_image("omg.bmp", new_image, &error);


    write_image("copy.bmp", image, &error);

    BMPImage *crop_image = crop_bmp(image, 2, 3, 4, 2, &error);
    write_image("crop.bmp", crop_image, &error);

    bool is_valid = check_bmp_header(&crop_image->header, fopen("crop.bmp", "rb"));
    if(!is_valid)
        printf("Invalid BitMap-Header\n");

    _clean_up(NULL, image, &error);
    _clean_up(NULL, crop_image, &error);

    return EXIT_SUCCESS;
}





// https://stackoverflow.com/questions/15127522/how-to-ifdef-by-compilertype-gcc-or-vc
int main(int argc, char* argv[])
{
    // std::cout << "Hello, World!" << std::endl; // Requires iostream and CPP
    printf("Hello World\n");
    old_main();
    return EXIT_SUCCESS;
}




BMPImage *read_image(const char *filename, char **error)
{
    FILE *input_ptr = _open_file(filename, "rb");

    BMPImage *image = read_bmp(input_ptr, error);
    if (*error != NULL)
    {
        _handle_error(error, input_ptr, image);
    }
    fclose(input_ptr);

    return image;
}

void write_image(const char *filename, BMPImage *image, char **error)
{
    FILE *output_ptr = _open_file(filename, "wb");

    if (!write_bmp(output_ptr, image, error))
    {
        _handle_error(error, output_ptr, image);
    }
    fclose(output_ptr);
}

// Open file. In case of error, print message and exit.
FILE *_open_file(const char *filename, const char *mode)
{
    FILE *fp = fopen(filename, mode);
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", filename);

        exit(EXIT_FAILURE);
    }

    return fp;
}

// Print error message and clean up resources.
void _handle_error(char **error, FILE *fp, BMPImage *image)
{
    fprintf(stderr, "ERROR: %s\n", *error);
    _clean_up(fp, image, error);

    exit(EXIT_FAILURE);
}

// Close file and release memory.void _clean_up(FILE *fp, BMPImage *image, char **error)
void _clean_up(FILE *fp, BMPImage *image, char **error)
{
    if (fp != NULL)
    {
        fclose(fp);
    }
    free_bmp(image);
    free(*error);
}
