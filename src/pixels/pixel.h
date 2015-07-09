#ifndef __pixels_pixel_H
#define __pixels_pixel_H

#include <pixels/defs.h>
#include <pixels/status.h>
#include <pixels/math.h>
typedef unsigned char* IMAGEDATA;

typedef unsigned int pixels_pixel_t;
typedef unsigned int* PIXELS;
typedef unsigned int* PIXEL;

struct __pixels_coef {
	double R;
	double G;
	double B;
	double A;
};
typedef struct __pixels_coef pixels_coef_t;
typedef pixels_coef_t* COEFS;
typedef pixels_coef_t* COEF;

typedef int X;
typedef int Y;
typedef int INDEX;

typedef unsigned long LEN; 

PIXELS_API STATUS pixels_coefs_print(const COEFS coefs, LEN len);
PIXELS_API STATUS pixels_coef_print(const COEF coef);
PIXELS_API PIXELS new_pixels(LEN width, LEN height);
PIXELS_API COEFS new_coefs(LEN width, LEN height);

PIXELS_API STATUS scramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b);
PIXELS_API STATUS rescramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b);

PIXELS_API STATUS ptoc(PIXELS pixels, COEFS coefs, LEN len);
PIXELS_API STATUS ctop(COEFS coefs, PIXELS pixels, LEN len);

PIXELS_API STATUS dct_pixels(PIXELS pixels, COEFS coefs, LEN len);
PIXELS_API STATUS idct_pixels(COEFS coefs, PIXELS pixels, LEN len);

#endif
