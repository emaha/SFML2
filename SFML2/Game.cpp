#include <iostream>
#include <cstdlib>

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
	//window.setFramerateLimit(APP_FPS); 
}


void Game::run()
{
	createWindow();
	Clock clock;
	networkClient->doConnect();
	

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMilliseconds();
		
		if (time >= 1000 / 60)
		{
			clock.restart();
			window.clear();

			//time /= 1000.0f;
			checkEvents(window, time);
			update(time);
			draw(window);
			window.display();
		}
	}
}

void Game::update(float time)
{
	level.update(time);
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

void Game::checkEvents(RenderWindow &window, float time) const
{
	Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:
		{
			window.close();
			networkClient->socket->disconnect();
			break;
		}
		case Event::KeyPressed:
		{
			if (Keyboard::isKeyPressed(Keyboard::Num1)) 
			{
				if (Player::getInstance()->isSkillAvailable(0))
				{
					Packet packet;
					packet << Action::UseSkill << Player::getInstance()->id << Skill::ShieldWall;
					networkClient->sendPacket(packet);
					cout << "Skill 1 used" << endl;

				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Num2))
			{
				if (Player::getInstance()->isSkillAvailable(1))
				{
					Packet packet;
					packet << Action::UseSkill << Player::getInstance()->id << Skill::Cure;
					networkClient->sendPacket(packet);
					cout << "Skill 2 used" << endl;
				}
			}

			break;
		}
		case Event::KeyReleased:
		{
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
				Vector2f vector = Vector2f(mousePos) - Player::getInstance()->position + Player::getInstance()->viewportOffset;
				float lenght = sqrt(vector.x * vector.x + vector.y * vector.y);
				Vector2f normVelocity(vector.x / lenght, vector.y / lenght);

				//ObjectManager::getInstance()->addBullet(Player::getInstance()->id, Player::getInstance()->position, normVelocity);

				Packet packet;
				packet << Action::Shot << Player::getInstance()->id << Player::getInstance()->position.x 
					<< Player::getInstance()->position.y << normVelocity.x << normVelocity.y;
				networkClient->sendPacket(packet);
			}
		}
		
		if (Keyboard::isKeyPressed(Keyboard::W)) { Player::getInstance()->position.y -= Player::getInstance()->speed * time; }
		if (Keyboard::isKeyPressed(Keyboard::S)) { Player::getInstance()->position.y += Player::getInstance()->speed * time; }
		if (Keyboard::isKeyPressed(Keyboard::A)) { Player::getInstance()->position.x -= Player::getInstance()->speed * time; }
		if (Keyboard::isKeyPressed(Keyboard::D)) { Player::getInstance()->position.x += Player::getInstance()->speed * time; }
		
		/* //  ���������� ��� �� �����
		if (Keyboard::isKeyPressed(Keyboard::A)) { Player::getInstance()->direction.x -= 0.1f * time; }
		if (Keyboard::isKeyPressed(Keyboard::D)) { Player::getInstance()->direction.x += 0.1f * time; }
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			Vector2f forwardVector(sin(Player::getInstance()->direction.x * DEG2RAD), -cos(Player::getInstance()->direction.x * DEG2RAD));
			Player::getInstance()->position += forwardVector * 0.2f * time;
		}

		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			Vector2f forwardVector(sin(Player::getInstance()->direction.x * DEG2RAD), -cos(Player::getInstance()->direction.x * DEG2RAD));
			Player::getInstance()->position -= forwardVector * 0.2f * time;
		}
		*/
	} 
	else
	{
		if (Keyboard::isKeyPressed(Keyboard::Space)){}
	}
}
