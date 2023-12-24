#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "map.h"
#include "test.h"
#include <limits.h>

void printEmptyBoard();       // print initialized map with walls but no player placed at squares
char initializeDisplay();     // allegro display initialization and handling errors
void printPlayers();          // print all of the players such as cat, dog, mouse, and more(except walls)
void indicatePlayer();        // indicate the current player in the playboard
void printScoreBoard();       // a function to show side score board menu
void moveCurrentPlayerOnBoard(int, int);  // switches the current player location
ALLEGRO_FONT* font;           // as like as it's name this is a main font configuration
int currentPlayer = 2;        // as like as it's name stores the current player index
int currentRound = 1;         // as like as it's name it stroes the current round information
ALLEGRO_DISPLAY* display;     // as like as it's name stores the information about allegro display
ALLEGRO_BITMAP *dogIcon, *mouseIcon;          // dog and mouse icon bitmap
const short int k = SQUARE_SIZE + 2 * MARGIN; // a helpfull number to save the size of each box

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
		default:
			al_clear_to_color(WHITE);
			break;
		}
		// ------ End of Handling -------------------------------------
	}
	printEmptyBoard();
	printPlayers();
	indicatePlayer();
	printScoreBoard();

	al_install_keyboard();
	ALLEGRO_EVENT_QUEUE* ev_queue = al_create_event_queue();
	al_register_event_source(ev_queue, al_get_display_event_source(display));
	al_register_event_source(ev_queue, al_get_keyboard_event_source());
	ALLEGRO_EVENT event;
	// only for test this section , this is going to be changed as soon as possible
	while (1) {
		al_wait_for_event(ev_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
		if (event.type == ALLEGRO_EVENT_KEY_UP) {
			switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_A:
			case ALLEGRO_KEY_LEFT:
				moveCurrentPlayerOnBoard(6, 7);
				indicatePlayer();
				__testMap();
				break;
			case ALLEGRO_KEY_D:
			case ALLEGRO_KEY_RIGHT:
				moveCurrentPlayerOnBoard(7, 7);
				indicatePlayer();
				__testMap();
				break;
			}
		}
	}
	al_destroy_display(display);
	al_destroy_bitmap(dogIcon);
	return 0;
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
	dogIcon = al_load_bitmap("src/dogIcon.png");
	mouseIcon = al_load_bitmap("src/mouse.png");
	if (!dogIcon && !mouseIcon) return INIT_DISPLAY_IMG_NOT_FOUND;
	return INIT_DISPLAY_SUCCESS;
}

// becuase this part of application needs many calculations. i have written unapproprietly!
// this function can print the score board with its information
void printScoreBoard() {
	const int x = k * BOARD_SIZE; //the basic left offset
	int h = al_get_font_line_height(font)*2;  //get the height of a line of text then multiply 2
	// draw the two important rectangles on the top of score board
	al_draw_filled_rectangle(x, 0, x + SCORE_BOARD_WIDTH / 2, h, COLOR3, 2);
	al_draw_filled_rectangle(x + SCORE_BOARD_WIDTH / 2, 0, x + SCORE_BOARD_WIDTH, h, COLOR3);
	// generate the player turn and game round caption by Sprintf function and then draw it
	char playerTurnText[15], gameRoundText[9];
	sprintf_s(playerTurnText, 15, "Player %d turn", currentPlayer + 1);
	sprintf_s(gameRoundText, 8, "Round %d", currentRound);
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
			al_draw_filled_rectangle(x + i * w, y + j * h, x + (i + 1) * w, y + (j + 1) * h, COLOR2, 2);
			// use switch case for print the current column data
			switch (i) {
			case 0:
				sprintf_s(t, 3, "#%d", j + 1);
				break;
			case 1:
				sprintf_s(t, 3, "%d", cats[j].defencePoint);
				break;
			case 2:
				sprintf_s(t, 3, "%d", cats[j].attackPoint);
				break;
			case 3:
				sprintf_s(t, 3, "%d", cats[j].mousePoint);
				break;
			}
			al_draw_text(font,WHITE, x + (i + .5) * w, y + (j + .5) * h - 15, 
				ALLEGRO_ALIGN_CENTER, t);
		}
	}
	al_flip_display();
}

// print initialized map with walls but no player placed at squares
// only prints the empty board not anything else
void printEmptyBoard() {
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
inline void __drawScaledPhoto(ALLEGRO_BITMAP* img, int x, int y, int w) {
	al_draw_scaled_bitmap(img, 0, 0,
		al_get_bitmap_width(img), al_get_bitmap_height(img), x, y, w, w, 0);
}

// print initial players only once
// this function prints all types of models
void printPlayers() {
	float x, y;
	int i;
	// ------ print ---- cat
	x = k * (cats[0].x + .25);
	y = k * (cats[0].y + .25);
	for (i = 0; i < CAT_COUNT; i++) 
		al_draw_filled_circle(x + ((i % 2) ? k / 2 : 0),
			y + ((i > 1) ? k / 2 : 0), 0.2 * SQUARE_SIZE, cats[i].color);
	// ------ endPrint-- cat
	// ------ print ---- dogs
	for (i = 0; i < DOG_COUNT; i++) {
		x = k * dogs[i].x + MARGIN;
		y = k * dogs[i].y + MARGIN;
		__drawScaledPhoto(dogIcon, x, y, SQUARE_SIZE);
	}
	// ------ endPrint - dogs
	// ------ print ---- mouses
	for (i = 0; i < MOUSE_COUNT; i++) {
		x = k * mouses[i].x + MARGIN;
		y = k * mouses[i].y + MARGIN;
		__drawScaledPhoto(mouseIcon, x, y, SQUARE_SIZE);
	}
	// ------ endPrint - mouses
	al_flip_display();
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
	// refresh display view
	al_flip_display();
}

// this complicated function allows you to move cats if that can move there
void moveCurrentPlayerOnBoard(int newX, int newY) {
	// -- solve -- the x and y of current square
	int x = cats[currentPlayer].x * k + MARGIN;
	int y = cats[currentPlayer].y * k + MARGIN;
	al_draw_filled_rectangle(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE, COLOR4);
	// -- end solve -- and finaly clear to default color by rectangle
	// -- adjust -- adjust previous items and reset to clear and change indexes 
	if (cats[currentPlayer].index)
		for (int i = 0; i < CAT_COUNT; i++) {
			if (i == currentPlayer)continue; // not for current Player
			// if current player overlaped the i th player , 
			// the i th player's index if it is more than currPlayer would be decreased
			if (cats[currentPlayer].x == cats[i].x &&
				cats[currentPlayer].y == cats[i].y &&
				cats[i].index > cats[currentPlayer].index) cats[i].index--;
			x = cats[i].x * k + ((cats[i].index % 2) ? k / 4 : 3 * k / 4); // solve x
			y = cats[i].y * k + ((cats[i].index <= 2) ? k / 4 : 3 * k / 4); // solve y
			al_draw_filled_circle(x, y, SQUARE_SIZE * .2, cats[i].color); // draw small quarter circle
		}
	else removeFlag(&map[cats[currentPlayer].y][cats[currentPlayer].x], FLAG_CAT);
	// -- end adjust -- if the previous item is not overlaped the big circle would be printed
	// -- change -- location and index
	cats[currentPlayer].x = newX;
	cats[currentPlayer].y = newY;
	cats[currentPlayer].index = 0;
	// -- end change --
	// -- if new box contains cat -- adjust the cat index
	if (hasFlag(map[newY][newX], FLAG_CAT)) {
		int k, i;
		for (i = 0, k = 0; i < CAT_COUNT; i++)
			if (cats[i].x == newX && cats[i].y == newY) k++;
		cats[currentPlayer].index = k;
	}
	// -- endif --
	else addFlag(&map[newY][newX], FLAG_CAT); // -- elif -- add this cat to a solid square
	// solve the x and y for the pivot
	x = cats[currentPlayer].x * k + MARGIN; 
	y = cats[currentPlayer].y * k + MARGIN;
	if (cats[currentPlayer].index) {
		// very complicated calculation for the overlapping cats
		x = cats[currentPlayer].x * k + ((cats[currentPlayer].index % 2) ? k / 4 : 3 * k / 4);
		y = cats[currentPlayer].y * k + ((cats[currentPlayer].index < 3) ? k / 4 : 3 * k / 4);
		// draw mini circle
		al_draw_filled_circle(x, y, .2 * SQUARE_SIZE, cats[currentPlayer].color);
	} // elif draw the big circle...
	else al_draw_filled_circle(x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2, .4 * SQUARE_SIZE, cats[currentPlayer].color);
	al_flip_display(); // refresh display
}