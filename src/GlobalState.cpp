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
  Point p1{x, y + sign*rect_size };
  Point p2{x + rect_half, y };
  
  return {p0, p1, p2};
}

LevelData create_level(size_t num_opponents, int seed, bool is_first)
{
    LevelData level;
    if (is_first)
    {
        level.first_level = is_first;
        level.opponents.push_back(tri_opponent_left(12, true));
        level.opponents.push_back(tri_opponent_left(20, false));
        level.opponents.push_back(rect_opponent_left(31, false));
    }
    else
    {
        level.opponents.push_back(tri_opponent_left(2, true));
        level.opponents.push_back(tri_opponent_left(3, false));
        level.opponents.push_back(rect_opponent_left(5, false));
        level.opponents.push_back(tri_opponent_left(6, true));
        level.opponents.push_back(tri_opponent_left(7, false));
        level.opponents.push_back(rect_opponent_left(8, false));
        level.opponents.push_back(tri_opponent_left(10, true));
        level.opponents.push_back(tri_opponent_left(13, false));
        level.opponents.push_back(rect_opponent_left(15, false));
        level.opponents.push_back(tri_opponent_left(17, true));
        level.opponents.push_back(tri_opponent_left(23, false));
    }
            
    return level;
}

Opponent create_opponent(double time, double speed, Direction direction, Shape shape, bool is_shapeshifter)
{
    Opponent o;
    o.speed = speed;
    o.active = false;
    o.direction = direction;
    o.rect_size = 40;
    o.time = time;
    o.shape = shape;
    o.x = 0;
    o.is_shapeshifter = is_shapeshifter;
    
    return o;
}

Opponent rect_opponent_right(double time, bool is_shapeshifter)
{
    return create_opponent(time,200,Direction::RIGHT,Shape::RECTANGLE, is_shapeshifter);
}

Opponent rect_opponent_left(double time, bool is_shapeshifter)
{
    return create_opponent(time,200,Direction::LEFT,Shape::RECTANGLE, is_shapeshifter);
}

Opponent tri_opponent_right(double time, bool is_shapeshifter)
{
    return create_opponent(time,200,Direction::RIGHT,Shape::TRIANGLE, is_shapeshifter);
}

Opponent tri_opponent_left(double time, bool is_shapeshifter)
{
    return create_opponent(time,200,Direction::LEFT,Shape::TRIANGLE, is_shapeshifter);
}

bool overlaps(int v0, int v1, int width)
{
  int v0min = v0 - width/2;
  int v0max = v0 + width/2;
  int v1min = v1 - width/2;
  int v1max = v1 + width/2;

  return (v0min <= v1max and v1min <= v0max);
}

ALLEGRO_COLOR ColorScheme::color_bg() { return al_map_rgb(0x50,0x50,0x50);}
ALLEGRO_COLOR ColorScheme::color0() { return al_map_rgb(0x9b,0xd7,0xd5);}
ALLEGRO_COLOR ColorScheme::color1() { return al_map_rgb(0xff,0x72,0x60);}
ALLEGRO_COLOR ColorScheme::color2() { return al_map_rgb(19,82,162);}
ALLEGRO_COLOR ColorScheme::color3() { return al_map_rgb(255,212,100);}

//  #129793 #505050 #FFF5C3 #9BD7D5 #FF7260