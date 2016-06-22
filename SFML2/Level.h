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
	void update(float time);

private:
	Texture backgroundTexture, floor;
	Sprite backgroundSprite;
	float offsetX;
};

