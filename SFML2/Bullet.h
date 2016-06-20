#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

class Bullet : public Entity
{
public:
	Bullet();
	Bullet(int id, Vector2f position, Vector2f velocity);
	~Bullet();

	void init(int id, Vector2f position, Vector2f velocity);
	void update(float time);
	void draw(RenderTarget &target);

	CircleShape shape;
	Vertex line[2];
	float speed;
	float lifeTime;

};

