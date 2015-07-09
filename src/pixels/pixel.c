#include <pixels/pixel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const double PI = 3.141592653589793238462643383279502884197169399375105820974944f;

void print_mp(const PIXELS pixels, LEN len){ PIXEL p = &*pixels; for(int i =0 ; i < len; i++){ for(int j =0 ; j < len; j++){ unsigned char* bp = (unsigned char*)p; printf("(%d,%d,%d,%d) ", *bp, *(bp+1), *(bp+2), *(bp+3)); bp = NULL; p++; } printf("\n"); } p = NULL; } 
void print_mc(const COEFS coefs, LEN len){ COEF c = &*coefs; for(int i =0 ; i < len; i++){ for(int j =0 ; j < len; j++){ printf("%8f ", *c); c++; } printf("\n"); } c = NULL; }

PIXELS_API PIXELS new_pixels(LEN width, LEN height) {
	return (PIXELS)calloc(width*height, sizeof(pixels_pixel_t));
}

PIXELS_API COEFS new_coefs(LEN width, LEN height) {
	return (COEFS)calloc(width*height, sizeof(pixels_coef_t));
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

PIXELS_API STATUS ptoc(PIXELS pixels, COEFS coefs, LEN len){
	PIXEL pixel = pixels;
	COEF coef = coefs;
	for (INDEX i = 0; i < len*len; i++){
		IMAGEDATA data = (IMAGEDATA)(pixel+i);
		(*(coef+i)).R = *(data+0);
		(*(coef+i)).G = *(data+1);
		(*(coef+i)).B = *(data+2);
		(*(coef+i)).A = *(data+3);
	}
} 

PIXELS_API STATUS ctop(COEFS coefs, PIXELS pixels, LEN len){
	COEF coef = coefs;
	PIXEL pixel = pixels;
	for (INDEX i = 0; i < len*len; i++){
		IMAGEDATA data = (IMAGEDATA)(pixel+i);
		*(data+0) = round((*(coef+i)).R);
		*(data+1) = round((*(coef+i)).G);
		*(data+2) = round((*(coef+i)).B);
		*(data+3) = round((*(coef+i)).A);
	}
}

PIXELS_API STATUS dct_pixels(PIXELS pixels, COEFS coefs, LEN len){
	COEFS c_pixels = new_coefs(len, len);
	ptoc(pixels, c_pixels, len);
	LEN N = len*len;
	COEF coef = coefs;
	COEF c_pixel;
	for (INDEX k = 0; k < N; k++){
		c_pixel = c_pixels;
		(*coef).R = 0.0f;
		(*coef).G = (*(c_pixels+k)).G;
		(*coef).B = (*(c_pixels+k)).B;
		(*coef).A = (*(c_pixels+k)).A;

		for (INDEX n = 0; n < N; n ++){
			(*coef).R += (*c_pixel).R*cos(PI*(0.5+n)*k/N);
			c_pixel++;
		}	
		(*coef).R = (*coef).R*2/N;
		coef++;
	}
}

PIXELS_API STATUS idct_pixels(COEFS coefs, PIXELS pixels, LEN len){
	LEN N = len*len;
	COEFS tmps = new_coefs(len, len);
	COEF tmp = tmps;
	COEF coef;
	for (INDEX k = 0; k < N; k++){
		coef = coefs;
		(*tmp).R =  0.5*(*coef).R;
		(*tmp).G = (*(coef+k)).G;
		(*tmp).B = (*(coef+k)).B;
		(*tmp).A = (*(coef+k)).A;
		coef ++;
		for (INDEX n = 1; n < N; n ++){
			(*tmp).R += (*coef).R*cos(PI*(0.5+k)*n/N);
			coef++;
		}	
		tmp++;
	}
	tmp = tmps;
	ctop(tmps, pixels, len);
}
