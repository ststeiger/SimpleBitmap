
#ifndef _BMP_H_  // prevent recursive inclusion
#define _BMP_H_

#include "BitmapInformation.h"

bool      write_bmp(FILE* fp, BMPImage* image, char** error);
void      free_bmp(BMPImage* image);
BMPImage * CreateBitmapFromScan0(int32_t w, int32_t h, uint8_t* scan0)

#endif  // bmp.h
