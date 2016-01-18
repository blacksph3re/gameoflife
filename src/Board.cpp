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
			// Count in the copied field, so the real one can be updated
			// To implement the rule of leaving one side means entering the other, use modulo on index calculation
			// invoke the test-method on each of the fields and add that to the total count
			int count=0;
			count += test(copied[(x-1+fieldSize)%fieldSize][(y-1+fieldSize)%fieldSize]);
			count += test(copied[x]              [(y-1+fieldSize)%fieldSize]);
			count += test(copied[(x+1+fieldSize)%fieldSize][(y-1+fieldSize)%fieldSize]);
			count += test(copied[(x-1+fieldSize)%fieldSize][y]);
			count += test(copied[(x+1+fieldSize)%fieldSize][y]);
			count += test(copied[(x-1+fieldSize)%fieldSize][(y+1+fieldSize)%fieldSize]);
			count += test(copied[x]              [(y+1+fieldSize)%fieldSize]);
			count += test(copied[(x+1+fieldSize)%fieldSize][(y+1+fieldSize)%fieldSize]);

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

void Board::createGliderGun()
{
	// Avoid index out of bounds errors
	if(fieldSize<39)
		return;

	// Left box
	fields[1][5].setState(State::OCCUPIED);
	fields[1][6].setState(State::OCCUPIED);
	fields[2][5].setState(State::OCCUPIED);
	fields[2][6].setState(State::OCCUPIED);

	// Sattelite-dish like thing
	fields[11][5].setState(State::OCCUPIED);
	fields[11][6].setState(State::OCCUPIED);
	fields[11][7].setState(State::OCCUPIED);
	fields[11][5].setState(State::OCCUPIED);
	fields[12][4].setState(State::OCCUPIED);
	fields[12][8].setState(State::OCCUPIED);
	fields[13][3].setState(State::OCCUPIED);
	fields[13][9].setState(State::OCCUPIED);
	fields[14][3].setState(State::OCCUPIED);
	fields[14][9].setState(State::OCCUPIED);

	fields[15][6].setState(State::OCCUPIED);

	fields[16][4].setState(State::OCCUPIED);
	fields[16][8].setState(State::OCCUPIED);
	fields[17][5].setState(State::OCCUPIED);
	fields[17][6].setState(State::OCCUPIED);
	fields[17][7].setState(State::OCCUPIED);
	fields[18][6].setState(State::OCCUPIED);

	// Space-invader alien
	fields[21][3].setState(State::OCCUPIED);
	fields[21][4].setState(State::OCCUPIED);
	fields[21][5].setState(State::OCCUPIED);
	fields[22][3].setState(State::OCCUPIED);
	fields[22][4].setState(State::OCCUPIED);
	fields[22][5].setState(State::OCCUPIED);
	fields[23][2].setState(State::OCCUPIED);
	fields[23][6].setState(State::OCCUPIED);

	fields[25][1].setState(State::OCCUPIED);
	fields[25][2].setState(State::OCCUPIED);

	fields[25][6].setState(State::OCCUPIED);
	fields[25][7].setState(State::OCCUPIED);

	// Right box
	fields[35][3].setState(State::OCCUPIED);
	fields[35][4].setState(State::OCCUPIED);
	fields[36][3].setState(State::OCCUPIED);
	fields[36][4].setState(State::OCCUPIED);

	// Eater
	fields[35][21].setState(State::OCCUPIED);
	fields[35][22].setState(State::OCCUPIED);
	fields[36][21].setState(State::OCCUPIED);
	fields[37][22].setState(State::OCCUPIED);
	fields[37][23].setState(State::OCCUPIED);
	fields[37][24].setState(State::OCCUPIED);
	fields[38][24].setState(State::OCCUPIED);

}
