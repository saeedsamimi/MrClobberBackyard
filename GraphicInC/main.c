#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "map.h"
#include "constants.h"
#include "flagManager.h"
#include "logics/logics.h"

void printEmptyBoard();
char initializeDisplay();
void printPlayers();
void indicatePlayer();

int currentPlayer = 0;
ALLEGRO_DISPLAY* display;
const short int k = SQUARE_SIZE + 2 * MARGIN;

void testMap() {
	printf("start testing map: \n");
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) 
			printf(" %-3d ", map[i][j]);
		printf("\n");
	}
}

int main() {
	setMap();
	testMap();
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
		default:
			al_clear_to_color(al_map_rgb(150, 150, 150));
			break;
		}
		// ------ End of Handling -------------------------------------
	}
	printEmptyBoard();
	printPlayers();
	indicatePlayer();
	ALLEGRO_EVENT_QUEUE* ev_queue = al_create_event_queue();
	al_register_event_source(ev_queue, al_get_display_event_source(display));
	ALLEGRO_EVENT event;
	al_flip_display();
	//-------------- Test Section ----------------
	int test_x,test_y,test_move_num;
	enum MOVEMENT test_move;
	scanf("%d %d %d",&test_x,&test_y,&test_move_num);
	switch (test_move_num)
	{
	case 1:
		//UP
		test_move = UP;
		break;
	case 2:
		//RIGHT
		test_move = RIGHT;
		break;
	case 3:
		//DOWN
		test_move = DOWN;
		break;
	case 4:
		//LEFT
		test_move = LEFT;
		break;
	case 5:
		//UP RIGHT
		test_move = UP_RIGHT;
		break;
	case 6:
		//UP LEFT
		test_move = UP_LEFT;
		break;
	case 7:
		//DOWN LEFT
		test_move = DOWN_LEFT;
		break;
	case 8:
		//DOWN RIGHT
		test_move = DOWN_RIGHT;
		break;
	default:
		break;
	}
	if(canMove(test_x,test_y,test_move)) printf("Can move!");
	else printf("SHIIIIIIIIT");


	//--------------------------------------------
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
	if (!(display = al_create_display((SQUARE_SIZE + 2 * MARGIN) * BOARD_SIZE, (SQUARE_SIZE + 2 * MARGIN) * BOARD_SIZE)))
		return INIT_DISPLAY_ERR;
	if (!al_init_primitives_addon()) return INIT_DISPLAY_PRIMITIVES_ERR;
	return INIT_DISPLAY_SUCCESS;
}

void printEmptyBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
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