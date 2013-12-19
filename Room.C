// Room for Squares
// Josh Villbrandt
// Nov 6, 2009

#include "Room.H"

Room::Room(State entrance, Object contents)
{
	this->entrance = entrance;
	this->contents = contents;
}

Room::~Room()
{
}
