#include "Enemy.h"
#include <iostream>


Enemy::Enemy()
{
	init();
}

void Enemy::setPostion(Vector2f pos)
{
	//lerpPosition = pos;
	//velocity = (lerpPosition - position);
	position = pos;
}

void Enemy::setDirection(Vector2f dir)
{
	direction = dir;
}

void Enemy::setVelocity(Vector2f vel)
{
	//velocity = vel;
}

void Enemy::setDamage(int damage)
{
	health -= damage;
}

void Enemy::setHealth(float hp)
{
	health = hp;
}


Enemy::Enemy(int id, Vector2f pos, Vector2f dir)
{
	this->id = id;
	this->position = pos;

	_isAlive = true;
	position = Vector2f(100, 150);
	velocity = Vector2f(0, 0);
	
	baseSize = Vector2f(40, 60);
	towerSize = Vector2f(50, 30);
	cannonSize = Vector2f(60, 5);

	damage = Vector2f(50, 100);
	health = 100;
	direction = dir;   //base, tower

	baseTexture.loadFromFile("../Data/Images/tank.png", IntRect(0, 0, 50, 75));
	baseSprite.setTexture(baseTexture);
	baseSprite.setOrigin(Vector2f(25, 37));

	baseShape.setFillColor(Color(100, 150, 250));
	baseShape.setSize(baseSize);
	baseShape.setOrigin(baseSize / 2.0f);

	towerShape.setFillColor(Color(80, 120, 220));
	towerShape.setSize(towerSize);
	towerShape.setOrigin(towerSize / 2.0f);

	cannonShape.setFillColor(Color(80, 120, 220));
	cannonShape.setSize(cannonSize);
	cannonShape.setOrigin(Vector2f(cannonSize.x / 4.0f, cannonSize.y / 2.0f));
	
	healthBar.setFillColor(Color::Green);
	healthBar.setSize(Vector2f(100,10));
}

Enemy::~Enemy()
{
}

void Enemy::update(float time)
{
	/*
	Vector2f vector = lerpPosition - position;
	float lenght = sqrt(vector.x * vector.x + vector.y * vector.y);
	Vector2f normVelocity(vector.x / lenght, vector.y / lenght);

	velocity = vector;

	if (abs(getDistance(lerpPosition, position) < 30))
	{
		position = lerpPosition; 
	}
	else
	{
		position += velocity * time * 15.0f;  //moving...
	}
	*/
	baseSprite.setPosition(position);
	baseSprite.setRotation(direction.x);

	baseShape.setRotation(direction.x);
	baseShape.setPosition(position);

	towerShape.setRotation(direction.y);
	towerShape.setPosition(position);

	cannonShape.setRotation(direction.y);
	cannonShape.setPosition(position);

	healthBar.setPosition(position - Vector2f(50, 50));
	healthBar.setSize(Vector2f(health <= 0 ? 0 : health, 10));	

	if (health < 70)
		healthBar.setFillColor(Color::Yellow);
	if (health < 35)
		healthBar.setFillColor(Color::Red);
	if (health <= 0)
		kill();
}

void Enemy::init()
{
	health = 100;
	position = Vector2f(rand() % 1000, rand() % 1000);
}

void Enemy::draw(RenderTarget &target)
{
	target.draw(baseShape);
	//target.draw(baseSprite);
	target.draw(towerShape);
	target.draw(cannonShape);
	target.draw(healthBar);
}

