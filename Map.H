// Map for Gumbot
// Josh Villbrandt
// Nov 6, 2009

#ifndef MAP_H
#define MAP_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <Image/Image.H>
#include <Image/PixelsTypes.H>
#include "State.H"

class Map
{
private:
	std::string fileName;
	Image< PixRGB<byte> > map;
	State startingState;
	
	bool loadMap();
	
public:
    Map(); // this should not be used
	Map(std::string fileName);
	~Map();
	
	// Search through the map, and create a vector of moves.
	// This vector will be valid as long as the robot successfully completes each move. If the robot
	// fail a move, it should recall this function.
	std::vector<State> getMoves(State currentState, State goalState);
	
	// Return starting state from the map
	State getStartingState();
	
	// Return the map
	Image< PixRGB<byte> > getMap();
};

#endif
