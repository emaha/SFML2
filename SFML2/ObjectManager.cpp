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
	//enemyList.push_back(new Enemy());
}

void ObjectManager::addBullet(int id, Vector2f position, Vector2f velocity)
{
	Lock lock(mutex);
	entityList.push_back(new Bullet(id, position, velocity));
}

void ObjectManager::editEnemy(int id, Vector2f pos, Vector2f vel, Vector2f dir)
{
	// TODO: переделать это с использованием std::map
	//Lock lock(mutex);
	
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
	//Lock lock(mutex);

	Lock lock(mutex);
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
	//Lock lock(mutex);
	enemyList.push_back(new Enemy(id, pos, dir));
}


void ObjectManager::killAll()
{

}

void ObjectManager::update(float time)
{
	//cout << "Object Manager Update" << endl;
	Lock lock(mutex);
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
	Lock lock(mutex);
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
	/*
	for (vector<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		if ((*it)->id == id)
		{
			(*it)->kill();
		}
	}
	*/
}

bool ObjectManager::isCollide(Vector2f a, Vector2f b)
{
	if (getDistance(a, b) < 35.0f)
	{
		cout << a.x << ", " << a.y << " --- " << b.x << ", " << b.y << "\tDIST: " << getDistance(a, b) << endl;
		return true;
	}
		
	return false;
}

void ObjectManager::checkCollision()
{
	//cout << "CheckCollisions" << endl;
	for (vector<Enemy*>::iterator enemy_it = enemyList.begin(); enemy_it != enemyList.end(); ++enemy_it)
	{
		for (vector<Entity*>::iterator bullet_it = entityList.begin(); bullet_it != entityList.end(); ++bullet_it)
		{
			//cout << "Testing: " << (*enemy_it)->id << " : " << (*bullet_it)->id << endl;

			if ((*enemy_it)->isAlive() && (*bullet_it)->isAlive() && (*enemy_it)->id != (*bullet_it)->id)
			{
				if (isCollide((*enemy_it)->position, (*bullet_it)->position))
				{
					if (isServer)
					{
						(*enemy_it)->setDamage(rand()%20+20);   // 20-40
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
	return sqrtf(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
}
