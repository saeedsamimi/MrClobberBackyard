#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "map.h"
#include "test.h"
#include "constants.h"
#include "logics/logics.h"
#include "logics/diceManager.h"
#include "windows/startWin.h"
#include "windows/error.h"
#include "windows/finishWin.h"
#include <locale.h>
#include <limits.h>
#ifdef _WIN32
// WindowsX32-specific includes
#include <windows.h>
#elif _WIN64
// WindowsX64-specific includes
#include <windows.h>
#elif __linux__
// Linux-specific includes
#include <unistd.h>
#elif __APPLE__
// macOS-specific includes
#include <unistd.h>
#endif

void printAcceptMove(char);
void printDiceHint(char);
void printDiceBoard(char);
void __drawScaledPhoto(ALLEGRO_BITMAP*, float, float, float);
void printEmptyBoard();
char initializeDisplay();
void printPlayers();
void clearCats();
void printCats();
void clearDogs();
void printDogs();
void clearMouses();
void printMouses();
void printChocolatesAndFishes();
void nextPlayer(bool);
void indicatePlayer();
void printScoreBoard();
void clearSquare(int, int);
void gameLoop(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT*);
void freeCache();
void finishBoard();
void moveCurrentPlayerOnBoard(int, int);
void GUI();
void closeApp();
int currentPlayer, indicateSort[4] = { 0 }, currentIndex = 0, currentRound = 1;
char currentPlayerMoves = 1, diceRolled = 0;
ALLEGRO_FONT* Font;
ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP* dogIcon[CAT_COUNT], *diceIcon[7], * mouseIcon, * chocoIcon, * fishIcon;
ALLEGRO_EVENT_QUEUE* EVQ;
const short int k = SQUARE_SIZE + 2 * MARGIN;

int main() {
	setlocale(LC_ALL, "en-US.UTF-8");
#ifdef _WIN32
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	GUI();
#elif _WIN64
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	GUI();
#elif __linux__
	if (fork() == 0)
		GUI();
#elif __APPLE__
	if (fork() == 0)
		GUI();
#endif
	return 0;
}

// after hiding the console this GUI will be appear
void GUI() {
	setMap();
	switch (runStartWin()) {
	case -1:
		printf("an error occured!");
		exit(1);
	case 2:
		printf("PROGRAM FINISHED.\n");
		exit(0);
	}
	if (!forceCreateDir("save")) {
		showError(NULL, "the save folder corrupted!", "the save folder cannot be created with force please check the folder permissions have gotten");
		exit(1);
	}
	// deleting unneccary tests
	char init_result = initializeDisplay();
	if (init_result == INIT_DISPLAY_SUCCESS)
		printf("The display created successfully!");
	else
		exit(init_result);
	printEmptyBoard();
	printPlayers();
	printDiceBoard(1);

	al_flip_display(); // refresh the view

	al_install_keyboard();
	EVQ = al_create_event_queue();
	al_register_event_source(EVQ, al_get_display_event_source(display));
	al_register_event_source(EVQ, al_get_keyboard_event_source());
	ALLEGRO_EVENT event;
	// only for test this section , this is going to be changed as soon as possible
	gameLoop(EVQ, &event);
}

// runs the main loop such as manage moving characters and moving items and choosing best player ...
void gameLoop(ALLEGRO_EVENT_QUEUE* ev_queue, ALLEGRO_EVENT* ev) {
	//------- Check For Game Roundes -------
	while (1) {
		if (currentRound > ROUNDS_NUMBER) finishBoard();
		// if the dice not rolled please roll it!
		if (!diceRolled) {
			clearCats();
			printCats();
			printDiceHint(1);
			printAcceptMove(0);
			al_flip_display();
		}
		al_wait_for_event(ev_queue, ev);
		while (diceRolled != 1) {
			if (ev->type == ALLEGRO_EVENT_KEY_UP && ev->keyboard.keycode == ALLEGRO_KEY_T) {
				printDiceBoard(0);
				al_flip_display();
			}
			// if user wants to exit prevent!
			else if (ev->type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				showError(display, "cannot exit", "you cannot exit while rolling dices!");
			// wait until next event occuredW
			if (diceRolled != 1)
				al_wait_for_event(ev_queue, ev);
		}
		printDiceHint(0);
		clearMouses();
		if (ev->type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			closeApp();
		if (ev->type == ALLEGRO_EVENT_KEY_UP) {
			al_flip_display();
			switch (ev->keyboard.keycode) {
			case ALLEGRO_KEY_A:
			case ALLEGRO_KEY_LEFT:
				if (canMove(cats[currentPlayer].x, cats[currentPlayer].y, LEFT)) {
					moveCurrentPlayerOnBoard(cats[currentPlayer].x - 1, cats[currentPlayer].y);
					nextPlayer(0);
				}
				break;
			case ALLEGRO_KEY_D:
			case ALLEGRO_KEY_RIGHT:
				if (canMove(cats[currentPlayer].x, cats[currentPlayer].y, RIGHT)) {
					moveCurrentPlayerOnBoard(cats[currentPlayer].x + 1, cats[currentPlayer].y);
					nextPlayer(0);
				}
				break;
			case ALLEGRO_KEY_W:
			case ALLEGRO_KEY_UP:
				if (canMove(cats[currentPlayer].x, cats[currentPlayer].y, UP)) {
					moveCurrentPlayerOnBoard(cats[currentPlayer].x, cats[currentPlayer].y - 1);
					nextPlayer(0);
				}
				break;
			case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_DOWN:
				if (canMove(cats[currentPlayer].x, cats[currentPlayer].y, DOWN)) {
					moveCurrentPlayerOnBoard(cats[currentPlayer].x, cats[currentPlayer].y + 1);
					nextPlayer(0);
				}
				break;
			case ALLEGRO_KEY_M:
				if (currentPlayerMoves > 1)
					nextPlayer(1);
				break;
			}
		}
		// ---- REMOVE THIS SECTION ----
		clearDogs();
		printCats();
		indicatePlayer();
		printChocolatesAndFishes();
		printDogs();
		printMouses();
		printScoreBoard();
		//------------------------------
		al_flip_display();
	}
}

// -- init -- allegro display settings
char initializeDisplay() {
	if (!al_init()) return INIT_DISPLAY_ALLEGRO_ERR;
	display = al_create_display((SQUARE_SIZE + 2 * MARGIN) * BOARD_SIZE + SCORE_BOARD_WIDTH, (SQUARE_SIZE + 2 * MARGIN) * BOARD_SIZE);
	if (!display) {
		showError(NULL, "Create display failed", "Please try again!");
		return INIT_DISPLAY_ERR;
	}
	if (!al_init_primitives_addon()) {
		showError(display, "Failed to load allegro-addons", "the allegro-primitives addon cannot be initialized!"
			"\n please check the library exist.");
		return INIT_DISPLAY_PRIMITIVES_ERR;
	}
	if (!al_init_font_addon()) {
		showError(display, "Failed to load allegro-addons", "the allegro-font addon cannot be initialized!"
			"\n please check the library exist.");
		return INIT_DISPLAY_FONT_ERR;
	}
	if (!al_init_image_addon()) {
		showError(display, "Failed to load allegro-addons", "the allegro-image addon cannot be initialized!"
			"\n please check the library exist.");
		return INIT_DISPLAY_IMAGE_ADDON_ERR;
	}
	if (!al_init_ttf_addon()) {
		showError(display, "Failed to load allegro-addons", "the allegro-TrueTypeFont(ttf.) addon cannot be initialized!"
			"\n please check the library exist.");
		return INIT_DISPLAY_TRUETYPE_FONT_ERR;
	}
	Font = al_load_ttf_font("src/NotoSerif-Medium.ttf", 24, 0);
	if (!Font) {
		showNotFoundErr(display, "Font", "src/NotoSerif-Medium.ttf");
		return INIT_DISPLAY_FONT_NOT_FOUND_ERR;
	}
	char temp[14];
	for (int i = 0; i < DOG_COUNT; i++) {
		sprintf(temp, "src/dog%d.png", i + 1);
		dogIcon[i] = al_load_bitmap(temp);
		if (!dogIcon[i]) {
			showNotFoundErr(display, "Image", temp);
			return INIT_DISPLAY_IMG_NOT_FOUND;
		}
	}
	// --- load images --- dices
	for (int i = 0; i <= 6; i++) {
		sprintf(temp, "src/dice%d.png", i);
		diceIcon[i] = al_load_bitmap(temp);
		if (!diceIcon[i]) {
			showNotFoundErr(display, "Image", temp);
			return INIT_DISPLAY_IMG_NOT_FOUND;
		}
	}
	mouseIcon = al_load_bitmap("src/mouse.png");
	chocoIcon = al_load_bitmap("src/chocolate.png");
	fishIcon = al_load_bitmap("src/fish.png");
	if (!mouseIcon) {
		showNotFoundErr(display, "Image", "src/mouse.png");
		return INIT_DISPLAY_IMG_NOT_FOUND;
	}
	if (!chocoIcon) {
		showNotFoundErr(display, "Image", "src/chocolate.png");
		return INIT_DISPLAY_IMG_NOT_FOUND;
	}
	if (!fishIcon) {
		showNotFoundErr(display, "Image", "src/fish.png");
		return INIT_DISPLAY_IMG_NOT_FOUND;
	}
	al_set_window_position(display, 300, 40);
	al_set_display_icon(display, img);
	return INIT_DISPLAY_SUCCESS;
}

// free the cache before closing the window
void freeCache() {
	for (int i = 0; i < CAT_COUNT; i++) 
		al_destroy_bitmap(dogIcon[i]);
	al_destroy_bitmap(chocoIcon);
	al_destroy_bitmap(fishIcon);
	al_destroy_bitmap(mouseIcon);
	al_destroy_event_queue(EVQ);
	al_destroy_display(display);
}

// becuase this part of application needs many calculations. i have written unapproprietly!
// this function can print the score board with its information
void printScoreBoard() {
	const int x = k * BOARD_SIZE; //the basic left offset
	int h = al_get_font_line_height(Font)*2;  //get the height of a line of text then multiply 2
	// draw the two important rectangles on the top of score board
	al_draw_filled_rectangle(x, 0, x + SCORE_BOARD_WIDTH / 2, h, COLOR3);
	al_draw_filled_rectangle(x + SCORE_BOARD_WIDTH / 2, 0, x + SCORE_BOARD_WIDTH, h, COLOR3);
	// generate the player turn and game round caption by Sprintf function and then draw it
	char playerTurnText[15], gameRoundText[9];
	sprintf(playerTurnText, "Player %d turn", currentPlayer + 1);
	sprintf(gameRoundText, "Round %d", currentRound);
	al_draw_text(Font, BLACK, x + SCORE_BOARD_WIDTH / 4, h/4, ALLEGRO_ALIGN_CENTER, playerTurnText);
	al_draw_text(Font, BLACK, x + SCORE_BOARD_WIDTH * 3 / 4, h / 4, ALLEGRO_ALIGN_CENTER, gameRoundText);
	// solve width for each item
	const int w = SCORE_BOARD_WIDTH / 4;
	// draw empty table cells
	for (int i = 0; i < 4; i++) {
		al_draw_filled_rectangle(x + i * w, h, x + (i + 1) * w, h + 4 * h / 5, COLOR4);
		al_draw_text(Font, BLACK,
			x + i * w + w / 2, h + h / 8,
			ALLEGRO_ALIGN_CENTER, TITLES[i]);
	}
	// use y property for saving information for text displacement y
	const float y = 9 * h / 5;
	h = h * .8; // minimize h
	char t[6];  // use a temp char array for saving the strings of items
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < CAT_COUNT; j++) {
			al_draw_filled_rectangle(x + i * w, y + indicateSort[j] * h, x + (i + 1) * w, y + (indicateSort[j] + 1) * h, cats[indicateSort[j]].color);
			// use switch case for print the current column data
			switch (i) {
			case 0:
				sprintf(t, "%s", indicateSort[j] == currentPlayer ? "<<>>" : "");
				break;
			case 1:
				sprintf(t, "%d", cats[indicateSort[j]].defencePoint);
				break;
			case 2:
				sprintf(t, "%d", cats[indicateSort[j]].attackPoint);
				break;
			case 3:
				sprintf(t, "%d", cats[indicateSort[j]].mousePoint);
				break;
			}
			al_draw_text(Font, WHITE, x + (i + .5) * w, y + (indicateSort[j] + .5) * h - 15,
				ALLEGRO_ALIGN_CENTER, t);
		}
	}
}

// prints the dice board ... has three states
// 1-the dices have no value and the table is empty and unknown
// 2-the dices have value but not fixed then just print those at the unknown (gray) location
// 3-the dices have correct values and then resume the game
void printDiceBoard(char mode) {
	float h = al_get_font_line_height(Font);  //get the height of a line of text then multiply 2
	const float x = BOARD_SIZE * k;
	const float score_board_H = 10 * h;
	const float y = score_board_H + MARGIN;
	const float diceW = SCORE_BOARD_WIDTH / 4 - MARGIN;
	const float diceH = diceW + 2 * MARGIN + 3 * h;
	// printDiceBoard
	if (!mode) {
		if (diceRolled == 0) {
			diceRolled = toIndex(indicateSort) ? 1 : 2;
		}
		else {
			improveIndexes(indicateSort);
			diceRolled = 1;
		}
		if (diceRolled != 2) {
			currentPlayer = indicateSort[currentIndex];
			printf("current player: %d with index %d\n", currentPlayer, currentIndex);
		}
	}
	else {
		diceRolled = 0;
		al_draw_filled_rectangle(x, y, x + SCORE_BOARD_WIDTH, y + diceH, al_map_rgb(220, 220, 220));
		for (int i = 0; i < CAT_COUNT; i++) {
			__drawScaledPhoto(diceIcon[0], x + i * (diceW + MARGIN) + MARGIN, y + MARGIN, diceW);
			float cx = x + i * (diceW + MARGIN) + MARGIN + diceW / 2;
			float cy = y + diceW + diceW / 2;
			al_draw_filled_circle(cx, cy, diceW / 4, al_map_rgb(234, 234, 234));
		}
	}
	if (!mode) {
		for (int i = 0; i < CAT_COUNT; i++) {
			DICE temp = DICES[i];
			__drawScaledPhoto(diceIcon[temp.diceInt], x + temp.cat_index * (diceW + MARGIN) + MARGIN, y + MARGIN, diceW);
			float cx = x + temp.cat_index * (diceW + MARGIN) + MARGIN + diceW / 2;
			float cy = y + diceW + diceW / 2;
			if (temp.fixed)
				al_draw_filled_circle(cx, cy, diceW / 4, cats[temp.cat_index].color);
			else
				al_draw_filled_circle(cx, cy, diceW / 4, al_map_rgb(234, 234, 234));
			char t[4] = "-";
			if (temp.fixed)
				sprintf(t, "#%d", temp.index + 1);
			al_draw_text(Font, BLACK, cx, cy - h / 2, ALLEGRO_ALIGN_CENTER, t);
		}
	}
}

// print the PRESS M TO ACCEPT MOVE
void printAcceptMove(char shown) {
	const int x = k * BOARD_SIZE + SCORE_BOARD_WIDTH / 2; //the basic left offset
	const int w = al_get_text_width(Font, "Please press M for accept moves!");
	const int fontH = al_get_font_line_height(Font);
	const int h = 14 * fontH + SCORE_BOARD_WIDTH / 4 + MARGIN;
	al_draw_filled_rectangle(x - w / 2 - 10, h - fontH / 2, x + w / 2 + 10, 1.5 * fontH + h, shown ? COLOR2 : WHITE);
	if (shown)
		al_draw_text(Font, BLACK, x, h, ALLEGRO_ALIGN_CENTER, "Please press M for accept moves!");
}

// print initialized map with walls but no player placed at squares
// only prints the empty board not anything else
void printEmptyBoard() {
	al_clear_to_color(WHITE);
	for (int i = 0; i < BOARD_SIZE; i++) 
		for (int j = 0; j < BOARD_SIZE; j++) {
			// solve and draw the dimensions of rectangle
			float x = i * k + MARGIN;
			float y = j * k + MARGIN;
			al_draw_filled_rectangle(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE, COLOR4);
			// adjust pivot x and y
			x -= MARGIN;
			y -= MARGIN;
			// draw walls
			if (hasFlag(map[j][i], FLAG_UWALL))
				al_draw_filled_rectangle(x, y, x + k, y + MARGIN, COLOR1);
			if(hasFlag(map[j][i], FLAG_DWALL))
				al_draw_filled_rectangle(x, y + k, x + k, y - MARGIN + k, COLOR1);
			if(hasFlag(map[j][i], FLAG_RWALL))
				al_draw_filled_rectangle(x + k, y + k, x - MARGIN + k, y, COLOR1);
			if(hasFlag(map[j][i], FLAG_LWALL))
				al_draw_filled_rectangle(x, y, x + MARGIN, y + k, COLOR1);
		}
}

// draw a photo in given size this function can not helpful everywhere
void __drawScaledPhoto(ALLEGRO_BITMAP* img, float x, float y, float w) {
	al_draw_scaled_bitmap(img, 0, 0,
		al_get_bitmap_width(img), al_get_bitmap_height(img), x, y, w, w, 0);
}

// print initial players only once
// this function prints all types of models
void printPlayers() {
	// ------ print ---- cat
	float x = k * (cats[0].x + .25);
	float y = k * (cats[0].y + .25);
	for (int i = 0; i < CAT_COUNT; i++) 
		al_draw_filled_circle(x + ((i % 2) ? k / 2 : 0),
			y + ((i > 1) ? k / 2 : 0), 0.2 * SQUARE_SIZE, cats[i].color);
	// ------ endPrint-- cat
	printDogs();
	printMouses();
	printChocolatesAndFishes();
}

// this function is show that where is the current player's cat 
void indicatePlayer() {
	// define current player
	CAT currentCat = cats[currentPlayer];
	// -- if the box contains only one cat -- then draw a big circle
	if (currentCat.index == 0) 
		al_draw_circle(k * (currentCat.x + .5), k * (currentCat.y + .5),
			.4 * SQUARE_SIZE,BLACK,5);
	// -- else -- draw mini circles if it's location depends on the current cat index
	else {
		float x = k * (currentCat.x + .25); //solve x and y
		float y = k * (currentCat.y + .25); //solve x and y
		// draw mini circle
		al_draw_circle(x + ((currentCat.index % 2) ? 0 : k / 2),
			y + ((currentCat.index > 2) ? k / 2 : 0), 0.15 * SQUARE_SIZE, BLACK, 4);
	}
}

// clear previous state of cats
void clearCats() {
	for (int i = 0; i < CAT_COUNT; i++) {
		float x = cats[i].x * k + MARGIN;
		float y = cats[i].y * k + MARGIN;
		al_draw_filled_rectangle(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE, COLOR4);
	}
}

// print new state of cats
void printCats() {
	float x, y;
	for (int i = 0; i < CAT_COUNT; i++) {
		// Eat Mouses at current position
		eat(cats[i].x,cats[i].y,i);
		//-------------------------------
		if (cats[i].index) {
			x = cats[i].x * k + ((cats[i].index % 2) ? k / 4 : 3 * k / 4); // solve x
			y = cats[i].y * k + ((cats[i].index <= 2) ? k / 4 : 3 * k / 4); // solve y
			al_draw_filled_circle(x, y, SQUARE_SIZE * .2, cats[i].color);
		}
		else {
			x = (cats[i].x + .5) * k;
			y = (cats[i].y + .5) * k;
			al_draw_filled_circle(x, y, SQUARE_SIZE * .4, cats[i].color);
		}
	}
	printChocolatesAndFishes();
}

// this complicated function allows you to move cats if that can move there
void moveCurrentPlayerOnBoard(int newX, int newY){
	if(cats[currentPlayer].freeze >=0) {
		clearCats();
		clearMouses();
		eat(newX, newY, currentPlayer);
		fight(newX,newY,0,currentPlayer);
		trap(newX,newY,currentPlayer);
		// reset old house by counting the previous items count
		int countOvers = 0;
		int indexes[CAT_COUNT];
		for (int i = 0; i < CAT_COUNT; i++)
			if (cats[i].x == cats[currentPlayer].x && cats[i].y == cats[currentPlayer].y)
				indexes[countOvers++] = i;
		if (countOvers > 2) {
			for (int i = 0,k=0; i < countOvers; i++) {
				if (indexes[i] == currentPlayer)continue;
				cats[indexes[i]].index = ++k;
			}
		} else {
			for (int i = 0; i < countOvers; i++)
				cats[indexes[i]].index = 0;
		}
		if (countOvers == 1) // do this when there is no cat after move the cat
			removeFlag(&map[cats[currentPlayer].y][cats[currentPlayer].x], FLAG_CAT);
		// reset current player index if not reseted
		cats[currentPlayer].index = 0;
		// check new house before travel to it
		countOvers = 0;
		for (int i = 0; i < CAT_COUNT; i++)
			if (cats[i].x == newX && cats[i].y == newY)
				indexes[countOvers++] = i;
		if (countOvers == 1) cats[indexes[0]].index = 1;
		cats[currentPlayer].index = countOvers ? countOvers + 1 : 0; // count overs is not zero let c+1 else zero
		if (!countOvers) 
			addFlag(&map[newY][newX], FLAG_CAT); //set this house khown as cat house
		cats[currentPlayer].x = newX;
		cats[currentPlayer].y = newY;
		
		clearSquare(cats[currentPlayer].x, cats[currentPlayer].y);
	}
}

// clear the previous dog locations
void clearDogs() {
	// ------ clear ---- dogs
	for (int i = 0; i < DOG_COUNT; i++) {
		float x = dogs[i].x * k + MARGIN;
		float y = dogs[i].y * k + MARGIN;
		al_draw_filled_rectangle(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE, COLOR4);
	}
	// ------ end clear --- dogs
}

// print new dogs location
void printDogs() {
	// ------ print ---- dogs
	for (int i = 0; i < DOG_COUNT; i++) {
		if(dogs[i].x != REMOVED_DOG && dogs[i].y != REMOVED_DOG) {
			float x = k * dogs[i].x + MARGIN + .15 * SQUARE_SIZE;
			float y = k * dogs[i].y + MARGIN;
			__drawScaledPhoto(dogIcon[i], x, y, .7 * SQUARE_SIZE);
		}
	}
	// ------ endPrint - dogs
}

// clear previous mouses
void clearMouses() {
	// ------ clear ---- mouses
	for (int i = 0; i < MOUSE_COUNT; i++) {
		float x = mouses[i].x * k + MARGIN;
		float y = mouses[i].y * k + MARGIN;
		al_draw_filled_rectangle(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE, COLOR4);
	}
	// ------ end clear ---- mouses
}

// print new mouses
void printMouses(){
	// ------ print ---- mouses
	for (int i = 0; i < MOUSE_COUNT; i++) {
		if (mouses[i].points != INVALID_MOUSE_POINT) {
			float x = k * mouses[i].x + MARGIN + .15 * SQUARE_SIZE;
			float y = k * mouses[i].y + MARGIN;
			__drawScaledPhoto(mouseIcon, x, y, .7 * SQUARE_SIZE);
		}
	}
	// ------ endPrint - mouses
}

// print chocolates and fishes
// use this function after doing a legal or illegal movement
void printChocolatesAndFishes() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (hasFlag(map[j][i], FLAG_CHOCO)) {
				float x = i * k + MARGIN + .5 * SQUARE_SIZE;
				float y = j * k + MARGIN + .5 * SQUARE_SIZE;
				__drawScaledPhoto(chocoIcon, x, y, .5 * SQUARE_SIZE);
			}
			if (hasFlag(map[j][i], FLAG_FISH)) {
				float x = i * k + MARGIN;
				float y = j * k + MARGIN + .5 * SQUARE_SIZE;
				__drawScaledPhoto(fishIcon, x, y, .5 * SQUARE_SIZE);
			}
		}
	}
}

// the x and y of fish and choco the remove the previous items
void clearSquare(int x, int y) {
	al_draw_filled_rectangle(x * k + MARGIN, y * k + MARGIN, x * k + MARGIN + SQUARE_SIZE, y * k + MARGIN + SQUARE_SIZE, COLOR4);
}

// get the next player and then indicate that
void nextPlayer(bool forceAccept) {
	if (cats[currentPlayer].defencePoint < 0 ||
		currentPlayerMoves >= cats[currentPlayer].defencePoint ||
		currentPlayerMoves >= 3 || (forceAccept && currentPlayerMoves > 1)) {
		cats[currentPlayer].defencePoint -= (cats[currentPlayer].defencePoint >= currentPlayerMoves) ? currentPlayerMoves : 0;
		if (currentIndex == CAT_COUNT - 1) {
			// Add Defence Point to add players one unit
			for (int cat_index = 0; cat_index < CAT_COUNT; cat_index++) {
				if (cats[cat_index].freeze >= 0)
					cats[cat_index].defencePoint++;
				cats[cat_index].freeze++;
			}
			//------------------------------------------
			currentIndex = 0;
			currentRound++;
			clearDogs();
			dogRandomMove();
			printDogs();
			mouseRandomMove();
			printCats();
			printMouses();
			printChocolatesAndFishes();
			printScoreBoard();
			printDiceBoard(1);
		}
		else {
			currentIndex = (currentIndex + 1) % CAT_COUNT;
			currentPlayer = indicateSort[currentIndex];
			printMouses();
			printChocolatesAndFishes();
			printScoreBoard();
			indicatePlayer();
		}
		currentPlayerMoves = 1;
		printAcceptMove(0);
	}
	else {
		printAcceptMove(1);
		currentPlayerMoves++;
	}
}

// for finishing the game
void finishBoard() { 
	freeCache();
	runFinishWin(Font,display, cats);
}

// prints the dice (should T press) Hint
void printDiceHint(char shown) {
	const int x = k * BOARD_SIZE + SCORE_BOARD_WIDTH / 2; //the basic left offset
	const int w = al_get_text_width(Font, "Please press T for rolling dices!");
	const int fontH = al_get_font_line_height(Font);
	const int h = 16 * fontH + SCORE_BOARD_WIDTH / 4 + MARGIN;
	al_draw_filled_rectangle(x - w / 2 - 10, h - fontH / 2, x + w / 2 + 10, 1.5 * fontH + h, shown ? COLOR2 : WHITE);
	if (shown)
		al_draw_text(Font, BLACK, x, h, ALLEGRO_ALIGN_CENTER, "Please press T for rolling dices!");
}

void closeApp() {
	int exitResult = al_show_native_message_box(
		display,
		"Exit",
		"Are you sure to exit?",
		"if you exit from game manualy the certain state of this game won't be saved!\n and you cannot resume the game next time!",
		NULL,
		ALLEGRO_MESSAGEBOX_YES_NO);
	if (exitResult == 1) {
		freeCache();
		al_destroy_font(Font);
		exit(0);
	}
}