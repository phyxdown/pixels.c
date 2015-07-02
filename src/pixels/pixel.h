#include <pixels/defs.h>
#include <pixels/status.h>
#include <pixels/math.h>
typedef unsigned char* PIXELS;
typedef unsigned char* PIXEL;
typedef int PIXELSIZE;
typedef unsigned long LEN; 
typedef char* MAP;

PIXELS_API STATUS copy_pixel(PIXEL src, PIXEL dest, const MAP map);
PIXELS_API PIXELSIZE get_pixel_size(const MAP map);
PIXELS_API STATUS scramble_pixels(PIXELS origin, PIXELS dest, const MAP map, LEN len, int a, int b);
PIXELS_API STATUS rescramble_pixels(PIXELS origin, PIXELS dest, const MAP map, LEN len, int a, int b);
