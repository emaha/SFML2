#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Parallax
{
public:
	RenderWindow window;
	float dist=50;
	CircleShape cir,cir2;

	Parallax();
	~Parallax();
	void update();
	void draw();
	void run();
};

