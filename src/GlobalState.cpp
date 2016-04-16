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