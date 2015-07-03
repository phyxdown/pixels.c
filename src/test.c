#include <stdio.h>
#include <pixels.h>
#include <math.h>
void print_mc(COEF c);
void print_mp(PIXEL p);
int main(){
	PIXELS pixels = new_pixels(8, 8);
	PIXEL pixel = pixels;
	COEFS coefs = new_coefs(8, 8);
	COEF coef = coefs;
	print_mp(pixel);
	printf("\n---------------------------------------------------------\n");
	print_mc(coef);
}

void print_mp(PIXEL p){
	for(int i =0 ; i < 8; i++){
		for(int j =0 ; j < 8; j++){
			printf("%d\t", *p);
			p++;
		}
		printf("\n");
	}
}

void print_mc(COEF c){
	for(int i =0 ; i < 8; i++){
		for(int j =0 ; j < 8; j++){
			printf("%f\t", *c);
			c++;
		}
		printf("\n");
	}
}
