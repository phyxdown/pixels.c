#include <pixels/math.h>
REMAINDER mod(DIVIDEND dividend, DIVIDER divider) {
	int ret = dividend % divider;
	if (ret < 0){
		return divider + ret;
	} else {
		return ret;
	}
}

