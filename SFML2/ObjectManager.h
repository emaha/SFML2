#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Enemy.h"

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

	void addEnemy();
	void addBullet(int id, Vector2f position, Vector2f velocity);

	void editEnemy(int id, Vector2f pos, Vector2f vel, Vector2f dir, float hp);
	void editEnemy(int id, Vector2f pos, Vector2f vel, Vector2f dir);

	
	void killAll();
	void killEntity(int id);
	void reSpawnPlayer();
	void update(float time);
	void draw(RenderTarget &target);
	void checkCollision();
	bool isCollide(Sprite a, Sprite b);

	int getPoolSize(){ return entityList.size(); }

	int PLAYER_ID;
	bool isServer = false;
	
	float getDistance(Vector2f a, Vector2f b);
	
	vector<Entity*> entityList;
	vector<Enemy*> enemyList;

	Mutex mutex;

private:
	static ObjectManager *pInstance;
};

