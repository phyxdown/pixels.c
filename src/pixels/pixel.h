#include <pixels/defs.h>
#include <pixels/status.h>
#include <pixels/math.h>
typedef unsigned char* IMAGEDATA;

typedef int* PIXELS;
typedef int* PIXEL;

typedef double* COEFS;
typedef double* COEF;

typedef int X;
typedef int Y;
typedef int INDEX;

typedef unsigned long LEN; 

PIXELS_API STATUS copy_pixel(PIXEL src, PIXEL dest);
PIXELS_API PIXELS new_pixels(LEN width, LEN height);
PIXELS_API COEFS new_coefs(LEN width, LEN height);
PIXELS_API STATUS scramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b);
PIXELS_API STATUS rescramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b);
PIXELS_API STATUS dct_pixels(PIXELS pixels, COEFS coefs);
PIXELS_API STATUS idct_pixels(COEFS coefs, PIXELS pixels);
