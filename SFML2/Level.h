#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Level
{
public:
	Level();
	~Level();

	void load();
	void draw(RenderTarget &target);

private:
	Texture backgroundTexture, floor;
	Sprite backgroundSprite;
	float offsetX;
};

