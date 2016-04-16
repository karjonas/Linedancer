#pragma once

struct User;
struct Opponent;

namespace Drawing
{
	void draw_user(int w, int h, User& user);
	void draw_opponent(int w, int h, Opponent& opponent);
};