// Room for Squares
// Josh Villbrandt
// Nov 6, 2009

#ifndef ROOM_H
#define ROOM_H

#include "State.H"

class Room
{
private:
	
public:
    enum Object {UNKNOWN, EMPTY, PERSON, BOMB};
    
	State entrance;
	Object contents;
	
	Room(State entrance, Object contents);
	~Room();
};

#endif
