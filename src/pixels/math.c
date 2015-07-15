#include <pixels/math.h>
int mod(long dividend, long divider) {
	int ret = dividend % divider;
	if (ret < 0){
		return divider + ret;
	} else {
		return ret;
	}
}

double round(double origin){
	return floor(origin+0.5);
}

double round255(double origin){
	double ret = floor(origin+0.5);
	if (ret > 255) return 255;
	if (ret < 0 ) return 0;
	return floor(origin+0.5);
}
