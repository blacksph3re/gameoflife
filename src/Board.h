/*
 * Board.h
 *
 *  Created on: Jan 14, 2016
 *      Author: sph3re
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Field.h"

class Board {
public:
	/************************************/
	// Frametime-Methods

	/**
	 * Does one time-step in the sense of Conway's game of life
	 */
	void update();
	/**
	 * Renders the current state of the board to the left side of the target
	 */
	void render(sf::RenderTarget &target);

	/**
	 * Dis/Enables updating
	 */
	void pause();


	/************************************/
	// Board-control methods

	/**
	 * Randomizes the setup of the board
	 */
	void randomize();

	/**
	 * Returns the number of occupied fields
	 */
	int countOccupied() const;

	/**
	 * Takes click coordinates relative to the board and toggles the field under the mouse
	 */
	void clicked(int x, int y);

	/**
	 * Sets all fields to empty
	 */
	void clear();

	/**
	 * Reduces the field size by 1 in both dimensions
	 */
	void shrink();

	/**
	 * Increases the field size by 1 in both dimensions
	 */
	void grow();

	/**
	 * Creates a Gospel's Glider Gun if the field is big enough
	 */
	void createGliderGun();

	/************************************/
	// Other methods

	/**
	 * Sets the size of the board on screen (in both dimensions, the board is a square)
	 */
	void setScreenSize(float newScreenSize);

	/**
	 * Returns the current size of the board on the screen (in both dimensions, the board is a square)
	 */
	float getScreenSize() const {return screenSize;}

	/**
	 * Returns the fieldSize, meaning the number of fields in each direction
	 */
	int getFieldSize() const {return fieldSize;}

	/**
	 * Returns the field on a given position
	 * No index checks will be performed, pass a valid index!
	 */
	const Field& getField(int x, int y) const {return fields[x][y];}

	/**
	 * Sets a field on a given position
	 * No index checks will be performed, pass a valid index!
	 */
	void setFieldState(int x, int y, State newState) {fields[x][y].setState(newState);}

	/**
	 * Toggles wether the snake is counted as an active field or not
	 */
	void toggleVitalSnake() {vitalSnake = !vitalSnake;}

	/**
	 * Constructs a board with a given field size, the count of cells in each dimension, and a given screen size, the size of the board on the screen
	 */
	Board(int fieldSize, float screenSize);
	~Board();

private:
	/**
	 * Initializes the board andthe drawBuffer
	 */
	void init();

	std::shared_ptr<sf::RenderTexture> 		drawBuffer;		// A RenderTexture so it's not necessary to render each field each frame
	bool 									fullRepaint;	// Should the drawBuffer be completely repainted this frame?
	float 									screenSize;		// The size of the board on screen, also the size of the drawBuffer

	bool 									paused;			// Is the game paused
	bool									vitalSnake;		// Is the snake counted as an occupied cell

	std::vector< std::vector<Field> > 		fields;			// The current state of each field
	int 									fieldSize;		// The fieldsize, stored extra for easier access
};

#endif /* BOARD_H_ */
