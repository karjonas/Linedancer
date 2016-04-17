#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <algorithm>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include "GlobalState.h"
#include "Drawing.h"

bool done;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_TIMER* timer;
ALLEGRO_DISPLAY* display;
ALLEGRO_FONT* font;

void abort_game(const char* message)
{
    printf("%s \n", message);
    exit(1);
}
 
void init(void)
{
    if (!al_init())
        abort_game("Failed to initialize allegro");
 
    if (!al_install_keyboard())
        abort_game("Failed to install keyboard");
    if (!al_init_primitives_addon())
        abort_game("Failed to install primitives");
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();// initialize the ttf (True Type Font) addon 

    if(!al_install_audio()){
        abort_game("failed to initialize audio!\n");
    }
 
    if(!al_init_acodec_addon()){
       abort_game("failed to initialize audio codecs!\n");
    }
 
    if (!al_reserve_samples(1)){
        abort_game("failed to reserve samples!\n");
    }
 
    timer = al_create_timer(1.0 / 60);
    if (!timer)
        abort_game("Failed to create timer");
 
    ALLEGRO_DISPLAY_MODE disp_data;
    al_get_display_mode(0, &disp_data);
   
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    //display = al_create_display(disp_data.width, disp_data.height);
    display = al_create_display(800, 480);
    if (!display)
        abort_game("Failed to create display");
 
    event_queue = al_create_event_queue();
    if (!event_queue)
        abort_game("Failed to create event queue");
    
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    
    font = al_load_ttf_font("src/Atari_Full.ttf", 16,0 );

    done = false;
}
 
void shutdown(void)
{
    if (timer)
        al_destroy_timer(timer);
    
    if (display)
        al_destroy_display(display);
 
    if (event_queue)
        al_destroy_event_queue(event_queue);

    al_shutdown_primitives_addon();
}
 
void game_loop(void)
{
    bool redraw = true;
    al_start_timer(timer);

    int windowWidth = al_get_display_width(display);
    int windowHeight = al_get_display_height(display);
        
    User user{};
    user.user_x = windowWidth/2;
    int level_nr = 4;

    LevelData level(create_level(60, level_nr, false));
    
    ALLEGRO_KEYBOARD_STATE kbd_state;
    
    double start_time = 0;
    double time_last = 0;
    double time_curr = 0;
    double elapsed_time = 0;
    double elapsed_time_unadjusted = 0;

    bool death = false;
    int finished_level = 5;
    int num_kills = 0;  
    while (!done)
    {        
        auto& opponents = level.opponents;
        
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            //done = main_loop.tick();
            redraw = true;
            time_curr = al_get_time();
            double dt = time_curr - time_last;
            dt = std::min(dt, 0.2);
            elapsed_time_unadjusted += dt;
            elapsed_time = elapsed_time_unadjusted - start_time;
            
            for (auto& opponent : opponents)
            {
                if (!opponent.active && elapsed_time >= opponent.time)
                {
                    opponent.active = true;
                    opponent.x = opponent.direction == Direction::LEFT ? windowWidth - 100 : 100;                    
                }
                else if (opponent.active)
                {
                    int sign = opponent.direction == Direction::LEFT ? -1 : 1;
                    opponent.x += sign*opponent.speed*dt;
                }
            }
            
            for (int i = 0; i < opponents.size(); i++)
            {
                auto& opponent = opponents[i];
                if (opponent.active && overlaps(user.user_x, opponent.x, user.rect_size))
                {
                    user.user_shape = opponent.is_shapeshifter ? opponent.shape : user.user_shape;
                    if (opponent.shape != user.user_shape)
                    {
                        death = true;
                    }
                    else
                    {
                        num_kills++;
                    }
                    opponents.erase(opponents.begin() + i);
                    i--;                    
                }
            }
            
            al_get_keyboard_state(&kbd_state);
            bool left = al_key_down(&kbd_state,ALLEGRO_KEY_LEFT) || al_key_down(&kbd_state, ALLEGRO_KEY_A);
            bool right = al_key_down(&kbd_state,ALLEGRO_KEY_RIGHT) || al_key_down(&kbd_state, ALLEGRO_KEY_D);

            if (left && !right)
            {
                user.user_x = user.user_x - user.user_speed*dt;
            }
            else if (!left && right)
            {
                user.user_x = user.user_x + user.user_speed*dt;
            }
            
            user.user_x = std::max(100.0 + 2*user.rect_size, user.user_x);
            user.user_x = std::min(static_cast<double>(windowWidth) - 100.0 -(2*user.rect_size), user.user_x);            
            
            time_last = time_curr;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                done = true;
            }
            {
                //main_loop.key_pressed(event.keyboard.keycode);
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
          done = true;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(ColorScheme::color_bg());
            
            Drawing::draw_all(windowWidth, windowHeight, user, opponents);

            if (level.first_level)
            {
                Drawing::draw_tutorial_texts(font, ColorScheme::color1(), windowWidth/2 , windowHeight/5, elapsed_time);
            }
            else if (level_nr != finished_level)
            {
                Drawing::draw_level_texts(font, ColorScheme::color1(), windowWidth/2, windowHeight/5, level_nr, elapsed_time);
            }
            else
            {
                Drawing::draw_credits(font, ColorScheme::color1(), windowWidth/2, windowHeight/5);
            }
            al_flip_display();
        }
        
        if (death)
        {
            if (level_nr == 1)
                level = create_level(60, level_nr, false);
            else
                level = create_level(60, level_nr, false);

            user = User{};
            user.user_x = windowWidth/2;
            start_time = al_get_time();
            death = false;
            num_kills = 0;            
        }
        else if (opponents.empty() && finished_level != level_nr)
        {
            level_nr++;
            level = create_level(60, level_nr, false);
            user = User{};
            user.user_x = windowWidth/2;
            start_time = al_get_time();
            death = false;
            num_kills = 0;      
        }
    }
}
 
int main(int argc, char* argv[])
{
    init();
    game_loop();
    shutdown();
}