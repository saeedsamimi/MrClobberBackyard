#pragma once
#include "allegro5/allegro.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "../map.h"
#include "../test.h"
#include "startWin.h"
#include "../save/fileManager.h"
#include "../constants.h"

extern void __drawScaledPhoto(ALLEGRO_BITMAP*, float, float, float);

#define FINISH_WIDTH 665
#define FINISH_HEIGHT 500
#define FINISH_MSPACER 10
#define FINISH_TOP 200

ALLEGRO_FONT* HeadFont;
ALLEGRO_BITMAP* crown;
char running = 1;

char cmp(CAT*,CAT*);

void qSort(CAT* A, int start, int end) {
	if (end > start) {
		int i = start, j = end - 1;
		CAT temp;
		while (j >= i)
			if (cmp(A + end,A+i)) {
				temp = A[i];
				A[i--] = A[j];
				A[j--] = temp;
			}
			else
				i++;
		temp = A[end];
		A[end] = A[i];
		A[i] = temp;
		qSort(A, start, i - 1);
		qSort(A, i + 1, end);
	}
}

void FinishWinPaint(ALLEGRO_FONT* FONT) {
	const float tableX = 15;
	const float tableY = 130;
	const float tableIH = 55;
	const float tableIW[5] = { 135,100,100,100,100 };
	const float verticalGap = 25;
	const float horizontalGap = 15;
	const ALLEGRO_COLOR topColor = al_map_rgb(164, 164, 164);
	const ALLEGRO_COLOR tableColor = al_map_rgb(217, 217, 217);
	// print top of the table
	float x = tableX;
	int fontHeight = al_get_font_line_height(FONT);
	for (int i = 0; i < 5; i++) {
		al_draw_filled_rounded_rectangle(x, tableY, x + tableIW[i],tableY+ tableIH, 20, 20, topColor);
		x += tableIW[i] + verticalGap;
	}
	x = tableX + tableIW[0] + verticalGap;
	for (int i = 1; i < 5; i++) {
		al_draw_text(FONT, BLACK, x + tableIW[i] / 2, tableY + (tableIH - fontHeight) / 2, ALLEGRO_ALIGN_CENTER, TITLES[i-1]);
		x += tableIW[i] + verticalGap;
	}
	float y = tableY + tableIH + horizontalGap;
	// endprint the top of the table
	// print players table
	for (int i = 0; i < CAT_COUNT; i++) {
		x = tableX;
		// print player identities
		al_draw_filled_rounded_rectangle(x, y, x + tableIW[0], y + tableIH, 20, 20, tableColor);
		al_draw_textf(FONT,
			cats[i].color,
			x + tableIW[0] / 2,
			y + (tableIH - fontHeight) / 2,
			ALLEGRO_ALIGN_CENTER,
			"#%d",i+1);
		// endprint players identities
		// print players name
		x += tableIW[0] + verticalGap;
		al_draw_filled_rounded_rectangle(x, y, x + tableIW[1], y + tableIH, 20, 20, tableColor);
		al_draw_textf(FONT,
			BLACK,
			x + tableIW[1] / 2,
			y + (tableIH - fontHeight) / 2,
			ALLEGRO_ALIGN_CENTER,
			"%d", i+1);
		// endprint players name
		// print players points 1- defencePoint
		x += tableIW[1] + verticalGap;
		al_draw_filled_rounded_rectangle(x, y, x + tableIW[2], y + tableIH, 20, 20, tableColor);
		al_draw_textf(FONT,
			BLACK,
			x + tableIW[2] / 2,
			y + (tableIH - fontHeight) / 2,
			ALLEGRO_ALIGN_CENTER,
			"%d", cats[i].defencePoint);
		// print players point 2- attackPoint
		x += tableIW[2] + verticalGap;
		al_draw_filled_rounded_rectangle(x, y, x + tableIW[3], y + tableIH, 20, 20, tableColor);
		al_draw_textf(FONT,
			BLACK,
			x + tableIW[3] / 2,
			y + (tableIH - fontHeight) / 2,
			ALLEGRO_ALIGN_CENTER,
			"%d", cats[i].attackPoint);
		// print players point 3- mousePoint
		x += tableIW[3] + verticalGap;
		al_draw_filled_rounded_rectangle(x, y, x + tableIW[4], y + tableIH, 20, 20, tableColor);
		al_draw_textf(FONT,
			BLACK,
			x + tableIW[4] / 2,
			y + (tableIH - fontHeight) / 2,
			ALLEGRO_ALIGN_CENTER,
			"%d", cats[i].mousePoint);
		y += tableIH + horizontalGap;
	}
	__drawScaledPhoto(crown, 15, 15, 100);
	__drawScaledPhoto(img, 550, 15, 100);
	al_draw_text(HeadFont, BLACK, 150, 47, 0, "Game Result: ");
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

void eventHandler(ALLEGRO_DISPLAY* buff,CAT Players[CAT_COUNT]) {
	ALLEGRO_EVENT_QUEUE* ev_queue = al_create_event_queue();
	ALLEGRO_EVENT ev;
	int dlgResult;
	al_register_event_source(ev_queue, al_get_display_event_source(buff));
	while (running) {
		al_wait_for_event(ev_queue, &ev);
		switch (ev.type) {
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			dlgResult = al_show_native_message_box(
				buff,
				"exit? ",
				"Are you sure to exit? ",
				"if you click \"YES\" the state of the game will be saved!\nif you click \"NO\" the game won't be saved.",
				NULL,
				ALLEGRO_MESSAGEBOX_YES_NO);
			if (dlgResult == 1) {
				FILE* file = createFile("./save/state.txt",buff);
				int saveResult;
				showError(buff, "Save state", (saveResult = writeOnFile(file)) ? "Yes saved" : "File cannot be saved! please try again.");
				if (saveResult) {
					al_destroy_display(buff);
					al_destroy_font(HeadFont);
					al_destroy_bitmap(crown);
					running = 0;
				}
			}
			else if(dlgResult == 2) {
				al_destroy_display(buff);
				al_destroy_font(HeadFont);
				al_destroy_bitmap(crown);
				running = 0;
			}
			break;
		}
	}
}

void runFinishWin(ALLEGRO_FONT *FONT,ALLEGRO_DISPLAY *buff,CAT Players[CAT_COUNT]) {
	qSort(Players, 0, CAT_COUNT - 1);
	buff = al_create_display(FINISH_WIDTH, FINISH_HEIGHT);
	crown = al_load_bitmap("src/crown.png");
	img = al_load_bitmap("src/pic.png");
	al_set_display_icon(buff, img);
	FONT = al_load_ttf_font("src/NotoSerif-Medium.ttf", 20, 0);
	HeadFont = al_load_ttf_font("src/NotoSerif-Medium.ttf", 35, 0);
	if (!HeadFont) {
		showNotFoundErr(buff, "font", "src/NotoSerif-Medium.ttf");
		showError(buff, "the font cannot load", "but the content of the game was saved in the local storage!");
		exit(1);
	}
	al_clear_to_color(WHITE);
	FinishWinPaint(FONT);
	eventHandler(buff,Players);
	exit(0);
}