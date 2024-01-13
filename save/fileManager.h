#pragma once
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>
#include "../logics/diceManager.h"
#include "../windows/error.h"
#include "../constants.h"
#include "../map.h"

typedef struct {
	int _currIndex, _currPlayer, _currRound, _currPlayerMoves, _indicSort[CAT_COUNT], _map[BOARD_SIZE][BOARD_SIZE];
	CAT _cats[CAT_COUNT];
	MOUSE _mouses[MOUSE_COUNT];
	DOG _dogs[DOG_COUNT];
	FISH _fishes[FISH_COUNT];
	DICE _dices[CAT_COUNT];
} gameState;

int saveGameState(const char*, gameState*, ALLEGRO_DISPLAY*);

bool forceCreateDir(const char* dirName) {
	struct stat sb;
	bool existence = false;
	bool temp = stat(dirName, &sb);
	if (!temp && (S_IFDIR & sb.st_mode))
		existence == true;
	if(!existence)
		return al_make_directory(dirName);
	return true;
}

bool fileExists(const char* fileName) {
	struct stat buffer;
	return !stat(fileName, &buffer);
}

FILE* createFile(const char*fileName,ALLEGRO_DISPLAY *displayBuff) {
	bool existence = fileExists(fileName);
	FILE* newFile = fopen(fileName, existence ? "at" : "wt");
	if (!newFile)
		showError(displayBuff, "File cannot be created", "this file in this location cannot be created! \n");
	return newFile;
}

// for making log from game at the end of game
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

void generateGameSate(gameState* _state) {
	// ---- generate ----- generate the map
	int row, col;
	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
			_state->_map[row][col] = map[row][col];
	// ---- end generate ------------------
	// ---- generate ----------- cats -----
	for (col = 0; col < CAT_COUNT; col++)
		_state->_cats[col] = cats[col];
	// ---- end generate ------------------
	// ---- generate ----------- fishes ---
	for (col = 0; col < FISH_COUNT;col++)
		_state->_fishes[col] = fishes[col];
	// ---- end generate ------------------
	// ---- generate ----------- mouses ---
	for (col = 0; col < MOUSE_COUNT; col++)
		_state->_mouses[col] = mouses[col];
	// ---- end generate ------------------
	// ---- generate ----------- dogs -----
	for (col = 0; col < DOG_COUNT; col++)
		_state->_dogs[col] = dogs[col];
	// ---- end generate ------------------
	// ---- generate ----------- dices ----
	for (col = 0; col < CAT_COUNT; col++)
		_state->_dices[col] = DICES[col];
	// ---- end generate ------------------
	// ---- generate ------ specialists ---
	_state->_currPlayer = currentPlayer;
	_state->_currIndex = currentIndex;
	_state->_currPlayerMoves = currentPlayerMoves;
	_state->_currRound = currentRound;
	for (col = 0; col < CAT_COUNT; col++)
		_state->_indicSort[col] = indicateSort[col];
	// ---- end generate ------------------
}

/// <summary>
/// tries to save the game state to the filename...
/// </summary>
/// <param name="fileName">the file name which you want to save state on it!</param>
/// <param name="_state">the game state</param>
/// <returns>
/// 0 - if the file maked successfully
/// 1 - if the file cannot opened
/// 2 - if the file cannot closed(saved)
/// 3 - if the application cannot write on the file.
/// </returns>
int saveGameState(const char* fileName,gameState* _state, ALLEGRO_DISPLAY* displayBuff) {
	FILE* out = fopen(fileName, "wb");
	if (!out) {
		showError(displayBuff, "File cannot be created", "this file in this location cannot be created!");
		return 1;
	}
	if (fwrite(_state, sizeof(gameState), 1, out) == EOF) {
		showError(displayBuff, 
			"File cannot be written", 
			"this file cannot be write by this user, maybe should have adminstrator privillage / permission!");
		return 3;
	}
	if (fclose(out)) {
		showError(displayBuff, 
			"File cannot be saved", 
			"the file cannot be saved. maybe the path of file was changed or the file deleted");
		return 2;
	}
	return 0;
}

/// <summary>
/// tries to load the games
/// </summary>
/// <param name="fileName">the specify name of the file</param>
/// <param name="displayBuff">the <type>ALLEGRO_DISPLAY</type> dialog is reperesented at it</param>
/// <returns>
/// 1 - if the game state loaded successfully
/// 0 - if the game state cannot be loaded or corrupted
/// </returns>
int loadGameState(const char* fileName,ALLEGRO_DISPLAY* displayBuff,gameState* stateBuffer) {
	if (fileExists(fileName)) {
		FILE* in = fopen(fileName, "rb");
		if (!in || feof(in)) {
			showError(displayBuff,
				"the data cannot be loaded! ", 
				"the file cannot be opened. maybe curropted or opened or don't have enough permissions!");
			return 0;
		}
		fread(stateBuffer, sizeof(gameState), 1, in);
		return 1;
	}
	else {
		showNotFoundErr(displayBuff, "DATA", "save/game.dat");
		return 0;
	}
}

void enableGameState(gameState* stateBuffer) {
	// ---- generate ----- generate the map
	int row, col;
	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
			map[row][col] = stateBuffer->_map[row][col];
	// ---- end generate ------------------
	// ---- generate ----------- cats -----
	for (col = 0; col < CAT_COUNT; col++)
		cats[col] = stateBuffer->_cats[col];
	// ---- end generate ------------------
	// ---- generate ----------- fishes ---
	for (col = 0; col < FISH_COUNT; col++)
		fishes[col] = stateBuffer->_fishes[col];
	// ---- end generate ------------------
	// ---- generate ----------- mouses ---
	for (col = 0; col < MOUSE_COUNT; col++)
		mouses[col] = stateBuffer->_mouses[col];
	// ---- end generate ------------------
	// ---- generate ----------- dogs -----
	for (col = 0; col < DOG_COUNT; col++)
		dogs[col] = stateBuffer->_dogs[col];
	// ---- end generate ------------------
	// ---- generate ----------- dices ----
	for (col = 0; col < CAT_COUNT; col++)
		DICES[col] = stateBuffer->_dices[col];
	// ---- end generate ------------------
	// ---- generate ------ specialists ---
	currentPlayer = stateBuffer->_currPlayer;
	currentIndex = stateBuffer->_currIndex;
	currentPlayerMoves =stateBuffer->_currPlayerMoves;
	currentRound = stateBuffer->_currRound;
	for (col = 0; col < CAT_COUNT; col++)
		indicateSort[col] = stateBuffer->_indicSort[col];
	// ---- end generate ------------------
}