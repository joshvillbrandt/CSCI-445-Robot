// Image Segmenter for Robots
// Josh Villbrandt
// Sep 28, 2009

#include <vector>
#include <Image/CutPaste.H>
#include "State.H"
#include "Map.H"
#include "Simulation.H"
#include "SimpleGumbot.H"
#include "Room.H"
#include "Blob.H"
#include <iostream> // used for debugging only

int main( int argc, char **argv )
{
    // Gumbot setup
	SimpleGumbot gumbot;
    
    // Situation setup
    State buildingEntrance( 144, 288, 0 );
    State buildingExit( 144, 288, 180 );
    State survivorDropOff( 48, 336, 270 );
    int bombRoomNumber = 0;
    
    // Room setup
    std::cout << "Initializing rooms" << std::endl;
    std::vector<Room> rooms;
    rooms.push_back( Room( State( 96, 240, 270 ), Room::UNKNOWN ) );
    rooms.push_back( Room( State( 96, 144, 270 ), Room::UNKNOWN ) );
    rooms.push_back( Room( State( 96, 48,  270 ), Room::UNKNOWN ) );
    
    // Search all rooms
    std::cout << "Searching rooms" << std::endl;
    for( int i = 0; i < rooms.size(); i++ )
    {
      gumbot.goToState( rooms.at( i ).entrance );
      rooms.at( i ).contents = gumbot.identifyObject();
        
      // Remove person from room
      if( rooms.at( i ).contents == Room::PERSON ) 
      {
	gumbot.pickUpObject();
	gumbot.goToState( survivorDropOff );
	gumbot.putDownObject();
	gumbot.goToState( buildingEntrance );
      }
      else if( rooms.at( i ).contents == Room::BOMB ) 
	bombRoomNumber = i;
    }
    
    // Move the bomb away from the building entrance
    std::cout << "Removing bomb" << std::endl;
    if( bombRoomNumber != rooms.size() - 1 ) 
    {
      gumbot.goToState( rooms.at( bombRoomNumber ).entrance );
      gumbot.pickUpObject();
      gumbot.goToState( rooms.at( rooms.size() - 1 ).entrance );
      gumbot.putDownObject();
    }
    gumbot.goToState( buildingExit );
}
