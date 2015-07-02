#include <pixels/pixel.h>
#include <string.h>
PIXELS_API STATUS copy_pixel(PIXEL src, PIXEL dest, const MAP map){
	PIXELSIZE size = pixel_size(map);
	for (int i = 0 ; i < size; i++) {
		*(src+i) = *(dest+i);
	}
}

PIXELS_API PIXELSIZE pixel_size(const MAP map){
	return strlen(map);
}


PIXELS_API STATUS scramble_pixels(PIXELS origin, PIXELS dest, const MAP map, LEN len, int a, int b){
	PIXEL origin_marker = origin;
	PIXEL dest_marker = dest;
	PIXELSIZE size = pixel_size(map);
	for (int i = 0; i < len*len; i++) {
		int x = i%len;
		int y = i/len;
		int s_x = mod(  x+      b*y, len);
		int s_y = mod(a*x+(a*b+1)*y, len);
		int s_i = s_y*len+s_x;
		copy_pixel(dest_marker+s_i*size, origin_marker+i*size, map);
	}
}

PIXELS_API STATUS rescramble_pixels(PIXELS origin, PIXELS dest, const MAP map, LEN len, int a, int b){
	PIXEL origin_marker = origin;
	PIXEL dest_marker = dest;
	PIXELSIZE size = pixel_size(map);
	for (int i = 0; i < len*len; i++) {
		int x = i%len;
		int y = i/len;
		int r_x = mod((a*b+1)*x-b*y, len);
		int r_y = mod(     -a*x+  y, len);
		int r_i = r_y*len+r_x;
		copy_pixel(dest_marker+r_i*size, origin_marker+i*size, map);
	}
}
