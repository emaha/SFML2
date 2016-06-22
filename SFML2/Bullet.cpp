#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

using namespace std;

Bullet::Bullet()
{

}

Bullet::Bullet(int id, Vector2f position, Vector2f velocity) 
{
	init(id, position, velocity);
}


Bullet::~Bullet()
{

}


void Bullet::init(int id, Vector2f position, Vector2f velocity)
{
	this->position = position;
	this->velocity = velocity;
	this->id = id;

	sprite.setTextureRect(IntRect(0, 0, 20, 20));

	_isAlive = true;

	line[0] = Vertex(position - Player::getInstance()->viewportOffset);
	line[1] = Vertex(position - Player::getInstance()->viewportOffset - velocity*5.0f);


	shape.setRadius(3);
	shape.setFillColor(Color::Green);
	shape.setPosition(position);
	speed = 2000.0f;

	lifeTime = 1.5f;
}

void Bullet::update(float time)
{
	if (isAlive())
	{
		position += velocity * time * speed;
		line[0] = Vertex(position - Player::getInstance()->viewportOffset);
		line[1] = Vertex(position - Player::getInstance()->viewportOffset +  velocity*speed / 20.0f);

		shape.setPosition(position);
		sprite.setPosition(position);
		if (lifeTime < 0.0f)
		{
			kill();
			//cout << "Bullet kill" << endl;
		}
		lifeTime -= time;
	}
}

void Bullet::draw(RenderTarget &target)
{
	
	if (isAlive())
	{
		//target.draw(shape);
		target.draw(line, 2, Lines);
	}
}