#pragma once
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>
#include "../windows/error.h"
#include "../constants.h"
#include "../map.h"

bool forceCreateDir(const char* dirName) {
	int result = al_make_directory(dirName);
	return result == 1;
}

bool fileExists(const char* fileName) {
	struct stat buffer;
	return !stat(fileName, &buffer);
}

FILE* createFile(const char*fileName,ALLEGRO_DISPLAY *displayBuff) {
	FILE* newFile = fopen(fileName, fileExists(fileName) ? "a" : "w");
	if (!newFile) {
		showError(displayBuff, "File cannot be created", "this file in this location cannot be created! \n");
		return NULL;
	}
	return newFile;
}

bool writeOnFile(FILE* out) {
	if (!out) return 0;
	// print game details
	time_t now = time(NULL);
	char* result = ctime(&now);
	fprintf(out, "\nGame (Date/Time): %s\n", result);
	// fprintf CATS
	fprintf(out, "[ PLAYERS (sorted): \n");
	for (int i = 0; i < CAT_COUNT; i++) {
		fprintf(out, "Player #%d: {\n", i + 1);
		fprintf(out,"\'Attack Point\': %d\n", cats[i].attackPoint);
		fprintf(out,"\'Defence Point\': %d\n", cats[i].defencePoint);
		fprintf(out,"\'Mouse Point\': %d\n}\n", cats[i].mousePoint);
	}
	fprintf(out, "]\n");
	for (int row = 0; row < BOARD_SIZE; row++) {
		// print U walls
		for (int col = 0; col < BOARD_SIZE; col++)
			if (hasFlag(map[row][col], FLAG_UWALL))
				fprintf(out, " ------- ");
			else
				fprintf(out, "         ");
		fprintf(out, "\n");
		for (int col = 0; col < BOARD_SIZE; col++)
			fputs(" +-----+ ", out);
		fprintf(out, "\n");
		// print Master character
		for (int col = 0; col < BOARD_SIZE; col++) {
			fprintf(out, hasFlag(map[row][col], FLAG_LWALL) ? "||" : " |");
			if (hasFlag(map[row][col], FLAG_CAT))
				fprintf(out, " CAT ");
			else if (hasFlag(map[row][col], FLAG_DOG))
				fprintf(out, " DOG ");
			else if (hasFlag(map[row][col], FLAG_MOUSE))
				fprintf(out, "MOUSE");
			else
				fprintf(out, "     ");
			fprintf(out, hasFlag(map[row][col], FLAG_RWALL) ? "||" : "| ");
		}
		fprintf(out, "\n");
		// print sub characters
		for (int col = 0; col < BOARD_SIZE; col++) {
			fprintf(out, hasFlag(map[row][col], FLAG_LWALL) ? "||" : " |");
			fprintf(out, hasFlag(map[row][col], FLAG_FISH) ? "f " : "  ");
			fprintf(out, hasFlag(map[row][col], FLAG_TRAP) ? "×" : " ");
			fprintf(out, hasFlag(map[row][col], FLAG_CHOCO) ? " +" : "  ");
			fprintf(out, hasFlag(map[row][col], FLAG_RWALL) ? "||" : "| ");
		}
		fprintf(out, "\n");
		for (int col = 0; col < BOARD_SIZE; col++)
			fprintf(out, " +-----+ ");
		fprintf(out, "\n");
	}
	return fclose(out) != -1;
}