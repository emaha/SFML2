#include "Level.h"
#include "Player.h"

Level::Level()
{
	backgroundTexture.loadFromFile("../Data/Images/back03.jpg");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setPosition(Vector2f(0, 150));
	backgroundSprite.setScale(Vector2f(1.5, 1.5));
}


Level::~Level()
{
}


void Level::load()
{
}

void Level::draw(RenderTarget &target)
{
	target.draw(backgroundSprite);
}

void Level::update(float time)
{
	backgroundSprite.setPosition(Vector2f(0, 150) - Player::getInstance()->viewportOffset);
}
