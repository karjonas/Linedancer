#pragma once

#include "allegro5/allegro.h"

#include <vector>

enum class Shape
{
	RECTANGLE,
	TRIANGLE,
	TALL_RECTANGLE
};

enum class Direction
{
	LEFT,
	RIGHT
};

struct User
{
	double user_x = 300.0;
	double user_speed = 300.0; // Pixels per second
	bool move_left = false;
	bool move_right = false;
	size_t rect_size = 20;
	Shape user_shape = Shape::RECTANGLE;
};

namespace ColorScheme
{
	ALLEGRO_COLOR color_bg();
	ALLEGRO_COLOR color0();
	ALLEGRO_COLOR color1();
	ALLEGRO_COLOR color2();
	ALLEGRO_COLOR color3();
};

struct Opponent
{
	double speed;
	double time;
	Direction direction;
	Shape shape;
	bool active;
	double x;
	double rect_size;
	bool is_shapeshifter;
};

struct Point
{
	Point(double x_in, double y_in) : x(x_in), y(y_in) {}

	double x;
	double y;
};

struct LevelData
{
	std::vector<Opponent> opponents;
	bool first_level = false;
};

LevelData create_level(size_t num_opponents, int seed, bool is_first);

std::vector<Point> calc_user_rectangle_points(double x, double y, bool flip, int rect_size);
std::vector<Point> calc_user_triangle_points(double x, double y, bool flip, int rect_size);
std::vector<Point> calc_user_tall_rectangle_points(double x, double y, bool flip, int rect_size);
std::vector<Point> calc_shape_points(int x, int y, bool flip, int rect_size, Shape shape);

Opponent create_opponent(double time, double speed, Direction direction, Shape shape, bool is_shapeshifter);
Opponent rect_opponent_right(double time, bool is_shapeshifter);
Opponent rect_opponent_left(double time, bool is_shapeshifter);
Opponent tri_opponent_right(double time, bool is_shapeshifter);
Opponent tri_opponent_left(double time, bool is_shapeshifter);

bool overlaps(int v0, int v1, int width);

