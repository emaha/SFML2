#pragma once
#include "Game.h"
#include "Parallax.h"
#include "NetworkServer.h"
#include "NetworkClient.h"

int main() {
	srand(time(NULL));

	cout << "1 - Client\n2 - Server\nChoose: ";

	int a =1;
	cin >> a;

	switch (a)
	{
		case 1:
		{
			Game game;	game.run();
			break;
		}
		case 2:
		{
			NetworkServer *server = new NetworkServer();
			break;
		}
		default:
			break;
	}

	return EXIT_SUCCESS;
}
