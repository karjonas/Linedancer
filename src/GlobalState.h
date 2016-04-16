#pragma once

#include "allegro5/allegro.h"

#include <vector>

enum class Shape
{
	RECTANGLE,
	TRIANGLE
};

struct User
{
	double user_x = 300.0;
	double user_speed = 500.0; // Pixels per second
	bool move_left = false;
	bool move_right = false;
	size_t rect_size = 40;
	Shape user_shape = Shape::RECTANGLE;
};

struct ColorScheme
{
	ALLEGRO_COLOR color_bg() { return al_map_rgb(33,33,32);}
	ALLEGRO_COLOR color0() { return al_map_rgb(240,241,238);}
	ALLEGRO_COLOR color1() { return al_map_rgb(19,82,162);}
	ALLEGRO_COLOR color2() { return al_map_rgb(255,212,100);}
	ALLEGRO_COLOR color3() { return al_map_rgb(251,105,100);}
};