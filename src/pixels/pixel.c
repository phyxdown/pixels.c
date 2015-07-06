#include <pixels/pixel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const double PI = 3.1415926535898f;

void print_mp(const PIXELS pixels, LEN len){
	PIXEL p = &*pixels;
	for(int i =0 ; i < len; i++){
		for(int j =0 ; j < len; j++){
			unsigned char* bp = (unsigned char*)p;
			printf("(%d,%d,%d,%d) ", *bp, *(bp+1), *(bp+2), *(bp+3));
			bp = NULL;
			p++;
		}
		printf("\n");
	}
	p = NULL;
}

void print_mc(const COEFS coefs, LEN len){
	COEF c = &*coefs;
	for(int i =0 ; i < len; i++){
		for(int j =0 ; j < len; j++){
			printf("%f\t", *c);
			c++;
		}
		printf("\n");
	}
	c = NULL;
}

PIXELS_API STATUS copy_pixel(PIXEL src, PIXEL dest){
	*src = *dest;
}

PIXELS_API PIXELS new_pixels(LEN width, LEN height) {
	return (PIXELS)calloc(width*height, sizeof(PIXELS));
}

PIXELS_API COEFS new_coefs(LEN width, LEN height) {
	return (COEFS)calloc(width*height, sizeof(COEFS));
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

PIXELS_API COEFS ptoc(PIXELS pixels, LEN len){
	PIXEL pixel = pixels;
	COEFS coefs = new_coefs(len, len);
	COEF coef = coefs;
	for (INDEX i = 0; i < len*len; i++){
		*(coef+i) = *(pixel+i);
	}
	return coefs;
}

PIXELS_API STATUS dct_pixels(PIXELS pixels, COEFS coefs, LEN len){
	LEN N = len*len;
	COEF coef = coefs;
	PIXEL pixel;
	for (INDEX k = 0; k < N; k++){
		pixel = pixels;
		*coef = 0.0f;
		for (INDEX n = 0; n < N; n ++){
			*coef += *pixel*cos(PI*(0.5+n)*k/N);
			pixel++;
		}	
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
		*tmp =  0.5*(*coef);
		for (INDEX n = 1; n < N; n ++){
			*tmp += *coef*cos(PI*(0.5+k)*n/N)*2/N;
			coef++;
		}	
		tmp++;
	}

	printf("IDCT RESULT:\n");
	print_mc(tmps, len);
	tmp = tmps;
	PIXEL pixel = pixels;
	for (INDEX i = 0; i < N; i ++){
		*(pixel+i) = *(tmp+i);
	}

	printf("\n-----------------------------\n");
	printf("IDCT PIXEL RESULT:\n");
	print_mp(pixels, len);
}
