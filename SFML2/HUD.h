#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class HUD
{
public:
	HUD();
	~HUD();

	void update(float time);
	void draw(RenderTarget &target);

private:
	Text fps;
	Text healthText;
	Font font;
};

