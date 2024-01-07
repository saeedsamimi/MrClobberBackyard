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
#include <limits.h>

void printDiceBoard(char);     // for printing the dice board in the area
void __drawScaledPhoto(ALLEGRO_BITMAP*, float, float, float);
void printEmptyBoard();       // print initialized map with walls but no player placed at squares
char initializeDisplay();     // allegro display initialization and handling errors
void printPlayers();          // print all of the players such as cat, dog, mouse, and more(except walls)
void clearCats();             // to clear all cats locatio on board
void printCats();             // for printing cats
void clearDogs();             // clear previous dogs
void printDogs();             // print new dogs
void clearMouses();           // clear previous mouses
void printMouses();           // print new mouses
void printChocolatesAndFishes(); // print all cholates and fishes finally
void nextPlayer();            // switch to the next player
void indicatePlayer();        // indicate the current player in the playboard
void printScoreBoard();       // a function to show side score board menu
void clearSquare(int, int);   // clear the custom square to redifne new character
void gameLoop(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT*,enum MOVEMENT); // the main game loop
void freeCache();    
void finishBoard();         // free cache such as pictures and displays and fonts
void moveCurrentPlayerOnBoard(int, int);  // switches the current player location
ALLEGRO_FONT* font;           // as like as it's name this is a main font configuration
int currentPlayer;        // as like as it's name stores the current player index
int currentIndex = 0;
int currentRound = 1;         // as like as it's name it stroes the current round information
ALLEGRO_DISPLAY* display;     // as like as it's name stores the information about allegro display
ALLEGRO_BITMAP* dogIcon[CAT_COUNT], *diceIcon[7], * mouseIcon, * chocoIcon, * fishIcon;          // dog and mouse icon bitmap
const short int k = SQUARE_SIZE + 2 * MARGIN; // a helpfull number to save the size of each box
int indicateSort[4] = { 0 };
char diceRolled = 0;

int main() {
	setMap();
	__testMap();
	{// ------- Handle Initializing Errs ---------------------------
		switch (initializeDisplay()) {
		case INIT_DISPLAY_ERR:
			printf("display can not be initialized!\n");
			return INIT_DISPLAY_ERR;
		case INIT_DISPLAY_ALLEGRO_ERR:
			printf("the allegro can not be initialized!\n");
			return INIT_DISPLAY_ALLEGRO_ERR;
		case INIT_DISPLAY_PRIMITIVES_ERR:
			printf("can not to load the allegro_primitives library!\n");
			return INIT_DISPLAY_PRIMITIVES_ERR;
		case INIT_DISPLAY_FONT_ERR:
			printf("can not run allegro font addon!\n");
			return INIT_DISPLAY_FONT_ERR;
		case INIT_DISPLAY_TRUETYPE_FONT_ERR:
			printf("can not run allegro TrueType fonts!\n");
			return INIT_DISPLAY_TRUETYPE_FONT_ERR;
		case INIT_DISPLAY_FONT_NOT_FOUND_ERR:
			printf("can not load the font file(\"/NotoSerif - Medium.ttf\")");
			return INIT_DISPLAY_FONT_NOT_FOUND_ERR;
		case INIT_DISPLAY_IMG_NOT_FOUND:
			printf("can not found/load the png files(\"mouseIcon.png or dogIcon.png\")");
			return INIT_DISPLAY_IMG_NOT_FOUND;
		}
		// ------ End of Handling -------------------------------------
	}
	printEmptyBoard();
	printPlayers();
	printScoreBoard();
	printDiceBoard(1);

	al_flip_display(); // refresh the view

	al_install_keyboard();
	ALLEGRO_EVENT_QUEUE* ev_queue = al_create_event_queue();
	al_register_event_source(ev_queue, al_get_display_event_source(display));
	al_register_event_source(ev_queue, al_get_keyboard_event_source());
	ALLEGRO_EVENT event;
	// only for test this section , this is going to be changed as soon as possible
	gameLoop(ev_queue, &event,NO_MOVE);
	return 0;
}

// runs the main loop such as manage moving characters and moving items and choosing best player ...
void gameLoop(ALLEGRO_EVENT_QUEUE* ev_queue, ALLEGRO_EVENT* ev,enum MOVEMENT previous_move) {
	//------- Check For Game Roundes -------
	if(currentRound>ROUNDS_NUMBER) finishBoard();
	al_wait_for_event(ev_queue, ev);
	if (ev->type == ALLEGRO_EVENT_DISPLAY_CLOSE) return;
	if (!diceRolled) {
		clearCats();
		printCats();
		al_flip_display();
	}
	while (diceRolled != 1) {
		if (ev->type == ALLEGRO_EVENT_KEY_UP && ev->keyboard.keycode == ALLEGRO_KEY_T) {
			printDiceBoard(0);
			al_flip_display();
		}
		else if (ev->type == ALLEGRO_EVENT_DISPLAY_CLOSE) return;
		if (diceRolled != 1) al_wait_for_event(ev_queue, ev);
	}
	if (ev->type == ALLEGRO_EVENT_KEY_UP) {
		al_flip_display();
		switch (ev->keyboard.keycode) {
		case ALLEGRO_KEY_A:
		case ALLEGRO_KEY_LEFT:
			if (canMove(cats[currentPlayer].x, cats[currentPlayer].y, LEFT)) {
				moveCurrentPlayerOnBoard(cats[currentPlayer].x - 1, cats[currentPlayer].y);
				nextPlayer();
			}
			__testMap();
			break;
		case ALLEGRO_KEY_D:
		case ALLEGRO_KEY_RIGHT:
			if (canMove(cats[currentPlayer].x, cats[currentPlayer].y, RIGHT)) {
				moveCurrentPlayerOnBoard(cats[currentPlayer].x + 1, cats[currentPlayer].y);
				nextPlayer();
			}
			__testMap();
			break;
		case ALLEGRO_KEY_W:
		case ALLEGRO_KEY_UP:
			if (canMove(cats[currentPlayer].x, cats[currentPlayer].y, UP)) {
				moveCurrentPlayerOnBoard(cats[currentPlayer].x, cats[currentPlayer].y - 1);
				nextPlayer();
			}
			__testMap();
			break;
		case ALLEGRO_KEY_S:
		case ALLEGRO_KEY_DOWN:
			if (canMove(cats[currentPlayer].x, cats[currentPlayer].y, DOWN)) {
				moveCurrentPlayerOnBoard(cats[currentPlayer].x, cats[currentPlayer].y + 1);
				nextPlayer();
			}
			__testMap();
			break;
		}
	}
	// ---- REMOVE THIS SECTION ----
	printCats();
	indicatePlayer();
	printChocolatesAndFishes();
	printDogs();
	printMouses();
	printScoreBoard();
	//------------------------------
	al_flip_display();
	gameLoop(ev_queue, ev,NO_MOVE);
}

// -- init -- allegro display settings
char initializeDisplay() {
	if (!al_init()) return INIT_DISPLAY_ALLEGRO_ERR;
	display = al_create_display((SQUARE_SIZE + 2 * MARGIN) * BOARD_SIZE + SCORE_BOARD_WIDTH, (SQUARE_SIZE + 2 * MARGIN) * BOARD_SIZE);
	if (!display) return INIT_DISPLAY_ERR;
	if (!al_init_primitives_addon()) return INIT_DISPLAY_PRIMITIVES_ERR;
	if (!al_init_font_addon()) return INIT_DISPLAY_FONT_ERR;
	if (!al_init_image_addon()) return INIT_DISPLAY_IMAGE_ADDON_ERR;
	if (!al_init_ttf_addon()) return INIT_DISPLAY_TRUETYPE_FONT_ERR;
	font = al_load_ttf_font("src/NotoSerif-Medium.ttf", 24, 0);
	if (!font) return INIT_DISPLAY_FONT_NOT_FOUND_ERR;
	char temp[14];
	for (int i = 0; i < DOG_COUNT; i++) {
		sprintf(temp, "src/dog%d.png", i + 1);
		dogIcon[i] = al_load_bitmap(temp);
		if (!dogIcon[i]) return INIT_DISPLAY_IMG_NOT_FOUND;
	}
	// --- load images --- dices
	for (int i = 0; i <= 6; i++) {
		sprintf(temp, "src/dice%d.png", i);
		diceIcon[i] = al_load_bitmap(temp);
		if (!diceIcon[i]) return INIT_DISPLAY_IMG_NOT_FOUND;
	}
	mouseIcon = al_load_bitmap("src/mouse.png");
	chocoIcon = al_load_bitmap("src/chocolate.png");
	fishIcon = al_load_bitmap("src/fish.png");
	if (!mouseIcon || !chocoIcon || !fishIcon) return INIT_DISPLAY_IMG_NOT_FOUND;
	al_set_window_position(display, 300, 40);
	return INIT_DISPLAY_SUCCESS;
}

// free the cache before closing the window
void freeCache() {
	for (int i = 0; i < CAT_COUNT; i++) 
		al_destroy_bitmap(dogIcon[i]);
	al_destroy_bitmap(chocoIcon);
	al_destroy_bitmap(fishIcon);
	al_destroy_bitmap(mouseIcon);
	al_destroy_font(font);
	al_destroy_display(display);
}

// becuase this part of application needs many calculations. i have written unapproprietly!
// this function can print the score board with its information
void printScoreBoard() {
	const int x = k * BOARD_SIZE; //the basic left offset
	int h = al_get_font_line_height(font)*2;  //get the height of a line of text then multiply 2
	// draw the two important rectangles on the top of score board
	al_draw_filled_rectangle(x, 0, x + SCORE_BOARD_WIDTH / 2, h, COLOR3);
	al_draw_filled_rectangle(x + SCORE_BOARD_WIDTH / 2, 0, x + SCORE_BOARD_WIDTH, h, COLOR3);
	// generate the player turn and game round caption by Sprintf function and then draw it
	char playerTurnText[15], gameRoundText[9];
	sprintf(playerTurnText, "Player %d turn", currentPlayer + 1);
	sprintf(gameRoundText, "Round %d", currentRound);
	al_draw_text(font, BLACK, x + SCORE_BOARD_WIDTH / 4, h/4, ALLEGRO_ALIGN_CENTER, playerTurnText);
	al_draw_text(font, BLACK, x + SCORE_BOARD_WIDTH * 3 / 4, h / 4, ALLEGRO_ALIGN_CENTER, gameRoundText);
	// a static constant object which is stores the table titles
	const static char TITLES[][7] = { "Player","Energy", "Power","Point" };
	// solve width for each item
	const int w = SCORE_BOARD_WIDTH / 4;
	// draw empty table cells
	for (int i = 0; i < 4; i++) {
		al_draw_filled_rectangle(x + i * w, h, x + (i + 1) * w, h + 4 * h / 5, COLOR4);
		al_draw_text(font, BLACK,
			x + i * w + w / 2, h + h / 8,
			ALLEGRO_ALIGN_CENTER, TITLES[i]);
	}
	// use y property for saving information for text displacement y
	const float y = 9 * h / 5;
	h = h * .8; // minimize h
	char t[3];  // use a temp char array for saving the strings of items
	for (int i = 0; i < CAT_COUNT; i++) {
		for (int j = 0; j < CAT_COUNT; j++) {
			al_draw_filled_rectangle(x + i * w, y + j * h, x + (i + 1) * w, y + (j + 1) * h, COLOR2);
			// use switch case for print the current column data
			switch (i) {
			case 0:
				sprintf(t, "#%d", j + 1);
				break;
			case 1:
				sprintf(t, "%d", cats[j].defencePoint);
				break;
			case 2:
				sprintf(t, "%d", cats[j].attackPoint);
				break;
			case 3:
				sprintf(t, "%d", cats[j].mousePoint);
				break;
			}
			al_draw_text(font,WHITE, x + (i + .5) * w, y + (j + .5) * h - 15, 
				ALLEGRO_ALIGN_CENTER, t);
		}
	}
}

void printDiceBoard(char mode) {
	float h = al_get_font_line_height(font);  //get the height of a line of text then multiply 2
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
		if (diceRolled != 2)
			currentPlayer = indicateSort[currentIndex];
		printf("current player: %d with index %d\n", currentPlayer, currentIndex);
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
			al_draw_text(font, BLACK, cx, cy - h / 2, ALLEGRO_ALIGN_CENTER, t);
		}
	}
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
		printCats();
		printChocolatesAndFishes();
	} else {
		nextPlayer();
		indicatePlayer();
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
void nextPlayer() {
	if (currentIndex == CAT_COUNT - 1) {
		// Add Defence Point to add players one unit
		for(int cat_index=0;cat_index<CAT_COUNT;cat_index++) {
			if(cats[cat_index].freeze >= 0) {
				cats[cat_index].defencePoint++;
			} else {printf("CAT AT %d,%d IS FREEZE AND BE RELEAZED AT %d ROUNDS\n",cats[cat_index].x,cats[cat_index].y, (-1 * cats[cat_index].freeze) );}
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
	} else{ 
		currentIndex = (currentIndex + 1) % CAT_COUNT;
		currentPlayer = indicateSort[currentIndex];
		printMouses();
		printChocolatesAndFishes();
		printScoreBoard();
		indicatePlayer();
	}
}

void finishBoard() { 
	freeCache();
	exit(0); 
}