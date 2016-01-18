/*
 * HelpText.cpp
 *
 *  Created on: Jan 15, 2016
 *      Author: sph3re
 */

#include <iostream>
#include "HelpText.h"

HelpText::HelpText()
{}


HelpText::~HelpText()
{}

bool HelpText::init(Board& board, Snake& snake)
{
	// Load the font
	if(!defaultFont.loadFromFile("YanoneKaffeesatz-Regular.ttf"))
		return false;

	// Create a vector with the text and a bound function to each of the options
	textToRender.emplace_back(sf::Text("(Space) - Pause", defaultFont), [&](){board.pause();});
	textToRender.emplace_back(sf::Text("(C) - Clear", defaultFont), [&](){board.clear();});
	textToRender.emplace_back(sf::Text("(R) - Random population", defaultFont), [&](){board.randomize();});
	textToRender.emplace_back(sf::Text("(PgUp) - Increase size", defaultFont), [&](){board.grow();});
	textToRender.emplace_back(sf::Text("(PgDown) - Decrease size", defaultFont), [&](){board.shrink();});
	textToRender.emplace_back(sf::Text("(G) - Create Glider Gun", defaultFont), [&](){board.createGliderGun();});
	textToRender.emplace_back(sf::Text("(S) - Toggle Snake", defaultFont), [&](){snake.toggleActive();});
	textToRender.emplace_back(sf::Text("(V) - Toggle Vital Snake", defaultFont), [&](){board.toggleVitalSnake();});

	// Set some more properties
	for(auto& i : textToRender)
	{
		std::get<0>(i).setCharacterSize(20);
		std::get<0>(i).setColor(sf::Color(100, 250, 50));
	}

	return true;
}

void HelpText::update(float xOffset, sf::Vector2f mousePos)
{
	float yOffset = 10.0f;
	xOffset += 10.0f;

	// Update the position of each text and check wether the mouse hovers it
	for(auto& i : textToRender)
	{
		std::get<0>(i).setPosition(xOffset, yOffset);
		yOffset += std::get<0>(i).getGlobalBounds().height+3;
		if(std::get<0>(i).getGlobalBounds().contains(mousePos))
			std::get<0>(i).setStyle(sf::Text::Underlined);
		else
			std::get<0>(i).setStyle(sf::Text::Regular);
	}
}

void HelpText::render(sf::RenderTarget& target)
{
	// Draw the texts to screen
	for(auto& i : textToRender)
	{
		target.draw(std::get<0>(i));
	}
}

void HelpText::clicked(int x, int y)
{
	// Check if the mouseclick was inside the bounding box of one of the texts, then invoke the function
	for(auto& i : textToRender)
	{
		if(std::get<0>(i).getGlobalBounds().contains(x, y))
		{
			std::get<1>(i)();
		}
	}
}

