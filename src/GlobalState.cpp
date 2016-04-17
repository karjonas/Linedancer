#include "GlobalState.h"

#include <algorithm>
#include <vector>
#include <cstdlib>     /* srand, rand */

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

std::vector<Opponent> generate_opponents(size_t num_opponents, int seed)
{
    srand(seed + 2);
    
    std::vector<Opponent> opponents;
    
    Direction prev_dir = Direction::LEFT;
    Shape prev_shape = Shape::RECTANGLE;
    bool prev_shifter = false;
    int time_glob = 0;
    for (int i = 0; i < num_opponents; i++)
    {
        time_glob++;
        
        bool skip = (rand() % 2) == 1;
        if (skip)
        {
            time_glob++;
        }
        int time = time_glob/2;
        
        Direction dir = prev_dir == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
        Shape shape = (rand() % 2) == 1 ? Shape::RECTANGLE : Shape::TRIANGLE;
        bool shape_shift = (rand() % 5) == 1;

        Opponent o;
        
        if (shape_shift)
        {
            shape = prev_shape == Shape::RECTANGLE ? Shape::TRIANGLE : Shape::RECTANGLE;
            o = create_opponent(time, 200, dir, shape, true); // Morph into next shape
        }
        else
        {
            o = create_opponent(time, 200, dir, prev_shape, false);
        }
        
        prev_dir = o.direction;
        prev_shape = o.shape;
        prev_shifter = o.is_shapeshifter;
        
        opponents.push_back(o);      
    }
    
    //std::reverse(opponents.begin(),opponents.end());
    return opponents;
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
        level.opponents = generate_opponents(num_opponents, seed);
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