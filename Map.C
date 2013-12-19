// Map for Gumbot
// Josh Villbrandt
// Nov 6, 2009

#include "Map.H"

Map::Map()
{
}

Map::Map(std::string fileName)
{
	this->fileName = fileName;
	loadMap();
}

Map::~Map()
{
}

bool Map::loadMap() {
    // Open File
	std::ifstream file;
	file.open(fileName.c_str());
	
	// Prepare wall marker
	PixRGB<byte> wallDot;
    wallDot.set(255, 255, 255);
    int width, height;
    
    // Prepare starting State
    startingState = State(0, 0, 0);
	
	if(file) {
	    // Get first line of file, create blank canvas
	    if(!file.eof()) {
			std::string line;
			getline(file, line);
            std::istringstream lineStream(line);
            lineStream >> width >> height;
            
            map = Image< PixRGB<byte> >(width, height, ZEROS);
	    }
	    
	    // Get the meat of the map, check for starting locations
	    for(int y = 0; y < height && !file.eof(); y++) {
			std::string line;
			getline(file, line);
			
	        for(int x = 0; x < line.size() && x < width; x++) {
	            if(line[x] == '*') map.setVal(x, y, wallDot);
	            else if(line[x] == 'n') {
	                startingState.x = x;
	                startingState.y = y;
	                startingState.angle = 0;
	            }
	            else if(line[x] == 'e') {
	                startingState.x = x;
	                startingState.y = y;
	                startingState.angle = 90;
	            }
	            else if(line[x] == 's') {
	                startingState.x = x;
	                startingState.y = y;
	                startingState.angle = 180;
	            }
	            else if(line[x] == 'w') {
	                startingState.x = x;
	                startingState.y = y;
	                startingState.angle = 270;
	            }
	        }
	    }
		
		// Close File
		file.close();
		
		return true;
	}
	else return false;
}

std::vector<State> Map::getMoves(State currentState, State goalState) {
	std::vector<State> moves;
	moves.push_back(State(0, 0, 0));
	
	// todo
	
	return moves;
}

State Map::getStartingState() {
	return startingState;
}

Image< PixRGB<byte> > Map::getMap() {
	return map;
}
