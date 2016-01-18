/*
 * Snake.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: sph3re
 */

#include "Snake.h"

Snake::Snake()
: active(false),
  resetSnake(false),
  headDirection(EAST)
{}


Snake::~Snake()
{}

void Snake::toggleActive()
{
	active = !active;
	if(active)
	{
		// Start with a 3-piece snake heading east on a fixed position
		headDirection = EAST;
		elements.clear();
		elements.push_back(sf::Vector2i(5, 1));
		elements.push_back(sf::Vector2i(4, 1));
		elements.push_back(sf::Vector2i(3, 1));
	}
	else
		resetSnake = true;
}

void Snake::update(Board& board)
{
	if(!(active || resetSnake))
		return;

	// Store the fieldSize for easier access
	int fieldSize = board.getFieldSize();

	// At first reset all the elements of the old snake on the board
	for(sf::Vector2i& i : elements)
	{
		if(i.x < fieldSize && i.y < fieldSize)
			board.setFieldState(i.x, i.y, State::EMPTY);
	}
	resetSnake = false;

	// If not active, this is the last update frame with this snake, so we can free the memory
	if(!active)
	{
		elements.clear();
		return;
	}

	sf::Vector2i lastPosition = *elements.begin();
	// Move the head 1 step
	switch(headDirection)
	{
	case NORTH:
		lastPosition.y--;
		break;
	case EAST:
		lastPosition.x++;
		break;
	case SOUTH:
		lastPosition.y++;
		break;
	case WEST:
		lastPosition.x--;
	}

	// Let all elements follow
	// The head automatically takes the updated position
	for(sf::Vector2i& i : elements)
	{
		std::swap(lastPosition, i);
	}

	// Check for all elements whether they are outside the field
	// For the tail it is also necessary because the user could have shrinked the field
	for(sf::Vector2i& i : elements)
	{
		i.x += fieldSize;
		i.x = i.x % fieldSize;

		i.y += fieldSize;
		i.y = i.y % fieldSize;
	}

	// If the snake has eaten an occupied field, add one element
	if(board.getField(elements.begin()->x, elements.begin()->y).getState() == State::OCCUPIED)
		elements.push_back(lastPosition);

	// Check if the head has hit another element
	{
		std::list<sf::Vector2i>::iterator i = elements.begin();
		i++;
		for(; i != elements.end(); i++)
		{
			if(*elements.begin() == *i)
				toggleActive();
		}
	}

	// Finally write the snake to the board
	writeSnake(board);
}

void Snake::writeSnake(Board& board)
{
	// Write the snake to the board
	for(sf::Vector2i& i : elements)
	{
		if(i.x < board.getFieldSize() && i.y < board.getFieldSize())
			board.setFieldState(i.x, i.y, State::SNAKE);
	}
}
