#include "Enemy.h"
#include <iostream>
#include "Player.h"


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
	respawnTimer = 3.0f;
	state = Alive;

	position = Vector2f(100, 150);
	velocity = Vector2f(0, 0);
	
	baseSize = Vector2f(60, 60);
	towerSize = Vector2f(50, 30);
	cannonSize = Vector2f(60, 5);

	damage = Vector2f(50, 100);
	baseHealth = health = 1000;
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
		respawnTimer -= time;
		if (respawnTimer<0.0f)
		{
			respawn();
		}
	}

	if (isAlive())
	{


		updateBuff(time);


	}
	else
	{
		if (respawnTimer - time < 0.0f)
		{
			respawn();
		}
	}


}

void Enemy::updateBuff(float time)
{
	for (vector<Buff*>::iterator it = buffList.begin(); it != buffList.end();)
	{
		if ((*it)->isActive)
		{
			(*it)->update(time);
			if((*it)->isReady())
			{
				health += (*it)->amount;
				if (health > baseHealth) health = baseHealth;
			}
			++it;
		}
		else
		{
			delete *it;
			it = buffList.erase(it);
		}
	}
}


void Enemy::respawn()
{
	//cout << id << " " << "respawn" << endl;
	respawnTimer = 3.0f;
	health = 1000.0f;
	state = Spawn;
	_isAlive = true;
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

void Enemy::addBuff(BuffType buffType)
{
	buffList.push_back(new Buff(buffType, 5.0f, 10.0f));
}