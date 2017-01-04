#pragma once
#include "Game.h"
#include "NetworkServer.h"
#include "NetworkClient.h"

bool Game::isServer;

int main() {
	srand(time(NULL));

	cout << "1 - Client\n2 - Server\nChoose: ";

	int a =2;
	cin >> a;

	switch (a)
	{
		case 1:
		{
			Game::isServer = false;
			Game game;	game.run();
			break;
		}
		case 2:
		{
			Game::isServer = true;
			NetworkServer *server = new NetworkServer();
			break;
		}
		default:
			break;
	}
	
	return EXIT_SUCCESS;
}
