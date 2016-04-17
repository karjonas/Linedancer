#include "GlobalState.h"

#include <algorithm>
#include <vector>
#include <cstdlib>     /* srand, rand */

std::vector<Point> calc_user_rectangle_points(double x, double y, bool flip, int rect_size)
{
  int rect_half = rect_size/2;

  int sign = flip ? 1 : -1;

  Point p0{x - rect_half, y };
  Point p1{x - rect_half, y + sign*rect_size };
  Point p2{x + rect_half, y + sign*rect_size };
  Point p3{x + rect_half, y };

  return {p0, p1, p2, p3};
}

std::vector<Point> calc_user_triangle_points(double x, double y, bool flip, int rect_size)
{
  double rect_half = rect_size*0.5;

  double sign = flip ? 1.0 : -1.0;

  Point p0{x - rect_half, y };
  Point p1{x, y + sign*rect_size };
  Point p2{x + rect_half, y };
  
  return {p0, p1, p2};
}

std::vector<Point> calc_user_tall_rectangle_points(double x, double y, bool flip, int rect_size)
{
  double rect_half = rect_size*0.5;
  double rect_4th = rect_size*0.25;

  int sign = flip ? 1.0 : -1.0;

  Point p0{x - rect_4th,  y };
  Point p1{x - rect_4th,  y + sign*rect_size*2};
  Point p2{x + rect_4th,  y + sign*rect_size*2};
  Point p3{x + rect_4th,  y };
  
  return {p0, p1, p2, p3};
}

std::vector<Point> calc_shape_points(int x, int y, bool flip, int rect_size, Shape shape)
{
    switch (shape)
    {
        case Shape::RECTANGLE:
            return calc_user_rectangle_points(x,y,flip,rect_size);
            break;
        case Shape::TRIANGLE:
            return calc_user_triangle_points(x,y,flip,rect_size);
            break;
        case Shape::TALL_RECTANGLE:
            return calc_user_tall_rectangle_points(x,y,flip,rect_size);
            break;
        default:
            return {};
    }
}


std::vector<Opponent> generate_opponents(size_t num_opponents, int level)
{
    srand(level + 2);
    std::vector<Shape> allowed_shapes;
    if (level == 1)
    {
       allowed_shapes.push_back(Shape::RECTANGLE);
       allowed_shapes.push_back(Shape::TRIANGLE);
    }
    else
    {
       allowed_shapes.push_back(Shape::RECTANGLE);
       allowed_shapes.push_back(Shape::TALL_RECTANGLE);
       allowed_shapes.push_back(Shape::TRIANGLE);
    }
    const size_t n_shapes = allowed_shapes.size();
    
    std::vector<Opponent> opponents;
    
    Direction prev_dir = Direction::LEFT;
    Shape prev_shape = Shape::RECTANGLE;
    bool prev_shifter = false;
    int time_glob = 0;

    const int speed = level >= 3 ? 200 : 100;
    const int shift_rate = level >= 3 ? 4 : 2;
    
    int orders[6][4] = {{0,1,2,3},{0,2,1,3},{0,1,2,3}, {1,0,2,3}, {1,0,3,2}, {1,3,0,2}};
    
    bool last_skipped = false;
    
    for (int i = 0; i < (num_opponents/4); i++)
    {
        time_glob += 4;
        
        std::vector<std::pair<int, Direction>> side_ops;
        side_ops.push_back(std::make_pair(time_glob/2, Direction::LEFT));
        side_ops.push_back(std::make_pair(time_glob/2, Direction::RIGHT));
        side_ops.push_back(std::make_pair(time_glob/2 + 1, Direction::LEFT));
        side_ops.push_back(std::make_pair(time_glob/2 + 1, Direction::RIGHT));
        
        const int idx = rand() % 4;
        
        for (int j = 0; j < 4; j++)
        {
            const bool skip = (rand() % 4) == 0;
            if (skip && !last_skipped)
            {
                last_skipped = true;
                continue;
            }
            auto p = side_ops[orders[idx][j]];
            const int time = p.first;
            const Direction dir = p.second;
            
            Shape shape = allowed_shapes[rand() % n_shapes];
            bool shape_shift = ((rand() % shift_rate) == 0) && !prev_shifter;

            Opponent o;

            if (shape_shift)
            {
                if (shape == prev_shape)
                  shape = allowed_shapes[rand() % n_shapes];
                o = create_opponent(time, speed, dir, shape, true); // Morph into next shape
            }
            else
            {
                o = create_opponent(time, speed, dir, prev_shape, false);
            }

            prev_dir = o.direction;
            prev_shape = o.shape;
            prev_shifter = o.is_shapeshifter;

            opponents.push_back(o);      
        }
    }
    
    //std::reverse(opponents.begin(),opponents.end());
    return opponents;
}

LevelData create_level(size_t num_opponents, int level_idx, bool is_first)
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
        level.opponents = generate_opponents(num_opponents, level_idx);
    }
            
    return level;
}

Opponent create_opponent(double time, double speed, Direction direction, Shape shape, bool is_shapeshifter)
{
    Opponent o;
    o.speed = speed;
    o.active = false;
    o.direction = direction;
    o.rect_size = 20;
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