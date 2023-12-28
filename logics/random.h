#pragma once
#include <stdlib.h>
#include <time.h>

// appropriate random seed setting
void updateSeed() {
	srand(rand() - time(NULL));
}

// maximum < 10 -> 0 -- 9
int random(int maximum) {
	static int temp = 0;
	if (temp == 0) {
		updateSeed();
		temp = rand();
	}
	int r = temp % maximum;
	temp /= maximum;
	return r;
}