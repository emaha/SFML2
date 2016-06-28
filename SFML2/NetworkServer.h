#pragma once

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <list>

using namespace sf;
using namespace std;

class NetworkServer
{
public:
	NetworkServer();
	~NetworkServer();
	void startSever();
	void sendGameStateToAll(float time);
	void sendPacketToAll(Packet packet);
	void update(float time);
	//void checkCollision();
	void recievePacket();

	void acceptLoop();
	void recieveLoop(TcpSocket* socket);

	int id;
	float sendtimer =0;
	Clock clock;
	//TcpListener listener;

	//Mutex mutex;
	//Thread thread;

	//list<TcpSocket*> clients;
	//SocketSelector selector;

};

