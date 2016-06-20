#pragma once
#include "NetworkServer.h"

#include <string.h>
#include "Constants.h"
#include "ObjectManager.h"
#include <cmath>
#include <SFML/System.hpp>
#include <iostream>
#include <functional>   // std::bind
#include <thread>
#include <cassert>

using namespace std;

Mutex mutex;
list<TcpSocket*> clients;



NetworkServer::NetworkServer()
{
	id = 0;
	startSever();

}


NetworkServer::~NetworkServer()
{
}


void NetworkServer::startSever()
{
	cout << "Listening..." << endl;

	ObjectManager::getInstance()->isServer = true;

	thread thread(&NetworkServer::acceptLoop, this);
	thread.detach();

	float time, lastTime = 0.0f;

	while (true)
	{
		time = clock.getElapsedTime().asMilliseconds() / 1000.0f;

		if (time - lastTime >= (1.0f / 60.0f))
		{
			update(time - lastTime);  //переделать!!!!!!!!!!
			lastTime = clock.getElapsedTime().asMilliseconds()/1000.0f;
			sleep(milliseconds(10));
		}
		
	}
}

void NetworkServer::acceptLoop()
{
	TcpListener listener;
	listener.listen(53000);
	TcpSocket* socket;

	while (true)
	{
		socket = new TcpSocket;
		if (listener.accept(*socket) == sf::Socket::Done)
		{

			clients.push_back(socket);
			cout << "Connected" << endl;
			auto t = thread(&NetworkServer::recieveLoop, this, ref(socket));
			t.detach();
		}

	}
}

void NetworkServer::recieveLoop(TcpSocket *socket)
{
	Packet packet;
	srand(time(NULL));
	packet << Action::SetID << id++;
	socket->send(packet);

	while (true)
	{
		sleep(milliseconds(10));
		sf::Packet packet;
		switch (socket->receive(packet))
		{
		case sf::Socket::Done:
		{
			int action, id;
			packet >> action;
			switch (action)
			{
			case Action::Move: 
			{
				float x, y, velx, vely, rx, ry;
				packet >> id >> x >> y >> velx >> vely >> rx >> ry;
				ObjectManager::getInstance()->editEnemy(id, Vector2f(x, y), Vector2f(velx, vely), Vector2f(rx, ry));
				break;
			}
			case Action::Shot: 
			{
				float x, y, velx, vely;
				packet >> id >> x >> y >> velx >> vely;
				ObjectManager::getInstance()->addBullet(id, Vector2f(x, y), Vector2f(velx, vely));
				sendPacketToAll(packet);
				break;
			}
			case Action::UseSkill: //смысл такой же как и у Shot, мб стоит и объеденить
			{
				int skillNum;
				packet >> id >> skillNum;
				ObjectManager::getInstance()->enemyMap.find(id)->second->setDamage(-250);
				break;
			}

			default:
				break;
			}
			break;
		}
		case sf::Socket::Disconnected:
		{
			socket->disconnect();
			cout << "Disconnected..." << clients.size() <<  endl;
			delete(socket);
			break;
		}
		default:
			break;
		}
	}
}




void NetworkServer::update(float time)
{
	ObjectManager::getInstance()->update(time);
	sendGameStateToAll(time);
}

void NetworkServer::sendGameStateToAll(float time)
{
	if (sendtimer >= 0.1f)  
	{
		Packet packet;
		packet << Action::AllPlayers << ObjectManager::getInstance()->enemyMap.size(); //список всех пользователей
		
		for (map<int, Enemy*>::iterator it = ObjectManager::getInstance()->enemyMap.begin(); it != ObjectManager::getInstance()->enemyMap.end(); ++ it)
		{
			Enemy* en = it->second;
			
			if (en->state == Enemy::Spawn)
			{
				float x, y;
				x = rand() % 1000;	y = rand() % 800;
				Packet p;
				p << Action::SpawnPlayer << en->id <<  x << y;
				sendPacketToAll(p);

				en->state = Enemy::Alive;
				en->_isAlive = true;
			}
			
			packet << en->id << en->position.x << en->position.y
				<< en->velocity.x << en->velocity.y
				<< en->direction.x << en->direction.y
				<< en->health;

		}

		sendPacketToAll(packet);
		sendtimer = 0;
	}
	sendtimer += time;
}

void NetworkServer::sendPacketToAll(Packet packet)
{
	for (std::list<sf::TcpSocket*>::iterator iter = clients.begin(); iter != clients.end(); ++iter)
	{
		sf::TcpSocket& clnt = **iter;
		
		clnt.send(packet);
	}
}
