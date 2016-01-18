/*
 * Field.h
 *
 *  Created on: Jan 14, 2016
 *      Author: sph3re
 */

#ifndef FIELD_H_
#define FIELD_H_

/**
 * The possible states of a field are empty and occupied
 */
enum State {
	EMPTY,
	OCCUPIED,
	SNAKE
};

class Field {
public:
	/**
	 * Applies the game rules based on the number of surrounding occupied fields
	 */
	void update(int count);

	Field();
	/**
	 * The copy constructor resets the changed state
	 */
	Field(const Field& rhs);
	~Field();

	/**
	 * Returns wether the field's state has changed since the last reset
	 * Fields changes will be recorded automatically by setter methods
	 */
	bool hasChanged() const {
		return changed;
	}

	/**
	 * Reset the changed state
	 */
	void resetChanged() {
		changed = false;
	}

	/**
	 * Gets the field's occupation state
	 */
	State getState() const {
		return state;
	}

	/**
	 * Sets the field's occupation state
	 * If the new state is different from the last state, also sets the changed flag
	 */
	void setState(State state) {
		if(!changed)
			changed = (state != this->state);
		this->state = state;
	}

	/**
	 * Toggles the occupation state, so if it was unoccupied it is now occupied and vice versa
	 */
	void toggleState() {
		if(state == EMPTY)
			state = OCCUPIED;
		else
			state = EMPTY;
		changed = true;
	}

private:
	State 	state;		// The state of the field
	bool 	changed;	// Changed-flag
};

#endif /* FIELD_H_ */
