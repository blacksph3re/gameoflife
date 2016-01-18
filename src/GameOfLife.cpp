//============================================================================
// Name        : GameOfLife.cpp
// Author      : Nico
// Version     : 0.1
// Copyright   : GPLv3
// Description : A simple C++-Implementation of Conway's Game of Life with usage of SFML
//============================================================================

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Board.h"
#include "HelpText.h"
#include "Snake.h"

int main(int argc, char* argv[]) {
	// Read the initial board size from command line parameters (if given)
	int boardsize = 50;
	if(argc >= 2)
		boardsize = std::stoi(argv[1]);

	// Some initialistion work
	srand (time(NULL));
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Conway's Game of Life");
	Board board(boardsize, 768);
	Snake snake;

	HelpText helpText;
	helpText.init(board, snake);


	sf::Clock snakeTimer;
	sf::Clock updateTimer;

	// Start the updatetimer for the first update
	updateTimer.restart();
	snakeTimer.restart();
	// Run as long as the window is open
	while(window.isOpen())
	{
		/************************************/
		// Event-handling
		// Check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while(window.pollEvent(event))
		{
			// Upon resized window, also resize the board
			if(event.type == sf::Event::Resized)
			{
				board.setScreenSize(std::min(event.size.width, event.size.height));
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			}

			// Close upon system's close event
			if(event.type == sf::Event::Closed)
				window.close();

			// Fetch some keyboard events and invoke the corresponding methods
			if(event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;

				case sf::Keyboard::R:
					board.randomize();
					break;
				case sf::Keyboard::C:
					board.clear();
					break;

				case sf::Keyboard::Space:
					board.pause();
					break;

				case sf::Keyboard::PageUp:
					board.grow();
					break;

				case sf::Keyboard::PageDown:
					board.shrink();
					break;

				case sf::Keyboard::G:
					board.createGliderGun();
					break;

				case sf::Keyboard::S:
					snake.toggleActive();
					break;

				case sf::Keyboard::V:
					board.toggleVitalSnake();
					break;

				case sf::Keyboard::Up:
					snake.setHeadDirection(Direction::NORTH);
					break;

				case sf::Keyboard::Right:
					snake.setHeadDirection(Direction::EAST);
					break;

				case sf::Keyboard::Down:
					snake.setHeadDirection(Direction::SOUTH);
					break;

				case sf::Keyboard::Left:
					snake.setHeadDirection(Direction::WEST);
					break;

				default:
					break;
				}
			}

			// Forward clicks to the board and helptext
			if (event.type == sf::Event::MouseButtonPressed)
			{
				board.clicked(event.mouseButton.x, event.mouseButton.y);
				helpText.clicked(event.mouseButton.x, event.mouseButton.y);
			}

		}

		/************************************/
		// Update-Phase

		if(updateTimer.getElapsedTime() > sf::seconds(0.5))
		{
			updateTimer.restart();
			board.update();
			snake.writeSnake(board);
		}
		helpText.update(board.getScreenSize(), sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));

		if(snakeTimer.getElapsedTime() > sf::seconds(0.2))
		{
			snakeTimer.restart();
			snake.update(board);
		}

		// Sleep some milliseconds to keep CPU-usage down
		sf::sleep(sf::milliseconds(5));

		/************************************/
		// Render-Phase
		window.clear(sf::Color::Black);

		board.render(window);
		helpText.render(window);

		// end the current frame
		window.display();
	}

	return 0;

}
