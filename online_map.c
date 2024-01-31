#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "networking/client.h"


#define WIDTH 500
#define HEIGHT 330
#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 60
#define FONT_PATH "./src/NotoSerif-Medium.ttf"
#define FONT_SIZE 24
#define BUTTON_COLOR al_map_rgb(50,50,50)

int buttons[3][2]; // Three buttons , 0 => x 1 => y for each button
const char *username;
const char *password;
const char *server;
const char *map_path;

int whichButtonClicked(int mouse_x,int mouse_y) {
    for(int i=0;i<3;i++) {
        if(buttons[i][0] <= mouse_x && (mouse_x <= (buttons[i][0]+BUTTON_WIDTH))) {
            if(buttons[i][1] <= mouse_y && (mouse_y <= (buttons[i][1]+BUTTON_HEIGHT))) {
                return i;
            }
        }
    }

    return -1;
}

void draw_buttons(ALLEGRO_FONT *font) {
    int start_x = 100;
    int start_y = 50;
    int margin = 30;
    int last_x = start_x+BUTTON_WIDTH;
    int last_y = start_y + BUTTON_HEIGHT;
    // UPLOAD Button
    al_draw_filled_rectangle(start_x,start_y,last_x, last_y,BUTTON_COLOR);
    al_draw_text(font,al_map_rgb(255,255,255),start_x+BUTTON_WIDTH/2,start_y+10,ALLEGRO_ALIGN_CENTER,"UPLOAD MAP");
    buttons[0][0] = start_x;
    buttons[0][1] = start_y;
    //Download Button
    start_y = last_y + margin;
    last_y = start_y + BUTTON_HEIGHT;
    al_draw_filled_rectangle(start_x,start_y,last_x,last_y,BUTTON_COLOR);
    al_draw_text(font,al_map_rgb(255,255,255),start_x+BUTTON_WIDTH/2,start_y+10,ALLEGRO_ALIGN_CENTER,"DOWNLOAD MAP");
    buttons[1][0] = start_x;
    buttons[1][1] = start_y;
    // Exit Button
    start_y = last_y + margin;
    last_y = start_y + BUTTON_HEIGHT;
    al_draw_filled_rectangle(start_x,start_y,last_x, last_y,BUTTON_COLOR);
    al_draw_text(font,al_map_rgb(255,255,255),start_x+BUTTON_WIDTH/2,start_y+10,ALLEGRO_ALIGN_CENTER,"EXIT");
    buttons[2][0] = start_x;
    buttons[2][1] = start_y;
}

void readCreds() {
    ALLEGRO_CONFIG *config = al_load_config_file("networking/profile.conf");
    username = al_get_config_value(config,"","username");
    password = al_get_config_value(config,"","password");
    server = al_get_config_value(config,"","server");
    map_path = al_get_config_value(config,"","map_path");
    printf("[+] Read config file!\n");
}


int main() {
    if(!al_init() || !al_init_primitives_addon() || !al_init_font_addon() || !al_init_ttf_addon()){
        exit(EXIT_FAILURE);
    }

    //Install Mouse & Keyboard
    al_install_keyboard();
    al_install_mouse();


    ALLEGRO_DISPLAY *display = al_create_display(WIDTH,HEIGHT);
    ALLEGRO_EVENT_QUEUE *EVQ = al_create_event_queue();
    ALLEGRO_EVENT ev;
    ALLEGRO_FONT *font = NULL;
    font = al_load_ttf_font(FONT_PATH,FONT_SIZE,0);

    // Register Sources
    al_register_event_source(EVQ,al_get_display_event_source(display));
    al_register_event_source(EVQ,al_get_keyboard_event_source());
    al_register_event_source(EVQ,al_get_mouse_event_source());

    al_clear_to_color(al_map_rgb(255,255,255));

    draw_buttons(font);
    al_flip_display();
    readCreds();
    while(1) {
        
        al_wait_for_event(EVQ,&ev);
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            switch (whichButtonClicked(ev.mouse.x,ev.mouse.y))
            {
            case 0:
                upload_file(username,password,server,map_path);
                break;
            case 1:
                download_map(server,username,password,map_path);
                break;
            case 2:
                exit(EXIT_SUCCESS);
                break;
            
            }
        }
        
    }
    al_destroy_display(display);
    al_destroy_event_queue(EVQ);
    al_destroy_font(font);
    return 0;
}