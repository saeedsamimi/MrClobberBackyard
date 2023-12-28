#pragma onc
#include <stdlib.h>
#include <time.h>

// maximum < 10 -> 0 -- 9
int random(int maximum) {
	static int temp = 0;
	if (temp == 0) {
		srand(rand() - time(NULL));
		temp = rand();
	}
	int r = temp % maximum;
	temp /= maximum;
	return r;
}