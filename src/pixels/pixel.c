#include <pixels/pixel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

PIXELS_API STATUS copy_pixel(PIXEL src, PIXEL dest){
	*src = *dest;
}

PIXELS_API PIXELS new_pixels(LEN width, LEN height) {
	return (PIXELS)calloc(width*height, sizeof(4));
}

PIXELS_API COEFS new_coefs(LEN width, LEN height) {
	return (COEFS)calloc(width*height, sizeof(8));
}


PIXELS_API STATUS scramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b){
	PIXEL origin_marker = origin;
	PIXEL dest_marker = dest;
	for (INDEX i = 0; i < len*len; i++) {
		X x = i%len;
		Y y = i/len;
		X s_x = mod(  x+      b*y, len);
		Y s_y = mod(a*x+(a*b+1)*y, len);
		INDEX s_i = s_y*len+s_x;
		*(dest_marker+s_i) = *(origin_marker+i);
	}
}

PIXELS_API STATUS rescramble_pixels(PIXELS origin, PIXELS dest, LEN len, int a, int b){
	PIXEL origin_marker = origin;
	PIXEL dest_marker = dest;
	for (INDEX i = 0; i < len*len; i++) {
		X x = i%len;
		Y y = i/len;
		X r_x = mod((a*b+1)*x-b*y, len);
		Y r_y = mod(     -a*x+  y, len);
		INDEX r_i = r_y*len+r_x;
		*(dest_marker+r_i) =  *(origin_marker+i);
	}
}

PIXELS_API STATUS dct_pixels(PIXELS pixels, COEFS coefs){
}

PIXELS_API STATUS idct_pixels(COEFS coefs, PIXELS pixels){
}
