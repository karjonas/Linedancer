#pragma once

#include "allegro5/allegro.h"

#include <vector>

enum class Shape
{
	RECTANGLE,
	TRIANGLE
};

enum class Direction
{
	LEFT,
	RIGHT
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
	static ALLEGRO_COLOR color_bg() { return al_map_rgb(33,33,32);}
	static ALLEGRO_COLOR color0() { return al_map_rgb(240,241,238);}
	static ALLEGRO_COLOR color1() { return al_map_rgb(19,82,162);}
	static ALLEGRO_COLOR color2() { return al_map_rgb(255,212,100);}
	static ALLEGRO_COLOR color3() { return al_map_rgb(251,105,100);}
};

struct Opponent
{
	double speed;
	double time;
	Direction direction;
	Shape shape;
	bool active;
	double x;
};

struct Point
{
	Point(int x_in, int y_in) : x(x_in), y(y_in) {}

	int x;
	int y;
};

std::vector<Point> calc_user_rectangle_points(int x, int y, bool flip, int rect_size);
std::vector<Point> calc_user_triangle_points(int x, int y, bool flip, int rect_size);