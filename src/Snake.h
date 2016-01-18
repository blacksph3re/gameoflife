/*
 * Snake.h
 *
 *  Created on: Jan 16, 2016
 *      Author: sph3re
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include <list>
#include <SFML/Graphics.hpp>
#include "Board.h"

/**
 * The possible directions the snake can go
 * As seen on a map
 */
enum Direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class Snake {
public:
	/**
	 * Moves the snake one step into the headdirection and writes the results to the board
	 */
	void update(Board& board);

	/**
	 * Only writes the current snake to the board in case the field updates have overwritten the snake status
	 */
	void writeSnake(Board& board);

	/**
	 * Sets the direction the head will do in the next step
	 */
	void setHeadDirection(Direction dir) {headDirection = dir;}

	/**
	 * Toggles whether the snake is active or not, it may take one update until the snake disappears
	 */
	void toggleActive();

	Snake();
	virtual ~Snake();

private:
	bool					active;			// Is the snake active
	bool					resetSnake;		// Should the snake be removed from the board the next frame
	std::list<sf::Vector2i> elements;		// All elements of the snake, including the head, in order
	Direction 				headDirection;	// The direction the head will take next frame
};

#endif /* SNAKE_H_ */
