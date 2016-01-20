/*
 * Board.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: sph3re
 */

#include "Board.h"
#include <stdlib.h>

Board::Board(int fieldSize, float screenSize)
: fullRepaint(true),
  screenSize(screenSize),
  paused(false),
  vitalSnake(true),
  fieldSize(fieldSize)
{
	init();
}


void Board::init()
{
	// Reset the drawBuffer
	drawBuffer.reset(new sf::RenderTexture());
	drawBuffer->create(screenSize, screenSize);
	drawBuffer->clear(sf::Color::Black);
	fullRepaint = true;

	// Reset the fields
	fields.clear();
	fields.reserve(fieldSize);
	for(int x=0; x<fieldSize; x++)
	{
		fields.emplace_back();
		fields[x].reserve(fieldSize);
		for(int y=0; y<fieldSize; y++)
			fields[x].emplace_back();
	}
}

Board::~Board() {}


void Board::update()
{
	if(paused)
		return;

	// A field is counted when it's either occupied or a snake part
	auto test = [&] (Field f) { return f.getState() == State::OCCUPIED || (vitalSnake && f.getState() == State::SNAKE); };

	// Create a copy of the whole field
	std::vector< std::vector<Field> > copied(fields);

	// Loop through the field and apply conway's rules to each field
	for(int x=0; x<fieldSize; x++)
	{
		for(int y=0; y<fieldSize; y++)
		{
			// Precalculate indices
			int left = (x-1+fieldSize)%fieldSize;
			int right = (x+1+fieldSize)%fieldSize;
			int top = (y-1+fieldSize)%fieldSize;
			int bottom = (y+1+fieldSize)%fieldSize;

			// Count in the copied field, so the real one can be updated
			// To implement the rule of leaving one side means entering the other, use modulo on index calculation
			// invoke the test-method on each of the fields and add that to the total count
			int count=0;
			count += test(copied[left] [top]);
			count += test(copied[x]    [top]);
			count += test(copied[right][top]);
			count += test(copied[left] [y]);
			count += test(copied[right][y]);
			count += test(copied[left] [bottom]);
			count += test(copied[x]    [bottom]);
			count += test(copied[right][bottom]);

			// Then apply the rules
			fields[x][y].update(count);
		}
	}
}

void Board::render(sf::RenderTarget &target)
{
	// A step is the size of a field on screen
	float stepsize = screenSize/(float)fieldSize;

	// Only use one shape and move it to the position where it's needed
	sf::RectangleShape shape;
	if(stepsize > 9)
		shape.setSize(sf::Vector2f(stepsize-stepsize/5, stepsize-stepsize/5));
	else if(stepsize > 1.9)
		shape.setSize(sf::Vector2f(stepsize-1, stepsize-1));
	else
		shape.setSize(sf::Vector2f(stepsize, stepsize));


	// On full repaint, clear the draw buffer beforehand
	if(fullRepaint)
		drawBuffer->clear();

	// Loop through all fields and if it needs to be updated, move the shape there and draw it to the buffer texture
	for(int x=0; x<fieldSize; x++)
	{
		for(int y=0; y<fieldSize; y++)
		{
			if(fullRepaint || fields[x][y].hasChanged())
			{
				shape.setPosition(x*stepsize, y*stepsize);
				if(fields[x][y].getState() == State::EMPTY)
					shape.setFillColor(sf::Color(50, 50, 50));
				else if(fields[x][y].getState() == State::OCCUPIED)
					shape.setFillColor(sf::Color(100, 250, 50));
				else
					shape.setFillColor(sf::Color(250, 100, 50));

				drawBuffer->draw(shape);
			}

			fields[x][y].resetChanged();
		}
	}

	fullRepaint = false;

	// Display finishes the rendering on texture
	drawBuffer->display();

	// Now draw the buffer to screen
	sf::Sprite sprite(drawBuffer->getTexture());
	target.draw(sprite);
}

void Board::randomize()
{
	// Loop through all fields and set the state based on lovely c random
	for(auto& row : fields)
	{
		for(auto& field : row)
		{
			if(rand()%2 == 0)
				field.setState(State::EMPTY);
			else
				field.setState(State::OCCUPIED);
		}
	}
}

int Board::countOccupied() const
{
	int retval = 0;
	for(auto& row : fields)
	{
		retval += std::count_if(row.begin(), row.end(), [] (Field f) { return f.getState() != State::EMPTY; });

	}
	return retval;
}


void Board::clicked(int x, int y)
{
	// If clicked outside the board, ignore
	if(x < 0 || x >= screenSize || y < 0 || y >= screenSize)
		return;

	// Determine which field the user has clicked and toggle that one
	float stepsize = screenSize/fieldSize;
	fields[x/stepsize][y/stepsize].toggleState();

}

void Board::pause()
{
	paused = !paused;
}

void Board::clear()
{
	for(auto& row : fields)
	{
		for(auto& field : row)
			field.setState(State::EMPTY);
	}
}

void Board::shrink()
{
	if(fieldSize <= 1)
		return;

	fieldSize--;
	// Remove the lowest row
	fields.pop_back();
	// Remove the last field in all other rows
	for(auto& row : fields)
		row.pop_back();

	// We need a full repaint because the stepsize has changed
	fullRepaint = true;
}

void Board::grow()
{
	fieldSize++;

	// Add an empty field on the end of each row
	for(auto& row : fields)
		row.emplace_back();

	// Add another full row
	fields.emplace_back();
	for(int i=0; i<fieldSize; i++)
		fields[fieldSize-1].emplace_back();

	// We need a full repaint because the stepsize has changed
	fullRepaint = true;
}

void Board::setFieldSize(int size)
{
	fieldSize = size;

	// Reset the fields
	fields.clear();
	fields.reserve(fieldSize);
	for(int x=0; x<fieldSize; x++)
	{
		fields.emplace_back();
		fields[x].reserve(fieldSize);
		for(int y=0; y<fieldSize; y++)
			fields[x].emplace_back();
	}

	fullRepaint = true;
}

void Board::setScreenSize(float newScreenSize)
{
	screenSize = newScreenSize;

	// Reset the draw buffer
	// Unfortunately we have to create a full new texture as changing the texture size is not supported
	drawBuffer.reset(new sf::RenderTexture());
	drawBuffer->create(screenSize, screenSize);
	drawBuffer->clear(sf::Color::Black);
	fullRepaint = true;
}

