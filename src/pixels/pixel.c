#include <pixels/pixel.h>
#include <string.h>

static MAP _map = "RGB";

PIXELS_API STATUS set_map(const MAP map){
	_map = map;
}

PIXELS_API STATUS copy_pixel(PIXEL src, PIXEL dest){
	PIXELSIZE size = get_pixel_size();
	for (int i = 0 ; i < size; i++) {
		*(src+i) = *(dest+i);
	}
}

PIXELS_API PIXELSIZE get_pixel_size(){
	return strlen(_map);
}


PIXELS_API STATUS scramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b){
	PIXEL origin_marker = origin;
	PIXEL dest_marker = dest;
	PIXELSIZE size = get_pixel_size();
	for (int i = 0; i < len*len; i++) {
		int x = i%len;
		int y = i/len;
		int s_x = mod(  x+      b*y, len);
		int s_y = mod(a*x+(a*b+1)*y, len);
		int s_i = s_y*len+s_x;
		copy_pixel(dest_marker+s_i*size, origin_marker+i*size);
	}
}

PIXELS_API STATUS rescramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b){
	PIXEL origin_marker = origin;
	PIXEL dest_marker = dest;
	PIXELSIZE size = get_pixel_size();
	for (int i = 0; i < len*len; i++) {
		int x = i%len;
		int y = i/len;
		int r_x = mod((a*b+1)*x-b*y, len);
		int r_y = mod(     -a*x+  y, len);
		int r_i = r_y*len+r_x;
		copy_pixel(dest_marker+r_i*size, origin_marker+i*size);
	}
}

PIXELS_API STATUS dct_pixels(PIXELS origin, COEFS dest){
	
}

PIXELS_API STATUS idct_pixels(PIXELS origin, COEFS dest){
	
}
