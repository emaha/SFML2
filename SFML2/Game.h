#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Player.h"
#include "HUD.h"
#include "Level.h"
#include "NetworkClient.h"


using namespace sf;

class Game
{
public:
	Game();
	~Game();
	void run();
	void checkEvents(RenderWindow &window, float time) const;
	void update(float time);
	void draw(RenderTarget &target);

	HUD hud;
	Level level;
	NetworkClient *networkClient;

	static bool isServer;

private:
	void createWindow();
	RenderWindow window;
	bool running;
	

	Mutex mutex;

	Shader shader;
};

