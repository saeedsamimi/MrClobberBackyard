#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "map.h"
#include <limits.h>

void printEmptyBoard();       // print initialized map with walls but no player placed at squares
char initializeDisplay();     // allegro display initialization and handling errors
void printPlayers();          // print all of the players such as cat, dog, mouse, and more(except walls)
void indicatePlayer();        // indicate the current player in the playboard
void __testMap();             // for test and debug board in the console environment
void printScoreBoard();       // a function to show side score board menu

ALLEGRO_FONT* font;           // as like as it's name this is a main font configuration
int currentPlayer = 0;        // as like as it's name stores the current player index
int currentRound = 1;         // as like as it's name it stroes the current round information
ALLEGRO_DISPLAY* display;     // as like as it's name stores the information about allegro display
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
			break;
		case INIT_DISPLAY_TRUETYPE_FONT_ERR:
			printf("can not run allegro TrueType fonts!\n");
			break;
		case INIT_DISPLAY_FONT_NOT_FOUND_ERR:
			printf("can not load the font file(\"/NotoSerif - Medium.ttf\")");
			break;
		default:
			al_clear_to_color(al_map_rgb(150, 150, 150));
			break;
		}
		// ------ End of Handling -------------------------------------
	}
	printEmptyBoard();
	printPlayers();
	indicatePlayer();
	printScoreBoard();
	ALLEGRO_EVENT_QUEUE* ev_queue = al_create_event_queue();
	al_register_event_source(ev_queue, al_get_display_event_source(display));
	ALLEGRO_EVENT event;
	while (1) {
		al_wait_for_event(ev_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			al_destroy_display(display);
			break;
		}
	}
	return 0;
}

char initializeDisplay() {
	if (!al_init()) return INIT_DISPLAY_ALLEGRO_ERR;
	if (!(display = al_create_display((SQUARE_SIZE + 2 * MARGIN) * BOARD_SIZE + SCORE_BOARD_WIDTH, (SQUARE_SIZE + 2 * MARGIN) * BOARD_SIZE)))
		return INIT_DISPLAY_ERR;
	if (!al_init_primitives_addon()) return INIT_DISPLAY_PRIMITIVES_ERR;
	if (!al_init_font_addon()) return INIT_DISPLAY_FONT_ERR;
	if (!al_init_ttf_addon()) return INIT_DISPLAY_TRUETYPE_FONT_ERR;
	font = al_load_ttf_font("NotoSerif-Medium.ttf", 24, 0);
	if (!font) return INIT_DISPLAY_FONT_NOT_FOUND_ERR;
	return INIT_DISPLAY_SUCCESS;
}

void printScoreBoard() {
	const int x = k * BOARD_SIZE;
	int h = al_get_font_line_height(font)*2;
	al_draw_filled_rectangle(x, 0, x + SCORE_BOARD_WIDTH / 2, h, COLOR3, 2);
	al_draw_filled_rectangle(x + SCORE_BOARD_WIDTH / 2, 0, x + SCORE_BOARD_WIDTH, h, COLOR3);
	char playerTurnText[15], gameRoundText[9];
	sprintf_s(playerTurnText, 15, "Player %d turn", currentPlayer + 1);
	sprintf_s(gameRoundText, 8, "Round %d", currentRound);
	al_draw_text(font, al_map_rgb(0, 0, 0), x + SCORE_BOARD_WIDTH / 4, h/4, ALLEGRO_ALIGN_CENTER, playerTurnText);
	al_draw_text(font, al_map_rgb(0, 0, 0), x + SCORE_BOARD_WIDTH * 3 / 4, h / 4, ALLEGRO_ALIGN_CENTER, gameRoundText);
	const static char TITLES[][7] = { "player","energy", "power","point" };
	const int w = SCORE_BOARD_WIDTH / 4;
	for (int i = 0; i < 4; i++) {
		al_draw_filled_rectangle(x + i * w, h, x + (i + 1) * w, h + 4 * h / 5, COLOR4);
		al_draw_text(font, al_map_rgb(0, 0, 0),
			x + i * w + w / 2, h + h / 8,
			ALLEGRO_ALIGN_CENTER, TITLES[i]);
	}
	const float y = 9 * h / 5;
	h = h * .8;
	for (int i = 0; i < CAT_COUNT; i++) {
		for (int j = 0; j < CAT_COUNT; j++) {
			al_draw_filled_rectangle(x + i * w, y + j * h, x + (i + 1) * w, y + (j + 1) * h, COLOR2, 2);
			char t[3];
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
			al_draw_text(font, al_map_rgb_f(1, 1, 1), x + (i + .5) * w, y + (j + .5) * h - 15, ALLEGRO_ALIGN_CENTER, t);
		}
	}
	al_flip_display();
}

void printEmptyBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) 
		for (int j = 0; j < BOARD_SIZE; j++) {
			float x = i * k + MARGIN;
			float y = j * k + MARGIN;
			al_draw_filled_rectangle(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE, al_map_rgb_f(1, 1, 1));
			x -= MARGIN;
			y -= MARGIN;
			if (hasFlag(map[j][i], FLAG_UWALL))
				al_draw_filled_rectangle(x, y, x + k, y + MARGIN, al_map_rgb(0, 0, 0));
			if(hasFlag(map[j][i], FLAG_DWALL))
				al_draw_filled_rectangle(x, y + k, x + k, y - MARGIN + k, al_map_rgb(0, 0, 0));
			if(hasFlag(map[j][i], FLAG_RWALL))
				al_draw_filled_rectangle(x + k, y + k, x - MARGIN + k, y, al_map_rgb(0, 0, 0));
			if(hasFlag(map[j][i], FLAG_LWALL))
				al_draw_filled_rectangle(x, y, x + MARGIN, y + k, al_map_rgb(0, 0, 0));
		}
}

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
		x = k * (dogs[i].x + .5);
		y = k * (dogs[i].y + .5);
		al_draw_filled_circle(x, y, SQUARE_SIZE * .4, DOG_COLOR);
	}
	// ------ endPrint - dogs
	// ------ print ---- mouses
	for (i = 0; i < MOUSE_COUNT; i++) {
		x = k * (mouses[i].x + .5);
		y = k * (mouses[i].y + .5);
		al_draw_filled_circle(x, y, SQUARE_SIZE * .4, MOUSE_COLOR);
	}
	// ------ endPrint - mouses
	al_flip_display();
}

void indicatePlayer() {
	CAT currentCat = cats[currentPlayer];
	if (currentCat.index == 0) {
		al_draw_circle(k * (currentCat.x + .5), k * (currentCat.y + .5),
			.4 * SQUARE_SIZE,al_map_rgb(0,0,0),5);
	} else {
		float x = k * (currentCat.x + .25);
		float y = k * (currentCat.y + .25);
		al_draw_circle(x + ((currentCat.index % 2) ? 0 : k / 2),
			y + ((currentCat.index > 2) ? k / 2 : 0), 0.15 * SQUARE_SIZE, al_map_rgb(0, 0, 0), 4);
	}
	al_flip_display();
}

void __testMap() {
	printf("start testing map: \n");
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++)
			printf(" %-3d ", map[i][j]);
		printf("\n");
	}
}