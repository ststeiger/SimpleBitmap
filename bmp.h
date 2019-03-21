
#ifndef _BMP_H_  // prevent recursive inclusion
#define _BMP_H_


#include "BitmapInformation.h"


BMPImage* read_bmp(FILE* fp, char** error);
bool      check_bmp_header(BMPHeader* bmp_header, FILE* fp);
bool      write_bmp(FILE* fp, BMPImage* image, char** error);
void      free_bmp(BMPImage* image);
BMPImage* crop_bmp(BMPImage* image, int x, int y, int w, int h, char** error);

#endif  // bmp.h
