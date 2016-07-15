#include "Player.h"
#include "Constants.h"
#include <math.h>

Player* Player::pInstance = 0;

Player::Player()
{
	name = "Player";

	baseSize = Vector2f(60, 60);
	towerSize = Vector2f(50, 30);
	cannonSize = Vector2f(60, 5);

	_isAlive = true;

	abilities[0] = 1.0f;
	abilities[1] = 1.0f;
	abilities[2] = 1.0f;
	abilities[3] = 1.0f;
	abilities[4] = 1.0f;

	gCoolDown = 3.0f;

	
	damage = Vector2f(50, 100);
	position = Vector2f(500, 500);
	velocity = Vector2f(0, 0);
	health = 1000.0f;
	speed = 200.0f;
	shotTime = 0.0f;
	direction = Vector2f(0, 0);   //base, tower

	baseTexture.loadFromFile("../Data/Images/tank.png", IntRect(0,0,50,75));
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
}


Player::~Player()
{
}

Vector2f LerpPLayer(Vector2f v0, Vector2f v1, float t)
{
	return (1 - t)*v0 + t*v1;
}

void Player::Update(float time, Vector2i mousePos)
{
	if (isAlive())
	{
		position += velocity;

		shotTime += time;
		gCoolDown += time;
		for (int i = 0; i < 3; i++)
		{
			abilities[i] += time;
		}

		viewportOffset = LerpPLayer(viewportOffset, position - Vector2f(APP_WIDTH/2, APP_HEIGHT/2), 0.05f);


		baseSprite.setPosition(position - viewportOffset);
		baseSprite.setRotation(direction.x);

		baseShape.setRotation(direction.x);
		baseShape.setPosition(position - viewportOffset);

		Vector2f deltaPosition;
		deltaPosition = Vector2f(mousePos) - position + viewportOffset;
		direction.y = float((atan2(deltaPosition.y, deltaPosition.x)) * 180 / 3.141592);

		towerShape.setRotation(direction.y);
		towerShape.setPosition(position - viewportOffset);

		cannonShape.setRotation(direction.y);
		cannonShape.setPosition(position - viewportOffset);
	}

	_isAlive = health > 0;
}

void Player::Draw(RenderTarget &target)
{
	target.draw(baseShape);
	//target.draw(baseSprite);
	target.draw(towerShape); 
	target.draw(cannonShape);
}

bool Player::isFire()
{
	if (shotTime > 0.1f && isAlive()){
		shotTime = 0;
		return true;
	}
	return false;
}

bool Player::isSkillAvailable(int i)
{
	if (gCoolDown > 0.5f && isAlive() && abilities[i]>5.0f){
		
		abilities[i] = 0.0f;
		gCoolDown = 0.0f;
		return true;
	}
	return false;
}

void Player::useSkill(int skillNum)
{
	
}

