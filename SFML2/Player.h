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
	bool isAlive(){ return _isAlive; }


	int id;
	string name;
	Vector2f position, velocity, baseSize, towerSize, cannonSize;
	bool _isAlive;


	Sprite baseSprite;
	Texture baseTexture;

	float health;
	Color color;
	Vector2f damage;
	RectangleShape baseShape, towerShape, cannonShape;
	float shotTime;
	Vector2f direction;

private:
	static Player *pInstance;
};

