#pragma once
#include "NetworkClient.h"
#include <iostream>
#include "ObjectManager.h"
#include "Player.h"
#include "Constants.h"
#include <thread>

using namespace std;
using namespace sf;

NetworkClient::NetworkClient()
{
}


NetworkClient::~NetworkClient()
{
}

void NetworkClient::update(float time)
{
	sendTimer += time;

	if (sendTimer > 50)
	{
		Packet packet;
		packet << Action::Move << Player::getInstance()->id << Player::getInstance()->position.x << Player::getInstance()->position.y
			   << Player::getInstance()->velocity.x << Player::getInstance()->velocity.y << Player::getInstance()->direction.x << Player::getInstance()->direction.y;
		sendPacket(packet);
		sendTimer = 0;
	}
}

void NetworkClient::recievePacket()
{
	while (true)
	{
		int action, id;
		float  x, y, velx, vely, rotx, roty, hp;
		Packet packet;

		switch (socket->receive(packet))
		{
		case sf::Socket::Done:
		{
			packet >> action;
			switch (action)
			{
			case Action::Move: //move
			{
				packet >> id >> x >> y >> velx >> vely >> rotx >> roty;
				ObjectManager::getInstance()->editEnemy(id, Vector2f(x, y), Vector2f(velx, vely), Vector2f(rotx, roty));
				break;
			}
			case Action::Shot: //Shot
			{
				packet >> id >> x >> y >> velx >> vely;

				ObjectManager::getInstance()->addBullet(id, Vector2f(x, y), Vector2f(velx, vely));
				break;
			}
			case Action::SpawnPlayer:
			{
				packet >> id >> x >> y;
				//cout << "I was spawned at " << x << " " << y << endl;
				if (id == ObjectManager::getInstance()->PLAYER_ID)
				{
					Player::getInstance()->position = Vector2f(x, y);
					//cout << x << " " << y << endl;
				}
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

				break;
			}
			case Action::AllPlayers://список всех клиентов
			{
				int size;
				packet >> size;
				for (int i = 0; i < size; ++i)
				{
					packet >> id >> x >> y >> velx >> vely >> rotx >> roty >> hp;
					ObjectManager::getInstance()->editEnemy(id, Vector2f(x, y), Vector2f(velx, vely), Vector2f(rotx, roty), hp);
				}

				break;
			}
			default:
				break;
			}

		}
		}
	}
}

void NetworkClient::sendPacket(Packet packet)
{
	Lock lock(mutex);
	socket->send(packet);
}


void NetworkClient::doConnect()
{
	socket = new TcpSocket();

	Socket::Status status = socket->connect("172.16.8.48", 53000);
	//Socket::Status status = socket->connect("192.168.56.101", 53000);
	if (status != Socket::Done)
	{
		cout << "ERROR\n";
	}
	bool isIdTaken = false;

	while (!isIdTaken)
	{
		Packet packet;
		int action, id;
		socket->receive(packet);
		packet >> action;
		switch (action)
		{
			
			case Action::SetID:
			{
				packet >> id;
				Player::getInstance()->id = id;
				ObjectManager::getInstance()->PLAYER_ID = id;
				isIdTaken = true;
				break;
			}

			default:
				break;
		}
	}
	
	thread thread(&NetworkClient::recievePacket, this);
	thread.detach();
}

