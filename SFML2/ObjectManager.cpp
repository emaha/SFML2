#include "ObjectManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "Constants.h"
#include <SFML/Network.hpp>

ObjectManager* ObjectManager::pInstance = 0;

ObjectManager::ObjectManager(){}
ObjectManager::~ObjectManager(){}

void ObjectManager::addEnemy()
{

	enemyList.push_back(new Enemy());

}

void ObjectManager::addBullet(int id, Vector2f position, Vector2f velocity)
{

	entityList.push_back(new Bullet(id, position, velocity));

}

void ObjectManager::editEnemy(int id, Vector2f pos, Vector2f vel, Vector2f dir)
{
	// TODO: переделать это с использованием std::map
	for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		if ((*it)->id == id)
		{
			(*it)->setPostion(pos);
			(*it)->setVelocity(vel);
			(*it)->setDirection(dir);
			return;
		}
	}

	enemyList.push_back(new Enemy(id, pos, dir));
}


void ObjectManager::editEnemy(int id, Vector2f pos, Vector2f vel, Vector2f dir, float hp)
{

	for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		
		if (id == PLAYER_ID)
		{
			Player::getInstance()->health = hp;
		}
		if ((*it)->id == id)
		{
			(*it)->setPostion(pos);
			(*it)->setVelocity(vel);
			(*it)->setDirection(dir);
			(*it)->setHealth(hp);
			return;
		}
	}

	enemyList.push_back(new Enemy(id, pos, dir));

}

void ObjectManager::killAll()
{

}

void ObjectManager::update(float time)
{
	//cout << "Object Manager Update" << endl;

	for (vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		if ((*it)->isAlive()){
			(*it)->update(time);
		}
	}

	for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		if ((*it)->isAlive()){
			(*it)->update(time);
		}
	}

	checkCollision();

	//cout << "Object Manager Update Exit" << endl;
}

void ObjectManager::draw(RenderTarget &target)
{

	for (vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		if ((*it)->isAlive()){
			(*it)->draw(target);
		}
	}

	for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		if ((*it)->id!=PLAYER_ID){
			(*it)->draw(target);
		}
	}

}

void ObjectManager::killEntity(int id)
{

	for (vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		if ((*it)->id == id)
		{
			(*it)->kill();
		}
	}
}

bool ObjectManager::isCollide(Sprite a, Sprite b)
{
	/*
	if (a.getGlobalBounds().intersects(b.getGlobalBounds()))
	{
	return true;
	}
	*/
	if (getDistance(a.getPosition(), b.getPosition()) < 150)
	{
		return true;
	}
		
	return false;
}

void ObjectManager::checkCollision()
{
	for (vector<Enemy*>::iterator enemy_it = enemyList.begin(); enemy_it != enemyList.end(); ++enemy_it)
	{
		for (vector<Entity*>::iterator bullet_it = entityList.begin(); bullet_it != entityList.end(); ++bullet_it)
		{
			if ((*enemy_it)->isAlive() && (*bullet_it)->isAlive() && (*enemy_it)->id != (*bullet_it)->id){

				if (isCollide((*enemy_it)->baseSprite, (*bullet_it)->sprite))
				{
					if (isServer)
					{
						(*enemy_it)->setDamage(30);
					}
					(*bullet_it)->kill();
					cout << (*bullet_it)->id << " ---> " << (*enemy_it)->id << endl;
				}
			}
		}

	}
}

void ObjectManager::reSpawnPlayer()
{
	
}

float ObjectManager::getDistance(Vector2f a, Vector2f b)
{
	return abs(sqrtf(pow((b.x - a.x), 2) - pow((b.y - a.x), 2)));
}
