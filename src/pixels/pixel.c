#include <pixels/pixel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const double PI = 3.141592653589793238462643383279502884197169399375105820974944f;

PIXELS_API IMAGEDATAS new_imagedatas(LEN width, LEN height){
	return (IMAGEDATAS)calloc(width*height, sizeof(pixels_byte_t));
}

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
		double r = *(data+0);
		double g = *(data+1);
		double b = *(data+2);
		double y = 0.299*r+0.587*g+0.114*b;
		double u = 0.492*(b - y);
		double v = 0.877*(r - y);
		(*(coef+i)).Y = y;
		(*(coef+i)).U = u;
		(*(coef+i)).V = v;
		(*(coef+i)).A = *(data+3);
	}
} 

PIXELS_API STATUS ctop(COEFS coefs, PIXELS pixels, LEN len){
	COEF coef = coefs;
	PIXEL pixel = pixels;
	for (INDEX i = 0; i < len*len; i++){
		IMAGEDATA data = (IMAGEDATA)(pixel+i);
		double y = (*(coef+i)).Y;
		double u = (*(coef+i)).U;
		double v = (*(coef+i)).V;
		double r = y + 1.140*v; 
		double g = y - 0.394*u - 0.581*v;
		double b = y + 2.032*u;
		*(data+0) = round255(r);
		*(data+1) = round255(g);
		*(data+2) = round255(b);
		*(data+3) = round((*(coef+i)).A);
	}
}

PIXELS_API STATUS pixels_insert88(PIXELS pixels){
	COEFS coefs = new_coefs(8, 8);
	ptoc(pixels, coefs, 8);
	COEF coef = coefs;
	C88 target = {{0.0}};
	C88 dct = {{0.0}};
	C88 idct = {{0.0}};
	for (int k1 = 0; k1 < 8; k1++) {
		for (int k2 = 0; k2 < 8; k2++) {
			target[k1][k2] = (*coef).Y;
			coef++;
		}
	}
	coef = NULL;
	//DCT: target -> dct
	for (int k1 = 0; k1 < 8; k1++) {
		for (int k2 = 0; k2 < 8; k2++) {
			dct[k1][k2] = 0.0;
			for (int n1 = 0; n1 < 8; n1++) {
				for (int n2 = 0; n2 < 8; n2++) {
					dct[k1][k2] += target[n1][n2]*cos(PI*(n2+0.5)*k2/8)*cos(PI*(n1+0.5)*k1/8);
				}
			}
			dct[k1][k2] = dct[k1][k2]*2/8;
		}
	}
	//INSERT
	dct[2][2] = 16;
	//IDCT: dct -> idct
	for (int k1 = 0; k1 < 8; k1++) {
		for (int k2 = 0; k2 < 8; k2++) {
			idct[k1][k2] = dct[0][0]/4;
			for (int n1 = 1; n1 < 8; n1++) {
				idct[k1][k2] += dct[n1][0]*cos(PI*(k1+0.5)*n1/8)/2;
			}
			for (int n2 = 1; n2 < 8; n2++) {
				idct[k1][k2] += dct[0][n2]*cos(PI*(k2+0.5)*n2/8)/2;
			}
			for (int n1 = 1; n1 < 8; n1++) {
				for (int n2 = 1; n2 < 8; n2++) {
					idct[k1][k2] += dct[n1][n2]*cos(PI*(k2+0.5)*n2/8)*cos(PI*(k1+0.5)*n1/8);
				}
			}
			idct[k1][k2] = idct[k1][k2]*2/8;
		}
	}
	coef = coefs;
	for (int k1 = 0; k1 < 8; k1++) {
		for (int k2 = 0; k2 < 8; k2++) {
			(*coef).Y = round(idct[k1][k2]);
			coef++;
		}
	}
	ctop(coefs, pixels, 8);
}

PIXELS_API int pixels_extract88(IMAGEDATAS imagedatas){
	IMAGEDATA _data = imagedatas;
	C88 target = {{0.0}};
	C88 dct = {{0.0}};
	C88 idct = {{0.0}};
	for (int k1 = 0; k1 < 8; k1++) {
		for (int k2 = 0; k2 < 8; k2++) {
			target[k1][k2] = *_data;
			_data++;
		}
	}
	_data = NULL;
	for (int k1 = 0; k1 < 8; k1++) {
		for (int k2 = 0; k2 < 8; k2++) {
			dct[k1][k2] = 0.0;
			for (int n1 = 0; n1 < 8; n1++) {
				for (int n2 = 0; n2 < 8; n2++) {
					dct[k1][k2] += target[n1][n2]*cos(PI*(n2+0.5)*k2/8)*cos(PI*(n1+0.5)*k1/8);
				}
			}
			dct[k1][k2] = dct[k1][k2]*2/8;
		}
	}
	if( dct[2][2] > 9 && dct[2][2] < 23) {
		return 1;
	}
	else 
		return 0;
}
