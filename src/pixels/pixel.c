#include <pixels/pixel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const double PI = 3.141592653589793238462643383279502884197169399375105820974944f;

void print_mp(const PIXELS pixels, LEN len){ PIXEL p = &*pixels; for(int i =0 ; i < len; i++){ for(int j =0 ; j < len; j++){ unsigned char* bp = (unsigned char*)p; printf("(%d,%d,%d,%d) ", *bp, *(bp+1), *(bp+2), *(bp+3)); bp = NULL; p++; } printf("\n"); } p = NULL; } 

PIXELS_API STATUS pixels_coefs_print(const COEFS coefs, LEN len){
	COEF coef = coefs;
	for (int i = 0; i < len; i++){
		for (int j = 0; j < len; j++){
			pixels_coef_print(coef);
			coef++;
		}
		printf("\n");
	}
}

PIXELS_API STATUS pixels_coef_print(const COEF coef){
	printf("(%8f, %8f, %8f, %8f)\n", (*coef).R, (*coef).G, (*coef).B, (*coef).A);
}

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

/**
 * Imagedatas must be derived from 8*8 pixels
 */
PIXELS_API STATUS pixels_insert88(PIXELS pixels){
	COEFS coefs = new_coefs(8, 8);
	ptoc(pixels, coefs, 8);

	COEF coef = coefs;

	C88 target = {{0.0}};
	C88 dct = {{0.0}};
	C88 idct = {{0.0}};

	for (int k1 = 0; k1 < 8; k1++) {
		for (int k2 = 0; k2 < 8; k2++) {
			target[k1][k2] = (*coef).B;
			coef++;
		}
	}
	//PRINT
//for (int k1 = 0; k1 < 8; k1++) {
//	for (int k2 = 0; k2 < 8; k2++) {
//		printf("%5.3f, ", target[k1][k2]);
//	}
//	printf("\n");
//}

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
	//PRINT
//for (int k1 = 0; k1 < 8; k1++) {
//	for (int k2 = 0; k2 < 8; k2++) {
//		printf("%5.3f, ", dct[k1][k2]);
//	}
//	printf("\n");
//}
	//insert
	dct[5][5] = 5;
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
	C88 iidct = {{0.0}};
	//DCT: idct -> iidct
	for (int k1 = 0; k1 < 8; k1++) {
		for (int k2 = 0; k2 < 8; k2++) {
			iidct[k1][k2] = 0.0;
			for (int n1 = 0; n1 < 8; n1++) {
				for (int n2 = 0; n2 < 8; n2++) {
					iidct[k1][k2] += idct[n1][n2]*cos(PI*(n2+0.5)*k2/8)*cos(PI*(n1+0.5)*k1/8);
				}
			}
			iidct[k1][k2] = iidct[k1][k2]*2/8;
		}
	}
	printf("%f, ", iidct[5][5]);

	//PRINT
//for (int k1 = 0; k1 < 8; k1++) {
//	for (int k2 = 0; k2 < 8; k2++) {
//		printf("%5.3f, ", idct[k1][k2]);
//	}
//	printf("\n");
//}
	coef = coefs;
	for (int k1 = 0; k1 < 8; k1++) {
		for (int k2 = 0; k2 < 8; k2++) {
			(*coef).B = round(idct[k1][k2]);
			coef++;
		}
	}
	ctop(coefs, pixels, 8);
}
/**
 * Imagedatas must be derived from 8*8 pixels
 */
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
	//PRINT
//for (int k1 = 0; k1 < 8; k1++) {
//	for (int k2 = 0; k2 < 8; k2++) {
//		printf("%5.3f, ", target[k1][k2]);
//	}
//	printf("\n");
//}

	_data = NULL;
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
	//PRINT
//for (int k1 = 0; k1 < 8; k1++) {
//	for (int k2 = 0; k2 < 8; k2++) {
//		printf("%5.3f, ", dct[k1][k2]);
//	}
//	printf("\n");
//}
	//insert
	if( dct[5][5] > 4 && dct[5][5] < 6) {
		printf("#%f, ", dct[5][5]);
		return 1;
	}
	else 
		return 0;
	//dct[4][4] = 1;
	//IDCT: dct -> idct
	//for (int k1 = 0; k1 < 8; k1++) {
	//	for (int k2 = 0; k2 < 8; k2++) {
	//		idct[k1][k2] = dct[0][0]/4;
	//		for (int n1 = 1; n1 < 8; n1++) {
	//			idct[k1][k2] += dct[n1][0]*cos(PI*(k1+0.5)*n1/8)/2;
	//		}
	//		for (int n2 = 1; n2 < 8; n2++) {
	//			idct[k1][k2] += dct[0][n2]*cos(PI*(k2+0.5)*n2/8)/2;
	//		}
	//		for (int n1 = 1; n1 < 8; n1++) {
	//			for (int n2 = 1; n2 < 8; n2++) {
	//				idct[k1][k2] += dct[n1][n2]*cos(PI*(k2+0.5)*n2/8)*cos(PI*(k1+0.5)*n1/8);
	//			}
	//		}
	//		idct[k1][k2] = idct[k1][k2]*2/8;
	//	}
	//}
	//PRINT
//for (int k1 = 0; k1 < 8; k1++) {
//	for (int k2 = 0; k2 < 8; k2++) {
//		printf("%5.3f, ", idct[k1][k2]);
//	}
//	printf("\n");
//}
}
