/*
 * RLEParser.cpp
 *
 *  Created on: Jan 18, 2016
 *      Author: sph3re
 */

#include "RLEParser.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <sstream>

bool RLEParser::parseFile(std::string filename, Board& board)
{
	// Open the file
	std::ifstream file;
	file.open(filename, std::ios::in);
	if(!file.is_open())
		return false;

	// Store the field information in a buf
	std::stringstream buf;

	// Parse the file line for line
	// Parse the field information later, as we can use geline with another delimiter then
	std::string line;
	std::regex rgx(".*x = (\\d+), y = (\\d+).*");
	std::smatch match;
	bool hadConfigParams = false;
	while(std::getline(file, line))
	{
		// Ignore comments and empty lines
		if(line.length() == 0)
			continue;
		else if(line[0] == '#')
			continue;
		// Try to parse the header (there should be only one header so avoid the regex comparison upon later lines)
		else if(!hadConfigParams && std::regex_search(line, match, rgx))
		{
			hadConfigParams = true;
			int xsize = std::stoi(match[1]);
			int ysize = std::stoi(match[2]);
			// If the board is too small or way too big, adjust it's size
			if(board.getFieldSize() < std::max(xsize, ysize) || board.getFieldSize() > 5*std::max(xsize, ysize)+70)
				board.setFieldSize(std::max(xsize, ysize));
		}
		// Safe the other lines into a buffer for later parsing
		else
		{
			unsigned int pos = -1;
			if((pos = line.find('!')) == std::string::npos)
			{
				buf << line.substr(0, pos);
				break;
			}
			else
				buf << line;
		}
	}

	// The board will be overwritten in the area of the pattern anyways, so clear it
	board.clear();

	int x = 0, y = 0;
	// The delimiter of a line in .rle is the dollar sign, so loop through the lines according to that
	while(std::getline(buf, line, '$'))
	{
		x = 0;

		std::string numberbuf = "";

		// Loop through the line and parse the objects in there
		for(char c : line)
		{
			// If there is a number, store it in the numberbuf
			if(c >= '0' && c <= '9')
				numberbuf += c;
			// Otherwise, turn the numberbuf to an int and repeat the char that many times
			else
			{
				int repeat = 1;
				if(!numberbuf.empty())
				{
					repeat = std::stoi(numberbuf);
					numberbuf = "";
				}

				// o means the cell shall be occupied
				if(c == 'o')
				{
					for(int i = 0; i < repeat; i++)
					{
						if(x < board.getFieldSize() && y < board.getFieldSize())
							board.setFieldState(x, y, State::OCCUPIED);
						x++;
					}
				}
				// b means blank cell
				else if(c == 'b')
				{
					for(int i = 0; i < repeat; i++)
					{
						if(x < board.getFieldSize() && y < board.getFieldSize())
							board.setFieldState(x, y, State::EMPTY);
						x++;
					}
				}
			}
		}

		y++;
	}

	return true;
}

