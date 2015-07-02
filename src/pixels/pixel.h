#include <pixels/defs.h>
#include <pixels/status.h>
#include <pixels/math.h>
typedef unsigned char* PIXELS;
typedef unsigned char* PIXEL;
typedef unsigned char* COEFS;
typedef int PIXELSIZE;
typedef unsigned long LEN; 
typedef char* MAP;

PIXELS_API STATUS set_map(const MAP map);
PIXELS_API STATUS copy_pixel(PIXEL src, PIXEL dest);
PIXELS_API PIXELSIZE get_pixel_size();
PIXELS_API STATUS scramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b);
PIXELS_API STATUS rescramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b);
PIXELS_API STATUS dct_pixels(PIXELS pixels, COEFS coefs);
PIXELS_API STATUS idct_pixels(COEFS coefs, PIXELS pixels);
