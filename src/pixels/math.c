#include <pixels/math.h>
int mod(long dividend, long divider){
	int ret = dividend % divider;
	if (ret < 0){
		return divider + ret;
	} else {
		return ret;
	}
}

