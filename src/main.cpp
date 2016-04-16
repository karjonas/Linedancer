#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
 
/* Our window. */
ALLEGRO_DISPLAY *display;
ALLEGRO_FONT *font;

/* Simple FPS counter. */
int fps, fps_accum;
double fps_time;
 
int main(void) {
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    bool redraw = true;
 
    srand(time(NULL));
 
    /* Init Allegro 5 + addons. */
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_install_keyboard();
 
    /* Create our window. */
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(640, 480);
    al_set_window_title(display, "LD35");
 
    font = al_load_font("fixed_font.tga", 0, 0);
 
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
 
    timer = al_create_timer(1.0 / 60);
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
 
    while (1) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
 
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                break;
        }
        if (event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
        if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            al_acknowledge_resize(display);
            redraw = true;
        }
 
        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;
            
            al_flip_display();
            fps_accum++;
            if (t - fps_time >= 1) {
                fps = fps_accum;
                fps_accum = 0;
                fps_time = t;
            }
        }
    }
    return 0;
}
