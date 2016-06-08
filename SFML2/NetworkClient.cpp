#include "NetworkClient.h"

#include <iostream>
#include "ObjectManager.h"
#include "Player.h"
#include "Constants.h"

using namespace std;
using namespace sf;

NetworkClient::NetworkClient()  : thread(&NetworkClient::recievePacket, this)
{
}


NetworkClient::~NetworkClient()
{
}

void NetworkClient::update(float time)
{
	//cout << "Network update" << endl;

	sendTimer += time;

	recievePacket();

	if (sendTimer > 0.1f)
	{
		Packet packet;
		packet << Action::Move << Player::getInstance()->id << Player::getInstance()->position.x << Player::getInstance()->position.y
			   << Player::getInstance()->velocity.x << Player::getInstance()->velocity.y << Player::getInstance()->direction.x << Player::getInstance()->direction.y;
		sendPacket(packet);
		sendTimer = 0.0f;
	}
}

void NetworkClient::recievePacket()
{
	//while (true)
	//{
		//cout << "while true" << endl;
		if (selector.wait(milliseconds(1)))
		{
			//cout << "selector wait" << endl;
			
			if (selector.isReady(*socket))
			{
				Lock lock(mutex);

				int action, id;
				float  x, y, velx, vely, rotx, roty, hp;
				Packet packet;
				socket->receive(packet);
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
					cout << "SHOT" << endl;
					packet >> id >> x >> y >> velx >> vely;
					ObjectManager::getInstance()->addBullet(id, Vector2f(x, y), Vector2f(velx, vely));
					break;
				}
				case Action::Kill:
				{
					packet >> id >> x >> y >> velx >> vely;
					ObjectManager::getInstance()->killEntity(id);
					break;
				}
				case Action::AllPlayers://список всех клиентов
				{
					//cout << "allPlayers" << endl;
					int size;
					packet >> size;
					for (int i = 0; i < size; ++i)
					{
						packet >> id >> x >> y >> velx >> vely >> rotx >> roty >> hp;
						//cout << x << '\t' << y << endl;
						ObjectManager::getInstance()->editEnemy(id, Vector2f(x, y), Vector2f(velx, vely), Vector2f(rotx, roty), hp);
					}

					break;
				}
				default:
					break;
				}
			}
			
		}
	//}
}

void NetworkClient::sendPacket(Packet packet)
{
	socket->send(packet);
}


void NetworkClient::doConnect()
{
	socket = new TcpSocket();
	//thread.launch();  //запускаем поток приема данных

	Socket::Status status = socket->connect("172.16.8.48", 53000);
	if (status != Socket::Done)
	{
		cout << "ERROR\n";
	}
	selector.add(*socket);

	Packet packet;
	int id;
	socket->receive(packet);
	packet >> id;
	Player::getInstance()->id = id;
	ObjectManager::getInstance()->PLAYER_ID = id;
}

