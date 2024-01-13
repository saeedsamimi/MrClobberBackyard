#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "save/fileManager.h"
#include <stdio.h>

// for test and debug board in the console environment
void __testMap() {
  FILE* outFile = fopen("log.txt", "wt");
  fprintf(outFile, "start testing map: \n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++)
      fprintf(outFile, " %-3d ", map[i][j]);
    fprintf(outFile, "\n");
  }
  fclose(outFile);
}

void __testLoadState(ALLEGRO_TEXTLOG *log,gameState* _state) {
	// ---- test ----- test the map
	int row, col;
	for (row = 0; row < BOARD_SIZE; row++) {
		for (col = 0; col < BOARD_SIZE; col++)
			al_append_native_text_log(log, " %-3d", _state->_map[row][col]);
		al_append_native_text_log(log, "\n");
	}
	/*// ---- end test ------------------
	// ---- test ----------- cats -----
	for (col = 0; col < CAT_COUNT; col++)
		_state->_cats[col] = cats[col];
	// ---- end test ------------------
	// ---- test ----------- fishes ---
	for (col = 0; col < FISH_COUNT; col++)
		_state->_fishes[col] = fishes[col];
	// ---- end test ------------------
	// ---- test ----------- mouses ---
	for (col = 0; col < MOUSE_COUNT; col++)
		_state->_mouses[col] = mouses[col];
	// ---- end test ------------------
	// ---- test ----------- dogs -----
	for (col = 0; col < DOG_COUNT; col++)
		_state->_dogs[col] = dogs[col];
	// ---- end test ------------------
	// ---- test ----------- dices ----
	for (col = 0; col < CAT_COUNT; col++)
		_state->_dices[col] = DICES[col];
	// ---- end test ------------------
	// ---- test ------ specialists ---
	_state->_currPlayer = currentPlayer;
	_state->_currIndex = currentIndex;
	_state->_currPlayerMoves = currentPlayerMoves;
	_state->_currRound = currentRound;
	for (col = 0; col < CAT_COUNT; col++)
		_state->_indicSort[col] = indicateSort[col];
	// ---- end test ------------------*/
}