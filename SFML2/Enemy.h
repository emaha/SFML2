#pragma once
#include "Entity.h"
#include <string>

using namespace std;

class Enemy : public Entity
{
public:
	Enemy();
	Enemy(int id, Vector2f pos, Vector2f dir);
	~Enemy();

	void init();
	void update(float time);
	void draw(RenderTarget &target);
	void setPostion(Vector2f pos);
	void setDirection(Vector2f dir);
	void setVelocity(Vector2f vel);
	void setHealth(float hp);
	void setDamage(int damage);

	int id;
	string name;
	
	Sprite baseSprite;
	Texture baseTexture;


	Vector2f position, velocity, lerpPosition, baseSize, towerSize, cannonSize;
	float health;
	Color color;
	Vector2f damage;
	RectangleShape baseShape, towerShape, cannonShape, healthBar;
	Vector2f direction;
};
