/*
 * HelpText.h
 *
 *  Created on: Jan 15, 2016
 *      Author: sph3re
 */

#ifndef HELPTEXT_H_
#define HELPTEXT_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include "Board.h"
#include "Snake.h"

class HelpText {
public:
	/**
	 * Update the help text based on the mouse position
	 * Also place the texts based on the given offset
	 */
	void update(float xOffset, sf::Vector2f mousePos);

	/**
	 * Render the texts on the target
	 */
	void render(sf::RenderTarget& target);

	/**
	 * Initialize the help text
	 * The board reference is needed to bind the functionality to their texts
	 */
	bool init(Board& board, Snake& snake);

	/**
	 * Process a mouseclick on window coordinates x and y
	 */
	void clicked(int x, int y);


	HelpText();
	~HelpText();
private:
	sf::Font 													defaultFont;	// Fontfile for rendering the texts
	std::vector< std::pair<sf::Text, std::function<void()> > > 	textToRender; 	// A vector with the text and the corresponding functionality

};

#endif /* HELPTEXT_H_ */
