#pragma once
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <string>


using namespace std;
using namespace sf;

class NetworkClient
{
public:
	NetworkClient();
	~NetworkClient();
	void doConnect();
	void update(float time);
	void sendPacket(Packet packet);
	void recievePacket();
	SocketSelector selector;
	TcpSocket *socket;
	

private:
	Mutex mutex;
	Thread thread;
	float sendTimer;

};

