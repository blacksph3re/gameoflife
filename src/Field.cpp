/*
 * Field.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: sph3re
 */

#include "Field.h"


Field::Field()
	: state(EMPTY),
	changed(true)
{}

Field::Field(const Field& rhs)
	: state(rhs.state),
	  changed(false)
{}

Field::~Field() {}


void Field::update(int count)
{
	// If 1 or 0 token surround this one, the cell will be dead next frame
	if(count < 2)
		setState(EMPTY);
	// If there are 2 surrounding token, do nothing, either the cell stays dead or it stays alive
	// If there are exactly 3 surrounding token, the cell will be alive next frame
	else if(count == 3)
		setState(OCCUPIED);
	// On more than 3, the cell will die by overpopulation
	else if(count > 3)
		setState(EMPTY);
}


