/*
 * RLEParser.h
 *
 *  Created on: Jan 18, 2016
 *      Author: sph3re
 */

#ifndef RLEPARSER_H_
#define RLEPARSER_H_
#include <string>
#include "Board.h"

class RLEParser {
public:

	static bool parseFile(std::string filename, Board& board);

private:
	RLEParser() {}
	RLEParser(const RLEParser& rhs) {}
	~RLEParser() {}
};

#endif /* RLEPARSER_H_ */
