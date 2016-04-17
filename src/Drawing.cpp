#include "Drawing.h"
#include "GlobalState.h"

#include "allegro5/allegro.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

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
    al_draw_line(fst.x, fst.y, snd.x, snd.y, ColorScheme::color0(), 3.0f);
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
    al_draw_line(fst.x, fst.y, snd.x, snd.y, opponent.is_shapeshifter ? ColorScheme::color0() : ColorScheme::color1(), 3.0f);
  }
}

void Drawing::draw_tutorial_texts(ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y, double time)
{
    if (time < 2.0 && time < 3.5)
      al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTRE, "Welcome!");
    else if (time > 4.0 && time < 9.5)
      al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTRE, "Move using arrow keys.");
    else if (time > 10.0 && time < 16.5)
    {
      al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTRE, "Collide with a same color shape");
      al_draw_text(font, color, x, y + 30, ALLEGRO_ALIGN_CENTRE, "to morph into it.");
    }
    else if (time > 17.0 && time < 23.5)
    {
      al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTRE, "Collide with same shape to kill it.");
    }
    else if (time > 24.0 && time < 30)
    {
      al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTRE, "Collide with another shape and ");
      al_draw_text(font, color, x, y + 30, ALLEGRO_ALIGN_CENTRE, "color and it kills you!");
    }
}

void Drawing::draw_score_texts(ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y, int score)
{
    al_draw_textf(font, color, x, y, ALLEGRO_ALIGN_CENTRE, "Score: %d", score);
}
