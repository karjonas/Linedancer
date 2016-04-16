#include "Drawing.h"
#include "GlobalState.h"

#include "allegro5/allegro.h"
#include <allegro5/allegro_primitives.h>

#include <vector>

void Drawing::draw_user(int w, int h, User& user)
{
  std::vector<Point> points;
  if (user.user_shape == Shape::TRIANGLE)
    points = calc_user_triangle_points(user.user_x, h/2, false, user.rect_size);
  else
    points = calc_user_rectangle_points(user.user_x, h/2, false, user.rect_size);

  points.insert(points.begin(), Point(0,h/2));
  points.push_back(Point(w,h/2));
  
  const size_t num_pts = points.size();
  
  for (int i = 0; i < num_pts - 1; i++)
  {
    auto fst = points[i];
    auto snd = points[i+1];
    al_draw_line(fst.x, fst.y, snd.x, snd.y, ColorScheme::color1(), 1.0f);
  }
}

void Drawing::draw_opponent(int /* w */, int h, Opponent& opponent)
{
  std::vector<Point> points;
  if (opponent.shape == Shape::TRIANGLE)
    points = calc_user_triangle_points(opponent.x, h/2, false, opponent.rect_size);
  else
    points = calc_user_rectangle_points(opponent.x, h/2, false, opponent.rect_size);
  
  const size_t num_pts = points.size();
  
  for (int i = 0; i < num_pts - 1; i++)
  {
    auto fst = points[i];
    auto snd = points[i+1];
    al_draw_line(fst.x, fst.y, snd.x, snd.y, ColorScheme::color2(), 1.0f);
  }
}
 