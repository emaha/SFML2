#include "Enemy.h"
#include <iostream>
#include "Player.h"
#include "Game.h"


Enemy::Enemy()
{
	init();
}

void Enemy::setPostion(Vector2f pos)
{
	lerpPosition = pos;
	//position = pos;
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
	float takenDamage = damage - Armor;

	health -= takenDamage < 0.0f ? 0.0f : takenDamage;   //что-бы не хилить)))
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
	respawnTimer = 3000;
	state = Alive;

	position = Vector2f(100, 150);
	velocity = Vector2f(0, 0);
	
	baseSize = Vector2f(60, 60);
	towerSize = Vector2f(50, 30);
	cannonSize = Vector2f(60, 5);

	damage = Vector2f(50, 100);
	baseHealth = health = 1000;
	baseArmor = Armor = 0.0f;
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

float getDistance(Vector2f a, Vector2f b)
{
	return abs(sqrtf(pow((b.x - a.x), 2) + pow((b.y - a.y), 2)));
}


Vector2f Lerp(Vector2f v0, Vector2f v1, float t)
{
	return (1 - t)*v0 + t*v1;
}

void Enemy::update(float time)
{
	Vector2f vector = lerpPosition - position;
	float lenght = sqrt(vector.x * vector.x + vector.y * vector.y);
	Vector2f normVelocity(vector.x / lenght, vector.y / lenght);

	position = Lerp(position, lerpPosition, 0.15f);

	_isAlive = health > 0.0f;


	baseSprite.setPosition(position - Player::getInstance()->viewportOffset);
	baseSprite.setRotation(direction.x);

	baseShape.setRotation(direction.x);
	baseShape.setPosition(position - Player::getInstance()->viewportOffset);

	towerShape.setRotation(direction.y);
	towerShape.setPosition(position - Player::getInstance()->viewportOffset);

	cannonShape.setRotation(direction.y);
	cannonShape.setPosition(position - Player::getInstance()->viewportOffset);

	healthBar.setPosition(position - Vector2f(50, 50) - Player::getInstance()->viewportOffset);
	healthBar.setSize(Vector2f(health <= 0 ? 0 : health / baseHealth * 100, 10));

	if (health >= 700)
		healthBar.setFillColor(Color::Green);
	if (health < 700)
		healthBar.setFillColor(Color::Yellow);
	if (health < 350)
		healthBar.setFillColor(Color::Red);
	if (health <= 0)
	{
		cout << respawnTimer << endl;
		respawnTimer -= time;
		if (respawnTimer < 0.0f)
		{

			respawn();
		}
		return;
	}

	updateBuff(time);
}

void Enemy::updateBuff(float time)
{
	
	for (vector<Buff*>::iterator it = buffList.begin(); it != buffList.end();)
	{
		if ((*it)->isActive)
		{
			(*it)->update(time);
			if ((*it)->isReady())
			{
				if ((*it)->buffType == BuffType::Health)
				{
					if (Game::isServer)
					{
						cout << "Buff tick: " << (*it)->buffType << endl;
						health += (*it)->amount;
						if (health > baseHealth) health = baseHealth;
					}
				}

				if ((*it)->buffType == BuffType::Armor)
				{
					Armor = baseArmor + (*it)->amount;
					cout << "Buff tick: " << (*it)->buffType << "\tBase = " << baseArmor << "\tArmor = " << Armor << endl;
				}



			}
			++it;
		}
		else // если баф уже не активен, удаляем.
		{
			if ((*it)->buffType == BuffType::Armor)
			{
				Armor = baseArmor;
			}

			delete *it;
			it = buffList.erase(it);
		}
	}
}


void Enemy::respawn()
{
	//cout << id << " " << "respawn" << endl;
	respawnTimer = 3000;
	health = 1000;
	state = Spawn;
	_isAlive = true;
}


void Enemy::init()
{
	health = 1000;
	position = Vector2f(rand() % 1000, rand() % 1000);
}

void Enemy::draw(RenderTarget &target)
{
	target.draw(baseShape);
	//target.draw(baseSprite);
	target.draw(towerShape);
	target.draw(cannonShape);
	target.draw(healthBar);

	int buffCnt = 0;
	for (vector<Buff*>::iterator it = buffList.begin(); it != buffList.end(); ++it)
	{
		RectangleShape sh;
		if ((*it)->buffType == BuffType::Health)
		{
			sh.setFillColor(Color(0, 250, 0));
		}
		else
		{
			sh.setFillColor(Color(250, 0, 0));
		}
		sh.setSize(Vector2f(10, 10));
		sh.setPosition(position - Player::getInstance()->viewportOffset + Vector2f(-50 + buffCnt * 12, 35));
		target.draw(sh);
		
		
		buffCnt++;
	}

}

void Enemy::addBuff(BuffType buffType)
{
	Mutex mutex;
	Lock lock(mutex);
	float time, amount;

	switch (buffType)
	{
	case BuffType::Armor:
		time = 10000;
		amount = 30;
		break;

	case BuffType::Health:
		time = 5000;
		amount = 5;
		break;

	default:
		break;
	}

	buffList.push_back(new Buff(buffType, time, amount));
}