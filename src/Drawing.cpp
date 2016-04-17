#include "Drawing.h"
#include "GlobalState.h"

#include "allegro5/allegro.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <vector>
#include <algorithm>

struct ShapePoints
{
    std::vector<Point> points;
    ALLEGRO_COLOR color;
    int mid_x;
};

struct LineColor
{
    LineColor(Point p0_in, Point p1_in, ALLEGRO_COLOR color_in)
            : p0(p0_in), p1(p1_in), color(color_in) {}
    Point p0;
    Point p1;
    ALLEGRO_COLOR color;
};

void Drawing::draw_user(int w, int h, User& user)
{
  std::vector<Point> points;
  if (user.user_shape == Shape::TRIANGLE)
    points = calc_user_triangle_points(user.user_x, h/2, false, user.rect_size);
  else
    points = calc_user_rectangle_points(user.user_x, h/2, false, user.rect_size);

  points.insert(points.begin(), Point(100,h/2));
  points.push_back(Point(w-100,h/2));
  
  const size_t num_pts = points.size();
  
  for (int i = 0; i < num_pts - 1; i++)
  {
    auto fst = points[i];
    auto snd = points[i+1];
    al_draw_line(fst.x, fst.y, snd.x, snd.y, ColorScheme::color0(), 1.0f);
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
    al_draw_line(fst.x, fst.y, snd.x, snd.y, opponent.is_shapeshifter ? ColorScheme::color0() : ColorScheme::color1(), 1.0f);
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

void Drawing::draw_level_texts(ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y, int level, double time)
{
    if (time < 3)
        al_draw_textf(font, color, x, y, ALLEGRO_ALIGN_CENTRE, "Level %d", level);
}

void Drawing::draw_credits(ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y)
{
    al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTRE, "Thanks for playing!");
}

void Drawing::draw_all(int w, int h, User& user, const std::vector<Opponent>& opponents)
{
    std::vector<ShapePoints> sps;
    
    {
        std::vector<Point> points =
            calc_shape_points(user.user_x, h/2, false, user.rect_size, user.user_shape);

        ShapePoints p;
        p.color = ColorScheme::color0();
        p.mid_x = user.user_x;
        p.points = points;
        
        sps.push_back(p);
    }
    
    
    for (auto& opponent : opponents)
    {
        if (!opponent.active)
            continue;
        std::vector<Point> points =
            calc_shape_points(opponent.x, h/2, false, opponent.rect_size, opponent.shape);
        
        ShapePoints p;
        p.color = opponent.is_shapeshifter ? ColorScheme::color0() : ColorScheme::color1();
        p.mid_x = opponent.x;
        p.points = points;
        
        sps.push_back(p);
    }
    
    std::sort(sps.begin(), sps.end(), [](const auto& sp0,const auto& sp1)
    {
        return sp0.mid_x < sp1.mid_x;
    });

    std::vector<LineColor> lcs;
    auto last_pt = Point(100,h/2);

    for (auto& sp : sps)
    {
        lcs.emplace_back(last_pt, sp.points[0], ColorScheme::color0());
        
        const int nums = static_cast<int>(sp.points.size()) - 1;
        for (int i = 0; i < nums; i++)
        {
            lcs.emplace_back(sp.points[i], sp.points[i+1], sp.color);
        }
        
        last_pt = sp.points.back();
    }

    lcs.emplace_back(last_pt, Point(w-100, h/2), ColorScheme::color0());

    for (auto& lc : lcs)
    {
        auto fst = lc.p0;
        auto snd = lc.p1;
        al_draw_line(fst.x, fst.y, snd.x, snd.y, lc.color, 1.0f);
    }
    
}
