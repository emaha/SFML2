#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;

class Player
{
public:
	static Player * getInstance() {
		if (!pInstance)
			pInstance = new Player();
		return pInstance;
	}

	Player();
	~Player();

	void Update(float time, Vector2i mousePos);
	void Draw(RenderTarget &target);

	bool isFire();
	bool isSkillAvailable(int i);
	bool isAlive(){ return _isAlive; }
	void useSkill(int skillNum);


	int id;
	string name;
	Vector2f position, velocity, baseSize, towerSize, cannonSize;
	bool _isAlive;

	float abilities[5];

	Vector2f viewportOffset;

	Sprite baseSprite;
	Texture baseTexture;

	float health, speed;
	Color color;
	Vector2f damage;
	RectangleShape baseShape, towerShape, cannonShape;
	float shotTime, gCoolDown;
	Vector2f direction;

private:
	static Player *pInstance;
};

