#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Entity
{
public:
	Entity();
	~Entity();

	virtual void init();
	virtual void update(float time);
	virtual void draw(RenderTarget &target);
	bool isAlive(){ return _isAlive; }
	void kill(){ _isAlive = false; }



	int id;
	Sprite sprite;
	Vector2f position, velocity;
	float health;
	bool _isAlive;
};

