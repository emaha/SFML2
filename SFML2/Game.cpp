#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>


#include "Game.h"
#include "Constants.h"
#include "ObjectManager.h"
#include "NetworkClient.h"


Game::Game() : running(true)
{
	networkClient = new NetworkClient();
}


Game::~Game()
{
	window.close();
}

void Game::createWindow()
{
	if (window.isOpen())
		window.close();

	window.create(sf::VideoMode(APP_WIDTH, APP_HEIGHT), APP_TITLE, sf::Style::Default);
	window.setKeyRepeatEnabled(true);
	window.setFramerateLimit(APP_FPS); 
}


void Game::run()
{
	createWindow();
	Clock clock;
	networkClient->doConnect();

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		time /= 1000;

		window.clear();

		checkEvents(window, time);
		update(time);
		draw(window);

		window.display();
		
	}
}

void Game::update(float time)
{
	networkClient->update(time);
	ObjectManager::getInstance()->update(time);
	hud.update(time);
	Player::getInstance()->Update(time, Mouse::getPosition(window));
}

void Game::draw(RenderTarget &target)
{
	level.draw(window);
	ObjectManager::getInstance()->draw(window);
	Player::getInstance()->Draw(window);
	hud.draw(window);
}

void Game::checkEvents(RenderWindow &window, float time)
{
	Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:
		{
			window.close();
			break;
		}
		case Event::KeyPressed:
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }
			break;
		}
		case Event::KeyReleased:
		{
			//if (event.key.code == Keyboard::W || event.key.code == Keyboard::S) { Player::getInstance()->velocity = Vector2f(0, 0); }
			break;
		}
		default:
			break;
		}
	}

	if (Player::getInstance()->isAlive())
	{

		if (Mouse::isButtonPressed(Mouse::Left)) 
		{ 
			if (Player::getInstance()->isFire())
			{
				Vector2i mousePos = Mouse::getPosition(window);
				
				//�� ����� ������� ������ ��������))))
				mousePos.y += rand() % 20 - 10;
				mousePos.x += rand() % 20 - 10;
				
				//�������� ������������� ������
				Vector2f vector = Vector2f(mousePos) - Player::getInstance()->position;
				float lenght = sqrt(vector.x * vector.x + vector.y * vector.y);
				Vector2f normVelocity(vector.x / lenght, vector.y / lenght);

				//ObjectManager::getInstance()->addBullet(Player::getInstance()->id, Player::getInstance()->position, normVelocity);

				Packet packet;
				packet << Action::Shot << Player::getInstance()->id << Player::getInstance()->position.x 
					<< Player::getInstance()->position.y << normVelocity.x << normVelocity.y;
				networkClient->sendPacket(packet);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::A)) { Player::getInstance()->direction.x -= 100.0f * time; }
		if (Keyboard::isKeyPressed(Keyboard::D)) { Player::getInstance()->direction.x += 100.0f * time; }
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			Vector2f forwardVector(sin(Player::getInstance()->direction.x * DEG2RAD), -cos(Player::getInstance()->direction.x * DEG2RAD));
			Player::getInstance()->position += forwardVector * 200.0f * time;
		}

		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			Vector2f forwardVector(sin(Player::getInstance()->direction.x * DEG2RAD), -cos(Player::getInstance()->direction.x * DEG2RAD));
			Player::getInstance()->position -= forwardVector * 200.0f * time;
		}
	} 
	else
	{
		if (Keyboard::isKeyPressed(Keyboard::Space)){}
	}

	if (Mouse::isButtonPressed(Mouse::Right)) { ObjectManager::getInstance()->addEnemy(); }


}