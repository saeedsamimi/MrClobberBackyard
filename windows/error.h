#pragma once
#include <stdio.h>
#include "allegro5/allegro_native_dialog.h"

void showError(ALLEGRO_DISPLAY* display, const char* head, const char* txt) {
  al_show_native_message_box(
    display,
    "Error",
    head,
    txt,
    NULL,
    ALLEGRO_MESSAGEBOX_ERROR
  );
}

void showNotFoundErr(ALLEGRO_DISPLAY* display, const char* type, const char* fileName) {
  char temp[100];
  sprintf(temp, "%s %s cannot open. may be deleted or corrupted!", type, fileName);
  showError(display, "The file cannot opened!", temp);
}