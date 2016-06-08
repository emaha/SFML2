#include "Entity.h"
#include <iostream>

Entity::Entity()
{
	init();
}


Entity::~Entity()
{
}

void Entity::init()
{
	_isAlive = true;
}

void Entity::update(float time)
{

}

void Entity::draw(RenderTarget &target)
{

}