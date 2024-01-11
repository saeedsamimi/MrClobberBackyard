#pragma once
#include "allegro5/allegro.h"
#include "../map.h"
#include "startWin.h"

#define FINISH_WIDTH 1000
#define FINISH_HEIGHT 700
#define FINISH_MSPACER 10
#define FINISH_TOP 200

char cmp(CAT*,CAT*);

void qSort(CAT* A, int start, int end) {
	if (end > start) {
		int i = start, j = end - 1;
		CAT temp;
		while (j >= i) {
			if (cmp(A + end,A+i)) {
				temp = A[i];
				A[i--] = A[j];
				A[j--] = temp;
			}
			else
				i++;
		}
		temp = A[end];
		A[end] = A[i];
		A[i] = temp;
		qSort(A, start, i - 1);
		qSort(A, i + 1, end);
	}
}

void FinishWinPaint(ALLEGRO_FONT* FONT) {
	const float tableIH = (FINISH_HEIGHT - FINISH_MSPACER - FINISH_TOP) / 5;
	const float tableIW = (FINISH_WIDTH - 2 * FINISH_MSPACER) / 4;
	al_draw_filled_rectangle(FINISH_MSPACER, FINISH_TOP, FINISH_WIDTH - FINISH_MSPACER, FINISH_HEIGHT - FINISH_MSPACER, COLOR3);
	float x, y = FINISH_TOP;
	const float textOffset = (tableIH - al_get_font_line_height(FONT)) / 2;
	for (int i = 0; i < 4; i++) {
		x = i * tableIW + FINISH_MSPACER;
		al_draw_rectangle(x, y, x + tableIW, y + tableIH, BLACK, 2.);
		al_draw_text(FONT, BLACK, x + tableIW / 2, y + textOffset, ALLEGRO_ALIGN_CENTER, TITLES[i]);
	}
	for (int i = 0; i < CAT_COUNT; i++) {
		y += tableIH;
		for (int j = 0; j < 4; j++) {
			x = j * tableIW + FINISH_MSPACER;
			al_draw_rectangle(x, y, x + tableIW, y + tableIH, BLACK, 2.);
		}
		x = FINISH_MSPACER + tableIW / 2;
		// j = 0
		al_draw_textf(FONT, BLACK, x, y + textOffset, ALLEGRO_ALIGN_CENTER, "#%d", i);
		// j = 1
		x += tableIW;
		al_draw_textf(FONT, BLACK, x, y + textOffset, ALLEGRO_ALIGN_CENTER, "%d", cats[i].defencePoint);
		// j = 2
		x += tableIW;
		al_draw_textf(FONT, BLACK, x, y + textOffset, ALLEGRO_ALIGN_CENTER, "%d", cats[i].attackPoint);
		// j = 3
		x += tableIW;
		al_draw_textf(FONT, BLACK, x, y + textOffset, ALLEGRO_ALIGN_CENTER, "%d", cats[i].mousePoint);
	}
	al_destroy_font(FONT);
	FONT = al_load_ttf_font("src/NotoSerif-Medium.ttf", 60, 0);
	al_draw_text(FONT, BLACK, FINISH_MSPACER, (FINISH_TOP - al_get_font_line_height(FONT))/2, 0, "The game's result: ");
	int W = al_get_bitmap_width(img);
	const float dw = FINISH_TOP - 2 * FINISH_MSPACER;
	al_draw_scaled_bitmap(img, 0, 0, W, W, FINISH_WIDTH - FINISH_MSPACER - dw, FINISH_MSPACER, dw, dw, 0);
	al_flip_display();
}

// return c1 > c2
char cmp(CAT *c1, CAT *c2) {
	if (c1->mousePoint > c2->mousePoint)
		return 1;
	else if(c1->mousePoint == c2->mousePoint)
		if (c1->attackPoint > c2->attackPoint)
			return 1;
		else if(c1->attackPoint == c2->attackPoint)
			if (c1->defencePoint > c2->defencePoint)
				return 1;
	return 0;
}

void runFinishWin(ALLEGRO_FONT *FONT,ALLEGRO_DISPLAY *buff,CAT Players[CAT_COUNT]) {
	qSort(Players, 0, CAT_COUNT - 1);
	buff = al_create_display(FINISH_WIDTH, FINISH_HEIGHT);
	al_set_display_icon(buff, img);
	al_clear_to_color(WHITE);
	FinishWinPaint(FONT);
	al_rest(10);
	exit(0);
}