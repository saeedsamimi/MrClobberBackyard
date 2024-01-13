#pragma once
#include "allegro5/allegro.h"
#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_image.h"
#include "../constants.h"

const char fileNames[][16] = { "src/game.png","src/history.png","src/info.png","src/quit.png" };

ALLEGRO_FONT* Font;
ALLEGRO_BITMAP* img, *icons[4];
const char titles[][15] = { "START NEW GAME","RESUME GAME","ABOUT","QUIT" };
#define START_HEIGHT 650
#define START_WIDTH 500

typedef struct {
	float x, y;
	float width, height;
	char text[15], right;
	ALLEGRO_BITMAP** imgPointer;
	ALLEGRO_COLOR color;
} GBUTTON;

int getFontLineHeight();
void drawRectangle(GBUTTON*);
void setBtn(GBUTTON*, float, float, float, float,const char[15],ALLEGRO_BITMAP**);
char inRectangle(GBUTTON*, int, int);
void paint(GBUTTON*, int);
void resetRectangleColor(GBUTTON*);
void setRectangleColor(GBUTTON*, ALLEGRO_COLOR);

int runStartWin() {
	al_init();
	al_set_new_display_refresh_rate(60);
	al_set_app_name("Mr. Clobber BackYard");
	ALLEGRO_DISPLAY* Display = al_create_display(START_WIDTH, START_HEIGHT);
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

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
			default:
				printf("KEY %d DOWN\n", e.keyboard.keycode);
			}
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			for (int i = 0; i < rectCount; i++)
				if (!i && inRectangle(rects + i, e.mouse.x, e.mouse.y)) {
					al_destroy_display(Display);
					al_destroy_font(Font);
					for (int i = 0; i < 4; i++)
						al_destroy_bitmap(icons[i]);
					al_rest(.5);
					return 1;
				}
				else if (i == 3 && inRectangle(rects + i, e.mouse.x, e.mouse.y)) {
					al_destroy_display(Display);
					al_destroy_font(Font);
					al_destroy_bitmap(img);
					for (int i = 0; i < 4; i++)
						al_destroy_bitmap(icons[i]);
					return 2;
				}
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			for (int i = 0; i < rectCount; i++) {
				if (inRectangle(rects + i, e.mouse.x, e.mouse.y)) {
					setRectangleColor(rects + i, COLOR2);
					break;
				}
				else
					resetRectangleColor(rects + i);
			}
			break;
		}
		paint(rects, rectCount);
	}
}

void paint(GBUTTON* rects, int n) {
	al_clear_to_color(COLOR4);
	const int W = al_get_bitmap_width(img);
	al_draw_scaled_bitmap(img, 0, 0, W, W, START_WIDTH / 4, 50, START_WIDTH / 2, START_WIDTH / 2, 0);
	al_draw_rectangle(0, 0, START_WIDTH, START_HEIGHT, COLOR1, 20);
	const GBUTTON* end = rects + n;
	while (rects < end)
		drawRectangle(rects++);
	al_flip_display();
}

void setBtn(GBUTTON* rect, float offx, float offy, float width, float height,const char name[15],ALLEGRO_BITMAP** bmpPtr) {
	rect->x = (START_WIDTH - width) / 2 + offx;
	rect->y = START_WIDTH / 3 + offy + 170;
	rect->width = width;
	rect->height = height;
	rect->imgPointer = bmpPtr;
	strcpy(rect->text, name);
	rect->color = al_map_rgb(0, 0, 0);
}

void setRectangleColor(GBUTTON* rect, ALLEGRO_COLOR color) {
	rect->color = color;
}

int getFontLineHeight() {
	static int store = 0;
	return store ? store : (store = al_get_font_line_height(Font));
}

void drawRectangle(GBUTTON* rect) {
	int h = al_get_bitmap_width(*(rect->imgPointer));
	al_draw_scaled_bitmap(*(rect->imgPointer), 0, 0, h, h, rect->right ? rect->x + rect->width + 5: rect->x - 65, rect->y + 5, 55, 55, 0);
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