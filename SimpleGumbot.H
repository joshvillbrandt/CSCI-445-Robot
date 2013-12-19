
#ifndef SIMPLEGUMBOT_H
#define SIMPLEGUMBOT_H

#include <vector>
#include <gumbot/Gumbot.H>
#include "Simulation.H"
#include "Map.H"
#include "State.H"
#include "Controller.H"
#include "Room.H"
#include "Segmenter.H"

class SimpleGumbot : public Gumbot
{
public:
  SimpleGumbot();
  ~SimpleGumbot();
  
  void goToState( State goalState );
  void pickUpObject();
  void putDownObject();
  Room::Object identifyObject();
  double getSonarReading( double direction );
  int getCompassReading();
  bool turnDegrees( int degrees );
  bool moveFeet( double feet );

private:
  Gumbot gumbot;
  Map map;
  Simulation simulation;
  Segmenter personSegmenter, bombSegmenter;
  Controller compassController;
  
  void waitForServos();
};

#endif
