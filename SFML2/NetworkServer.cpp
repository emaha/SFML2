#pragma once
#include "NetworkServer.h"

#include "Constants.h"
#include "ObjectManager.h"
#include <SFML/System.hpp>
#include <iostream>
#include <thread>

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

	thread thread(&NetworkServer::acceptLoop, this);
	thread.detach();

	while (true)
	{
		float time = clock.getElapsedTime().asMilliseconds();

		if (time >= 1000 / 60)
		{
			clock.restart();
			update(time);
		}
		sleep(milliseconds(10));
	}
}

void NetworkServer::acceptLoop()
{
	TcpListener listener;
	listener.listen(53000);
	TcpSocket* socket;

	while (true)
	{
		sleep(milliseconds(10));
		cout << "Waitint for client..." << endl;
		socket = new TcpSocket;
		if (listener.accept(*socket) == sf::Socket::Done)
		{

			clients.push_back(socket);
			cout << "Connected" << endl;
			auto t = thread(&NetworkServer::recieveLoop, this, socket);
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
	bool isAlive = true;

	while (isAlive)
	{
		//sleep(milliseconds(10));
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
				switch (skillNum)
				{
					case Skill::Cure:
					{
						ObjectManager::getInstance()->enemyMap.find(id)->second->addBuff(BuffType::Health);
						break;
					}
					case Skill::ShieldWall:
					{
						ObjectManager::getInstance()->enemyMap.find(id)->second->addBuff(BuffType::Armor);
						break;
					}

				}

				sendPacketToAll(packet);
				break;
			}

			default:
				break;
			}
			break;
		}
		case sf::Socket::Disconnected:
		{
			isAlive = false;
			socket->disconnect();
			cout << "Disconnected..." << clients.size() <<  endl;

			
			break;
		}
		default:
			break;
		}

	}

	delete(socket);
}




void NetworkServer::update(float time)
{
	ObjectManager::getInstance()->update(time);
	sendGameStateToAll(time);
}

void NetworkServer::sendGameStateToAll(float time)
{
	if (sendtimer >= 10)  //10 Milliseconds
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
	for (auto iter = clients.begin(); iter != clients.end(); ++iter)
	{
		(*iter)->send(packet);
	}
}
