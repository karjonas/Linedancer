#include "GlobalState.h"

#include <vector>

std::vector<Point> calc_user_rectangle_points(int x, int y, bool flip, int rect_size)
{
  int rect_half = rect_size/2;

  int sign = flip ? 1 : -1;

  Point p0{x - rect_half, y };
  Point p1{x - rect_half, y + sign*rect_size };
  Point p2{x + rect_half, y + sign*rect_size };
  Point p3{x + rect_half, y };
  
  return {p0, p1, p2, p3};
}

std::vector<Point> calc_user_triangle_points(int x, int y, bool flip, int rect_size)
{
  int rect_half = rect_size/2;

  int sign = flip ? 1 : -1;

  Point p0{x - rect_half, y };
  Point p1{x, y + sign*rect_half };
  Point p2{x + rect_half, y };
  
  return {p0, p1, p2};
}

Opponent create_opponent(double time, double speed, Direction direction, Shape shape)
{
    Opponent o;
    o.speed = speed;
    o.active = false;
    o.direction = direction;
    o.rect_size = 40;
    o.time = time;
    o.shape = shape;
    o.x = 0;
    
    return o;
}

Opponent rect_opponent_right(double time)
{
    return create_opponent(time,100,Direction::RIGHT,Shape::RECTANGLE);
}

Opponent rect_opponent_left(double time)
{
    return create_opponent(time,100,Direction::LEFT,Shape::RECTANGLE);
}

Opponent tri_opponent_right(double time)
{
    return create_opponent(time,100,Direction::RIGHT,Shape::TRIANGLE);
}

Opponent tri_opponent_left(double time)
{
    return create_opponent(time,100,Direction::LEFT,Shape::TRIANGLE);
}