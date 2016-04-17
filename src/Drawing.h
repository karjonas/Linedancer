#pragma once

#include <vector>

struct User;
struct Opponent;
struct ALLEGRO_FONT;
struct ALLEGRO_COLOR;

namespace Drawing
{
	void draw_user(int w, int h, User& user);
	void draw_opponent(int w, int h, Opponent& opponent);
	void draw_tutorial_texts(ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y, double time);
	void draw_score_texts(ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y, int score);
	void draw_level_texts(ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y, int level, double time);		
	void draw_credits(ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y);		
	void draw_all(int w, int h, User& user, const std::vector<Opponent>& opponents);
};