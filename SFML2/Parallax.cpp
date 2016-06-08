#include "Parallax.h"
#include <iostream>

using namespace std;

Parallax::Parallax()
{
	cout << "ctr" << endl;
	if (window.isOpen())
		window.close();

	window.create(sf::VideoMode(1024, 768), "Parallax", sf::Style::Default);
	window.setKeyRepeatEnabled(true);
	window.setFramerateLimit(60);

	cir.setRadius(40);
	cir.setOutlineThickness(1);
	cir.setFillColor(Color::Transparent);
	cir2.setRadius(40);
	cir2.setOutlineThickness(1);
	cir2.setFillColor(Color::Transparent);
	
}


Parallax::~Parallax()
{
}


void Parallax::run()
{
	
	while (window.isOpen())
	{
		window.clear(Color::Black);
		update();
		draw();
		window.display();
	}

}

void Parallax::update()
{
	if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }
	if (Keyboard::isKeyPressed(Keyboard::A)) { dist+=0.2; }
	if (Keyboard::isKeyPressed(Keyboard::Z)) { dist-=0.2; }
	cir.setPosition(Vector2f(500 - dist, 300));
	cir2.setPosition(Vector2f(500 + dist, 300));
}

void Parallax::draw()
{
	int step = 100;

	for (int x = -1000; x < 2000; x += step){
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(x, 0)),
			sf::Vertex(sf::Vector2f(x+768, 768))
		};

		
		window.draw(line, 2, sf::Lines);
	}
	
	for (int x = -1000; x < 2000; x += step){
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(x, 0)),
			sf::Vertex(sf::Vector2f(x - 768, 768))
		};

		window.draw(line, 2, sf::Lines);

	}

	window.draw(cir);
	window.draw(cir2);

	

}