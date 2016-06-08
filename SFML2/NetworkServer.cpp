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


void NetworkServer::acceptLoop()
{
	TcpListener listener;
	SocketSelector selector;
	listener.listen(53000);
	selector.add(listener);
	listener.setBlocking(false);
	TcpSocket* socket;
	cout << "init" << endl;

	while (true)
	{
		cout << "while true" << endl;
		if (selector.wait(milliseconds(100)))
		{
			cout << "selector wait" << endl;
			Lock lock(mutex);
			//mutex.lock();
			socket = new TcpSocket;
			if (listener.accept(*socket) == sf::Socket::Done)
			{
				
				clients.push_back(socket);
				cout << "Connected" << endl;
				auto t = thread(&NetworkServer::recieveLoop, this, ref(socket));
				t.detach();
			}
			//mutex.unlock();
		}
	}
}

void NetworkServer::recieveLoop(TcpSocket *socket)
{
	Packet packet;
	srand(time(0));
	packet << id++;
	socket->send(packet);

	while (true)
	{
		//sleep(milliseconds(1));
		//cout << "recieve loop" << endl;
		sf::Packet packet;
		Lock lock(mutex);
		//mutex.lock();
		switch (socket->receive(packet))
		{
		case sf::Socket::Done:
		{
			int action, id;
			packet >> action;
			switch (action)
			{
			case Action::Move: //Move
			{
				float x, y, velx, vely, rx, ry;
				packet >> id >> x >> y >> velx >> vely >> rx >> ry;
				ObjectManager::getInstance()->editEnemy(id, Vector2f(x, y), Vector2f(velx, vely), Vector2f(rx, ry));
				//cout << "MOVE id: " << id << endl;
				break;
			}
			case Action::Shot: //Shot
			{
				cout << "SHOT" << endl;
				float x, y, velx, vely;
				packet >> id >> x >> y >> velx >> vely;
				ObjectManager::getInstance()->addBullet(id, Vector2f(x, y), Vector2f(velx, vely));
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
			socket->disconnect();
			//cout << "Disconnected..." << clients.size() << endl;
			break;
		}
		default:
			break;
		}

		//mutex.unlock();
	}
}

void NetworkServer::startSever()
{
	cout << "Listening..." << endl;
	
	thread thread(&NetworkServer::acceptLoop, this);
	thread.detach();

	float time = 0.0f, lastTime = 0.0f;
	
	while (true)
	{
		//cout << "main loop" << endl;
		time = clock.getElapsedTime().asMilliseconds()/1000.0f;
		
		if (time - lastTime >= (1.0f / 60.0f))
		{
			clock.restart();
			
			update(time);
			lastTime = clock.getElapsedTime().asMilliseconds()/1000.0f;
		}
		
		
		//sleep(milliseconds(10));
	}
}


void NetworkServer::update(float time)
{
	sendGameStateToAll(time);
	ObjectManager::getInstance()->update(time);
}

void NetworkServer::sendGameStateToAll(float time)
{
	if (sendtimer >= 0.02f)  
	{
		//cout << "sendGameStateToAll\n";
		//формируем пакет
		Lock lock(mutex);
		//mutex.lock();
		Packet packet;
		packet << Action::AllPlayers << ObjectManager::getInstance()->enemyList.size(); //список всех пользователей
		
		
		for (vector<Enemy*>::iterator it = ObjectManager::getInstance()->enemyList.begin(); it != ObjectManager::getInstance()->enemyList.end(); ++it)
		{
			packet << (*it)->id << (*it)->position.x << (*it)->position.y
				   << (*it)->velocity.x << (*it)->velocity.y 
				   << (*it)->direction.x << (*it)->direction.y 
				   << (*it)->health;
		}
		sendPacketToAll(packet);

		sendtimer = 0.0f;
		//mutex.unlock();
	}

	sendtimer += time;
}

void NetworkServer::sendPacketToAll(Packet packet)
{
	//mutex.lock();
	Lock lock(mutex);
	for (std::list<sf::TcpSocket*>::iterator iter = clients.begin(); iter != clients.end(); ++iter)
	{
		sf::TcpSocket& clnt = **iter;
		clnt.send(packet);
	}
	//mutex.unlock();
}
