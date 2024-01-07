#pragma once
#include "random.h"
#include "../constants.h"
#include <stdio.h>

typedef struct {
	int index;
	int diceInt;
	int fixed;
	int cat_index;
} DICE;

DICE DICES[CAT_COUNT];

// initDices with randomfully numbers
// should call it when you want to initialize it!
void __initDices(DICE dices[CAT_COUNT]) {
	for (int i = 0; i < CAT_COUNT; i++) {
		dices[i].diceInt = random_function(6) + 1;
		dices[i].fixed = 0;
		dices[i].cat_index = i;
		dices[i].index = -1;
	}
	// sort all dices
	for (int i = 1; i < CAT_COUNT; i++) {
		DICE temp = dices[i];
		int j = i - 1;
		while (j >= 0 && dices[j].diceInt < temp.diceInt) {
			dices[j + 1] = dices[j];
			j--;
		}
		dices[j + 1] = temp;
	}
}

// get the uniquation of items
char __isUnique(DICE dices[CAT_COUNT], const int index) {
	for (int i = 0; i < CAT_COUNT; i++)
		if (i != index && !dices[i].fixed && dices[i].diceInt == dices[index].diceInt)
			return dices[i].fixed = 0;
	return 1;
}

// distiguish uniques and others
void distinguishDices(DICE dices[CAT_COUNT]) {
	for (int i = 0; i < CAT_COUNT; i++)
		if (!dices[i].fixed)
			dices[i].fixed = __isUnique(dices, i);
}

// fixing dices locations
void fixDices(DICE dices[CAT_COUNT]) {
	int index = 0, i = 0, n;
	while (i < CAT_COUNT) {
		n = 0;
		int curr = dices[i].diceInt;
		while (i < CAT_COUNT && dices[i].diceInt == curr) {
			dices[i].index = index;
			i++;
			n++;
		}
		index += n;
	}
}

// check the dices for availabling for the final use
char isReadyToUse(const DICE dices[CAT_COUNT]) {
	for (int i = 0; i < CAT_COUNT; i++)
		if (!dices[i].fixed)
			return 0;
	return 1;
}

void improveDices(DICE dices[CAT_COUNT]) {
	int i = 0;
	while (i < CAT_COUNT) {
		int curr = i;
		int n = 0;
		while (i < CAT_COUNT && dices[i].index == dices[curr].index) {
			i++;
			n++;
		}
		if (n > 1) {
			const int end = n + curr;
			for (int j = curr; j < end; j++) {
				dices[j].fixed = 1;
				char can;
				do {
					can = 1;
					dices[j].diceInt = random_function(6) + 1;
					for (int k = curr; k < j; k++)
						if (dices[k].diceInt == dices[j].diceInt)
							can = 0;
				} while (!can);
			}
			for (int j = curr; j < end; j++) {//sorting items
				DICE temp = dices[j];
				int k = j - 1;
				while (k >= curr && temp.diceInt > dices[k].diceInt) {
					dices[k + 1] = dices[k];
					k--;
				}
				dices[k + 1] = temp;
			}
			int minIndex = dices[curr].index;
			for (int j = curr; j < end; j++) {
				dices[j].index = minIndex++;
			}
		}
	}
}

void printDices(DICE dices[CAT_COUNT]) {
	printf("\n[");
	printf("%d:{index:%d,fixed:%d,value:%d,cat:%d}", 1, dices[0].index, dices[0].fixed, dices[0].diceInt, dices[0].cat_index);
	for (int i = 1; i < CAT_COUNT; i++)
		printf(", %d:{index:%d,fixed:%d,value:%d,cat:%d}", i + 1, dices[i].index, dices[i].fixed, dices[i].diceInt, dices[i].cat_index);
	printf("]\n");
}

int toIndex(int indices[CAT_COUNT]) {
	__initDices(DICES);
	//printf("FIRST ROLL: ");
	//printDices(DICES);
	distinguishDices(DICES);
	//printf("DISTINGUISHED: ");
	//printDices(DICES);
	fixDices(DICES);
	//printf("FIXED: ");
	//printDices(DICES);
	char res = isReadyToUse(DICES);
	for (int i = 0; i < CAT_COUNT; i++)
		indices[DICES[i].index] = DICES[i].cat_index;
	return res;
}

void improveIndexes(int indices[CAT_COUNT]) {
	improveDices(DICES);
	printf("IMPROVED: ");
	//printDices(DICES);
	for (int i = 0; i < CAT_COUNT; i++)
		indices[DICES[i].index] = DICES[i].cat_index;
}