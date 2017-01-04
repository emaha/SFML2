#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class HUD
{
public:
	HUD();
	~HUD();

	void update(float time);
	void draw(RenderTarget &target);
	void drawText(RenderTarget &target, string text, int x, int y);

private:
	Text fps;
	Text healthText;
	Font font;
};

