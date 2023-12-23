#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

int main1() {
  // Initialize Allegro
  if (!al_init()) {
    fprintf(stderr, "Failed to initialize Allegro!\n");
    return -1;
  }

  // Initialize the font addon
  al_init_font_addon();

  // Initialize the TTF (TrueType Font) addon
  if (!al_init_ttf_addon()) {
    fprintf(stderr, "Failed to initialize TTF addon!\n");
    return -1;
  }

  // Create a display
  ALLEGRO_DISPLAY* display = al_create_display(800, 600);
  if (!display) {
    fprintf(stderr, "Failed to create display!\n");
    return -1;
  }

  // Load a font
  ALLEGRO_FONT* font = al_load_ttf_font("NotoSerif-Medium.ttf", 24, 0);
  if (!font) {
    fprintf(stderr, "Failed to load font!\n");
    return -1;
  }

  // Set the background color
  al_clear_to_color(al_map_rgb(255, 255, 255));

  // Draw text on the display
  al_draw_text(font, al_map_rgb(0, 0, 0), 400, 300, ALLEGRO_ALIGN_CENTER, "Hello, Allegro!");

  // Flip the display to show the drawing
  al_flip_display();
  system("pause");
  // Wait for a few seconds
  al_rest(5.0);

  // Clean up resources
  al_destroy_font(font);
  al_destroy_display(display);

  return 0;
}
