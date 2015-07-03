#include <stdio.h>
#include <pixels.h>
#include <math.h>
void print_mc(COEF c);
void print_mp(PIXEL p);
int main(){
	PIXELS pixels = new_pixels(8, 8);
	print_mp(pixels);
	printf("\n---------------------------------------------------------\n");
	COEFS coefs = new_coefs(8, 8);
	print_mc(coefs);
	printf("\n---------------------------------------------------------\n");
	dct_pixels(pixels, coefs, 8);
	print_mc(coefs);

}

void print_mp(PIXELS pixels){
	PIXEL p = pixels;
	for(int i =0 ; i < 8; i++){
		for(int j =0 ; j < 8; j++){
			printf("%d\t", *p);
			p++;
		}
		printf("\n");
	}
}

void print_mc(COEFS coefs){
	COEF c = coefs;
	for(int i =0 ; i < 8; i++){
		for(int j =0 ; j < 8; j++){
			printf("%f\t", *c);
			c++;
		}
		printf("\n");
	}
}
