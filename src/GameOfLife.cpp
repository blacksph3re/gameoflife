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
#include <experimental/optional>
#include "Board.h"
#include "HelpText.h"
#include "Snake.h"
#include "RLEParser.h"

#ifdef PARSE_CMD
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#endif

int main(int argc, char* argv[]) {
	// Use boost to read command line parameters, if wanted
	int boardsize = 50;
	float updateMultiplicator=1.0f;
	std::experimental::optional<std::string> rleFile;


#ifdef PARSE_CMD
	try
	{
		po::options_description desc("Allowed options");
		desc.add_options()
	    								("help", "produce help message")
										("boardsize", po::value<int>(), "the initial size of the board in both dimensions")
										("updatemultiplicator", po::value<float>(), "a multiplicator for the update frequency, e.G. 2 would be twice as fast updating")
										("load-rle", po::value<std::string>(), "a rle file to load on program start")
										;

		po::positional_options_description p;
		p.add("load-rle", -1);

		po::variables_map vm;

		po::store(po::command_line_parser(argc, argv).
				options(desc).positional(p).run(), vm);

		po::notify(vm);

		if (vm.count("help"))
		{
			std::cout << desc << std::endl;
			return 1;
		}

		if (vm.count("boardsize"))
			boardsize = vm["boardsize"].as<int>();

		if(vm.count("updatemultiplicator"))
			updateMultiplicator = vm["updatemultiplicator"].as<float>();

		if(vm.count("load-rle"))
			rleFile = vm["load-rle"].as<std::string>();
	}
	catch(const po::error& ex)
	{
		std::cerr << ex.what() << std::endl;
		return -1;
	}

#endif


	// Some initialistion work
	srand (time(NULL));
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Conway's Game of Life");
	Board board(boardsize, 768);
	Snake snake;


	HelpText helpText;
	helpText.init(board, snake, updateMultiplicator);

	if(rleFile)
		RLEParser::parseFile(*rleFile, board);


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

				case sf::Keyboard::I:
					updateMultiplicator *= 2;
					break;

				case sf::Keyboard::D:
					updateMultiplicator /= 2;
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

		if(updateTimer.getElapsedTime() > sf::seconds(0.5*(1/updateMultiplicator)))
		{
			updateTimer.restart();
			board.update();
			snake.writeSnake(board);
		}
		helpText.update(board.getScreenSize(), sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));

		if(snakeTimer.getElapsedTime() > sf::seconds(0.2*(1/updateMultiplicator)))
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
