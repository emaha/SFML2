#include "ObjectManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

ObjectManager* ObjectManager::pInstance = 0;



ObjectManager::ObjectManager(){}
ObjectManager::~ObjectManager(){}

//Стреляем не создавая лишних объектов
void ObjectManager::addBullet(int id, Vector2f position, Vector2f velocity)
{
	Lock lock(mutex);
	
	for (vector<Bullet*>::iterator it = bulletList.begin(); it != bulletList.end(); ++it)
	{
		if (!(*it)->isAlive()){
			(*it)->init(id, position, velocity);
			return;
		}
	}
	
	bulletList.push_back(new Bullet(id, position, velocity));
}

//обновление данных о игроке для сервера
void ObjectManager::editEnemy(int id, Vector2f pos, Vector2f vel, Vector2f dir)
{
	Lock lock(mutex);

	auto iter = enemyMap.find(id); 
	if (iter != enemyMap.end())
	{
		Enemy* enemy = iter->second;
		enemy->setPostion(pos);
		enemy->setVelocity(vel);
		enemy->setDirection(dir);
	}
	else
	{
		enemyMap[id] = new Enemy(id, pos, dir);// .insert(pair<int, Enemy*>(id, new Enemy(id, pos, dir)));
	}
}

//обновление данных о игроке для клиента
void ObjectManager::editEnemy(int id, Vector2f pos, Vector2f vel, Vector2f dir, float hp)
{
	Lock lock(mutex);

	auto iter = enemyMap.find(id);
	if (iter != enemyMap.end())
	{
		Enemy* enemy = iter->second;

		if (enemy->id == PLAYER_ID)
		{
			Player::getInstance()->health = hp;
		}
		enemy->setPostion(pos);
		enemy->setVelocity(vel);
		enemy->setDirection(dir);
		enemy->setHealth(hp);

	}
	else
	{
		enemyMap.insert(pair<int, Enemy*>(id, new Enemy(id, pos, dir)));
	}
}

void ObjectManager::useSkill(int id, int skillNum)
{
		
}

void ObjectManager::update(float time)
{
	Lock lock(mutex);

	for (vector<Bullet*>::iterator it = bulletList.begin(); it != bulletList.end(); ++it)
	{
		(*it)->update(time);
	}

	for (map<int, Enemy*>::iterator it = enemyMap.begin(); it != enemyMap.end(); ++it)
	{
		it->second->update(time);
	}
	checkCollision();
}

void ObjectManager::draw(RenderTarget &target)
{
	Lock lock(mutex);
	for (vector<Bullet*>::iterator it = bulletList.begin(); it != bulletList.end(); ++it)
	{
		if ((*it)->isAlive()){
			(*it)->draw(target);
		}
	}

	for (map<int, Enemy*>::iterator it = enemyMap.begin(); it != enemyMap.end(); ++it)
	{
		if (it->second->id!=PLAYER_ID)
		{
			it->second->draw(target);
		}
	}
}

bool ObjectManager::isCollide(Vector2f a, Vector2f b)
{
	if (getDistance(a, b) < 35.0f)
	{
		return true;
	}
		
	return false;
}

void ObjectManager::checkCollision()
{
	//Столкновения игрока с пулей
	for (map<int, Enemy*>::iterator enemy_it = enemyMap.begin(); enemy_it != enemyMap.end(); ++ enemy_it)
	{
		for (vector<Bullet*>::iterator bullet_it = bulletList.begin(); bullet_it != bulletList.end(); ++bullet_it)
		{
			if (enemy_it->second->isAlive() && (*bullet_it)->isAlive() && enemy_it->second->id != (*bullet_it)->id)
			{
				if (isCollide(enemy_it->second->position, (*bullet_it)->position))
				{
					if (isServer)
					{
						enemy_it->second->setDamage(rand() % 20 + 20);   // 20-40
					}
					(*bullet_it)->kill();
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
