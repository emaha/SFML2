#include "HUD.h"
#include <string>
#include <iostream>
#include "Player.h"


HUD::HUD()
{
	font.loadFromFile("../Data/Fonts/arial.ttf");

	fps.setFont(font);
	fps.setCharacterSize(10);
	fps.setFillColor(Color::Red);

	healthText.setFont(font);
	healthText.setCharacterSize(20);
	healthText.setFillColor(Color::Red);
	healthText.setPosition(50, 50);

}


HUD::~HUD()
{
}


void HUD::update(float time)
{
	fps.setString("FPS: " + std::to_string(1000.0f / time));
	healthText.setString("HP: " + std::to_string(Player::getInstance()->health) + "\t ID: " + std::to_string(Player::getInstance()->id));
}

void HUD::drawText(RenderTarget &target, string text, int x, int y)
{
	Text t;
	t.setFont(font);
	t.setCharacterSize(15);
	t.setFillColor(Color::Green);
	t.setPosition(x, y);
	t.setString(text);
	target.draw(t);
}

void HUD::draw(RenderTarget &target)
{
	target.draw(fps);
	target.draw(healthText);

	//drawText(target, "ASDASDASD", 200, 200);
}
