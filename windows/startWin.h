#pragma once
#include "allegro5/allegro.h"
#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_native_dialog.h"
#include "../save/fileManager.h"
#include "../test.h"
#include "allegro5/allegro_image.h"
#include "../constants.h"

const char fileNames[][16] = { "src/game.png","src/history.png","src/info.png","src/quit.png" };

ALLEGRO_FONT* Font;
ALLEGRO_BITMAP* img, *icons[4];
const char titles[][15] = { "START NEW GAME","RESUME GAME","ABOUT","QUIT" };
bool isAboutShows = false;
int aboutTextIndex = 0;
#define ABOUT_TXTS_COUNT 4
#define START_HEIGHT 650
#define START_WIDTH 500

typedef struct {
	float x, y;
	float width, height;
	char text[15], right;
	ALLEGRO_BITMAP** imgPointer;
	ALLEGRO_COLOR color;
} GBUTTON;

GBUTTON btnPrevious, btnNext;

int getFontLineHeight();
void drawRectangle(GBUTTON*);
void setBtn(GBUTTON*, float, float, float, float,const char[15],ALLEGRO_BITMAP**);
char inRectangle(GBUTTON*, int, int);
void paint(GBUTTON*, int);
void resetRectangleColor(GBUTTON*);
void setRectangleColor(GBUTTON*, ALLEGRO_COLOR);
int menuClickHandler(GBUTTON*, ALLEGRO_MOUSE_EVENT, int, ALLEGRO_DISPLAY*);
void hoverHandler(GBUTTON*, ALLEGRO_MOUSE_EVENT, int);
void setNextPreviousBtns();

int runStartWin() {
	al_init();
	al_set_new_display_refresh_rate(60);
	al_set_app_name("Mr. Clobber BackYard");
	ALLEGRO_DISPLAY* Display = al_create_display(START_WIDTH, START_HEIGHT);
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	setNextPreviousBtns();

	Font = al_load_ttf_font("src/NotoSerif-Medium.ttf", 24, 0);
	if (!Font) return -1;
	img = al_load_bitmap("src/pic.png");
	if (!img) return -1;
	al_set_display_icon(Display, img);
	for (int i = 0; i < 4; i++) {
		icons[i] = al_load_bitmap(fileNames[i]);
		if (!icons[i]) return -1;
	}
	int rectCount = 4;
	GBUTTON rects[4];
	for (int i = 0; i < rectCount; i++) {
		setBtn(rects + i, i % 2 ? -30 : 30, i * (getFontLineHeight() * 2 + 10), 400, getFontLineHeight() * 2, titles[i],icons + i);
		rects[i].right = i % 2;
	}
	paint(rects, 4);
	ALLEGRO_EVENT_QUEUE* ev_queue = al_create_event_queue();
	al_install_keyboard();
	al_install_mouse();
	al_register_event_source(ev_queue, al_get_keyboard_event_source());
	al_register_event_source(ev_queue, al_get_mouse_event_source());
	al_register_event_source(ev_queue, al_get_display_event_source(Display));
	ALLEGRO_EVENT e;
	while (1) {
		al_wait_for_event(ev_queue, &e);
		switch (e.type) {
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			al_destroy_display(Display);
			al_destroy_font(Font);
			printf("menu closed!\n");
			exit(0);
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (e.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				al_destroy_display(Display);
				al_destroy_font(Font);
				printf("EXIT\n");
				return 0;
			case ALLEGRO_KEY_T:
				isAboutShows = true;
			}
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			int resultCode;
			if (!isAboutShows) {
				if (resultCode = menuClickHandler(rects, e.mouse, rectCount, Display))
					return resultCode;
			}
			else {
				int Dx = e.mouse.x - START_WIDTH + 40;
				int Dy = e.mouse.y - 20;
				// perform the backspace click...
				isAboutShows = abs(Dx) > 16 || abs(Dy) > 16 || sqrt(Dx * Dx + Dy * Dy) > 16;
				if (inRectangle(&btnPrevious, e.mouse.x, e.mouse.y))
					aboutTextIndex--;
				else if (inRectangle(&btnNext, e.mouse.x, e.mouse.y))
					aboutTextIndex++;
				if (aboutTextIndex < 0)
					aboutTextIndex = ABOUT_TXTS_COUNT - 1;
				else if (aboutTextIndex >= ABOUT_TXTS_COUNT)
					aboutTextIndex = 0;
			}
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			hoverHandler(rects, e.mouse, rectCount);
			break;
		}
		paint(rects, rectCount);
	}
}

void paint(GBUTTON* rects, int n) {
	if (!isAboutShows) {
		al_clear_to_color(COLOR4);
		const int W = al_get_bitmap_width(img);
		al_draw_scaled_bitmap(img, 0, 0, W, W, START_WIDTH / 4, 50, START_WIDTH / 2, START_WIDTH / 2, 0);
		al_draw_rectangle(0, 0, START_WIDTH, START_HEIGHT, COLOR1, 20);
		const GBUTTON* end = rects + n;
		while (rects < end)
			drawRectangle(rects++);
	}
	else {
		al_clear_to_color(COLOR2);
		al_draw_text(Font, BLACK, 10, 4, 0, "ABOUT: ");
		al_draw_filled_circle(START_WIDTH - 40, 20, 16, WHITE);
		al_draw_filled_rounded_rectangle(4, 40, START_WIDTH - 4, START_HEIGHT - 4, 10, 10, WHITE);
		al_draw_filled_triangle(START_WIDTH - 50, 10, START_WIDTH - 50, 30, START_WIDTH - 30, 20, BLACK);
		switch (aboutTextIndex) {
		case 0:
			al_draw_text(Font, BLUE, 14, 45, 0, "1 - Overview:");
			al_draw_multiline_text(Font, BLACK, 20, 160, START_WIDTH - 28, 35, 0, 
				"- Turn-based 4 player strategy game inspired by Tom & Jerry cartoons\n"
				"- Players control cat trying to catch mice while avoiding dogs\n"
				"- Takes place on 15x15 grid map with objects like walls, traps, power-ups, food, mice, and dogs\n"
				"- Players take turns moving cats to catch mice and gain points. Most points after 15 rounds wins.");
			break;
		case 1:
			al_draw_text(Font, BLUE, 14, 45, 0, "2 - Objects:");
			al_draw_multiline_text(Font, BLACK, 20, 90, START_WIDTH - 28, 35, 0,
				"- Cat - can move up, down, left, right each turn up to 3 spaces. Lose 1 energy per space moved. Can attack cats/dogs.\n"
				"- Mice - 18 worth 1, 2 or 3 points. Move randomly each turn. Cats gain points by landing on mice.\n"
				"- Dogs - 4 breeds that chase cats. Reduce cat energy/points if win battle.\n"
				"- Traps - Stepping on releases cat's mice or reduces power/energy.\n"
			  "- Power-ups(Chocolate) - Landing boosts cat's attack power.\n"
				"- Food(Fish) - Gain 2-4 energy points by eating fish.");
			break;
		case 2:
			al_draw_text(Font, BLUE, 14, 45, 0, "3 - Rules:");
			al_draw_multiline_text(Font, BLACK, 20, 100, START_WIDTH - 28, 35, 0,
				"- Turn Order - Players roll dice each round to determine order. Stays same that round.\n"
				"- Movement - On turn, move cat up to 3 spaces in 4 directions. Lose 1 energy per space.\n"
				"- Attacking - If cat encounters enemy cat or dog, battle occurs. Higher power/energy ratio wins.\n"
				"- End of Round - Mice & dogs reposition randomly after all players move. Food replenishes if needed.\n");
			break;
		case 3:
			al_draw_text(Font, BLUE, 14, 45, 0, "4 - Winning:");
			al_draw_multiline_text(Font, BLACK, 20, 200, START_WIDTH - 28, 35, 0,
				"- Most mouse points after 15 rounds wins. Ties broken by higher total power/energy.\n"
				"- Final scores displayed and saved to text file.\n"
				"-- You also can save the game resume the game where as you want!");
			break;
		}
		drawRectangle(&btnPrevious);
		drawRectangle(&btnNext);
	}
	al_flip_display();
}

void setBtn(GBUTTON* rect, float offx, float offy, float width, float height,const char name[15],ALLEGRO_BITMAP** bmpPtr) {
	rect->x = (START_WIDTH - width) / 2 + offx;
	rect->y = START_WIDTH / 3 + offy + 170;
	rect->width = width;
	rect->height = height;
	rect->imgPointer = bmpPtr;
	strcpy(rect->text, name);
	rect->color = BLACK;
}

void setRectangleColor(GBUTTON* rect, ALLEGRO_COLOR color) {
	rect->color = color;
}

int getFontLineHeight() {
	static int store = 0;
	return store ? store : (store = al_get_font_line_height(Font));
}

void drawRectangle(GBUTTON* rect) {
	if (rect->imgPointer) {
		int h = al_get_bitmap_width(*(rect->imgPointer));
		al_draw_scaled_bitmap(*(rect->imgPointer), 0, 0, h, h, rect->right ? rect->x + rect->width + 5: rect->x - 65, rect->y + 5, 55, 55, 0);
	}
	al_draw_rectangle(rect->x, rect->y, rect->x + rect->width, rect->y + rect->height, rect->color, 2);
	al_draw_text(Font, rect->color, rect->x + rect->width / 2, rect->y + rect->height / 2 - getFontLineHeight() / 2, ALLEGRO_ALIGN_CENTER, rect->text);
}

char inRectangle(GBUTTON* rect, int x, int y) {
	const float right = rect->x + rect->width, bottom = rect->y + rect->height;
	return x < right && y < bottom && x > rect->x && y > rect->y;
}

void resetRectangleColor(GBUTTON* rect) {
	setRectangleColor(rect, BLACK);
}

int menuClickHandler(GBUTTON* btns,ALLEGRO_MOUSE_EVENT mouseE,int rectCount,ALLEGRO_DISPLAY* Display) {
	for (int i = 0; i < rectCount; i++)
		if (!i && inRectangle(btns + i, mouseE.x, mouseE.y)) {
			al_destroy_display(Display);
			al_destroy_font(Font);
			for (int i = 0; i < 4; i++)
				al_destroy_bitmap(icons[i]);
			al_rest(.5);
			return 1;
		}
		else if (i == 1 && inRectangle(btns + i, mouseE.x, mouseE.y)) {
			gameState temp;
			if (loadDecryptedGameState("./save/game.dat", Display, &temp,ENCRYPTION_KEY)) {
				enableGameState(&temp);
				al_destroy_display(Display);
				al_destroy_font(Font);
				for (int i = 0; i < 4; i++)
					al_destroy_bitmap(icons[i]);
				al_rest(.5);
			}
			return 3;
		}
		else if (i == 2 && inRectangle(btns + i, mouseE.x, mouseE.y)) {
			isAboutShows = true;
			return 0;
		}
		else if (i == 3 && inRectangle(btns + i, mouseE.x, mouseE.y)) {
			al_destroy_display(Display);
			al_destroy_font(Font);
			al_destroy_bitmap(img);
			for (int i = 0; i < 4; i++)
				al_destroy_bitmap(icons[i]);
			return 2;
		}
}

void hoverHandler(GBUTTON* btns, ALLEGRO_MOUSE_EVENT mouseE, int rectCount) {
	if (!isAboutShows)
		for (int i = 0; i < rectCount; i++) {
			if (inRectangle(btns + i, mouseE.x, mouseE.y)) {
				setRectangleColor(btns + i, COLOR2);
				break;
			}
			else
				resetRectangleColor(btns + i);
		}
	else {
		if (inRectangle(&btnPrevious, mouseE.x, mouseE.y))
			setRectangleColor(&btnPrevious, COLOR2);
		else
			resetRectangleColor(&btnPrevious);
		if (inRectangle(&btnNext, mouseE.x, mouseE.y))
			setRectangleColor(&btnNext, COLOR2);
		else
			resetRectangleColor(&btnNext);
	}
}

void setNextPreviousBtns(){
	GBUTTON mybtn;
	mybtn.x = 15, mybtn.y = START_HEIGHT - 55;
	mybtn.height = 40, mybtn.width = (START_WIDTH - 30) / 2;
	mybtn.right = 1;
	mybtn.color = BLACK;
	mybtn.imgPointer = NULL;
	strcpy(mybtn.text, "PREVIOUS");
	btnPrevious = mybtn;
	mybtn.x = START_WIDTH / 2;
	strcpy(mybtn.text, "NEXT");
	btnNext = mybtn;
}