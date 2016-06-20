#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Bullet.h"

using namespace std;

class ObjectManager
{
public:
	static ObjectManager * getInstance() {
		if (!pInstance)
			pInstance = new ObjectManager();
		return pInstance;
	}

	ObjectManager();
	~ObjectManager();

	void addBullet(int id, Vector2f position, Vector2f velocity);

	void editEnemy(int id, Vector2f pos, Vector2f vel, Vector2f dir, float hp);
	void editEnemy(int id, Vector2f pos, Vector2f vel, Vector2f dir);

	void reSpawnPlayer();
	void update(float time);
	void draw(RenderTarget &target);
	void checkCollision();
	bool isCollide(Vector2f a, Vector2f b);

	int PLAYER_ID;
	bool isServer = false;
	
	float getDistance(Vector2f a, Vector2f b);

	map<int, Enemy*> enemyMap;
	vector<Bullet*> bulletList;

	

	Mutex mutex;

private:
	static ObjectManager *pInstance;
};

