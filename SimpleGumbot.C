// ---------------------------------------------------------------------
#include "SimpleGumbot.H"
#include <iostream> // Used for debugging only.

#define RIGHT_MOTOR_PIN 0
#define LEFT_MOTOR_PIN  1
#define SONAR_PIN       0
#define SONAR_SERVO_PIN 1
#define COMPASS_PIN     2
#define DISPLAY_IMAGE   true
#define DISPLAY_PORT    10002
#define RADTODEG        (180.0 / M_PI)

#define DEBUG_MOVEMENT( x ) x  // Define to ; to disable or x to enable.
#define DEBUG_SONAR( x )    x  // Define to ; to disable ot x to enable.

// ---------------------------------------------------------------------
SimpleGumbot::SimpleGumbot()
{
  // Set up the camera.
  #ifdef DISPLAY_IMAGE
    gumbot.openDisplayServerConnection( DISPLAY_PORT );
  #endif

  gumbot.startCamera();
    
  // Set up other junk.
  map = Map( "map.config" );
  simulation = Simulation( "simulation.config", map );
  personSegmenter = Segmenter( "personSegmenter.config" );
  bombSegmenter = Segmenter( "bombSegmenter.config" );
  compassController = Controller( "compassController.config" );
  
  #ifdef DISPLAY_IMAGE
    // Output initial map.
    gumbot.displayImage( simulation.getImage() );
  #endif
}

// ---------------------------------------------------------------------
void SimpleGumbot::goToStateInStraightLine( State currentState, State goalState )
{
  // Turn towards the next position.
  double moveAngle = RADTODEG * atan( (goalState.x - currentState.x) / (goalState.y - currentState.y) );
  turnDegrees( moveAngle );

  // Update the particles' angles.
  simulation.turnSmarticles( moveAngle );

  // Go forward.
  double moveDistance = sqrt( pow( goalState.x - currentState.x, 2 ) + pow( goalState.y - currentState.y, 2 ) ) / 24;
  moveFeet( moveDistance );

  // Update the particles' positions.
  simulation.moveSmarticles( moveDistance );
}

// ---------------------------------------------------------------------
void SimpleGumbot::goToState( State goalState ) 
{
  // Get current state based on where we think we are.
  State currentState = simulation.getMostLikelyState();

  // Get a path to where we want to go based on our current state.
  vector<State> path = map.getPath( currentState, goalState );
    
  // Follow the path.
  //  NOTE: ignores all state angles.
  for( int i = 0; i < path.size(); ++i )
    goToStateInStraightLine( currentState, goalState );
    
  // Turn to final angle.
  turn( goalState.angle );
    
  #ifdef DISPLAY_IMAGE
    // Output initial map.
    gumbot.displayImage( simulation.getImage() );
  #endif
}

// ---------------------------------------------------------------------
void SimpleGumbot::pickUpObject() 
{
  // todo
}

// ---------------------------------------------------------------------
void SimpleGumbot::putDownObject() 
{
  // todo
}

// ---------------------------------------------------------------------
Room::Object SimpleGumbot::identifyObject() 
{
  // get biggest blob from each segmenter, find out which is bigger
}

// =====================================================================
// ========================== Motor Functions ==========================
// =====================================================================
// Returns the average of 10 compass readings.
// Use this instead of just calling gumbot.getCompass() because the compass often returns bogus readings.
int SimpleGumbot::getCompassReading()
{
  int cSum = 0;
  for( int i = 0; i < 10; ++i )
    cSum += gumbot.getCompass( COMPASS_PIN );

  return cSum / 10;
}

// ---------------------------------------------------------------------
bool SimpleGumbot::turnDegrees( int degrees )
{
  DEBUG_MOVEMENT( cout << "Turning " << degrees << " degrees\n"; )
  // assert( direction == LEFT || direction == RIGHT );

  int currentHeading = getCompassReading( gumbot );
  int desiredHeading;

  // Set the motors and calculate desiredHeading based on direction.
  //  NOTE: If we want to go left, subtract degrees from currentHeading instead of adding.
  desiredHeading = currentHeading + degrees;
  if( desiredHeading < 0 )
    desiredHeading += 360;
  else if( desiredHeading > 360 )
    desiredHeading -= 360;

  // Keep turning until we are at/near enough to our desiredHeading.
  #define COMPASS_THRESHOLD 5
  if( degrees < 0 )
  {
    gumbot.setMotor( RIGHT_MOTOR_PIN, RIGHT_MOTOR_FORWARD );
    gumbot.setMotor( LEFT_MOTOR_PIN,  LEFT_MOTOR_BACKWARD );
    while( getCompassReading() >= desiredHeading + COMPASS_THRESHOLD ) {}
  }
  else
  {
    gumbot.setMotor( RIGHT_MOTOR_PIN, RIGHT_MOTOR_BACKWARD );
    gumbot.setMotor( LEFT_MOTOR_PIN,  LEFT_MOTOR_FORWARD );
    while( getCompassReading() <= desiredHeading - COMPASS_THRESHOLD ) {}
  }

  // We should now be facing the desiredHeading; stop the motors.
  gumbot.setMotor( RIGHT_MOTOR_PIN, MOTOR_STOP );
  gumbot.setMotor( LEFT_MOTOR_PIN,  MOTOR_STOP );

  return true;
}

// ---------------------------------------------------------------------
bool SimpleGumbot::moveFeet( double feet )
{
  DEBUG_MOVEMENT( cout << "Moving " << feet << " feet\n"; )
  // assert( direction == FORWARD || direction == BACKWARD ); 

  if( feet < 0 )
  {
    // Set the motors backward.
    gumbot.setMotor( RIGHT_MOTOR_PIN, RIGHT_MOTOR_BACKWARD );
    gumbot.setMotor( LEFT_MOTOR_PIN,  LEFT_MOTOR_BACKWARD );
  }
  else
  {
    // Set the motors forward.
    gumbot.setMotor( RIGHT_MOTOR_PIN, RIGHT_MOTOR_FORWARD );
    gumbot.setMotor( LEFT_MOTOR_PIN,  LEFT_MOTOR_FORWARD );
  }  
  return false;
  
  // Wait for the robot to drive distance in feet. 
  usleep( MOVE_ONE_FOOT_DELAY * int( feet ) );

  // We should now have moved the desired distance; stop the motors.
  gumbot.setMotor( RIGHT_MOTOR_PIN, MOTOR_STOP );
  gumbot.setMotor( LEFT_MOTOR_PIN,  MOTOR_STOP );

  return true;
}

// =====================================================================
// ========================== Servo Functions ==========================
// =====================================================================
// Call this after setting a servo to make sure it gets to where you want it to be.
void SimpleGumbot::waitForServos()
{
  // usleep( SERVO_TURN_DELAY );
  sleep( 1 );
}

// ---------------------------------------------------------------------
// Similar to getAdjustedCompassReading.
// Use this instead of just calling gumbot.getSonar() to prevent bogus readings.
int SimpleGumbot::getSonarReading( double direction )
{
  // Point the sonar in direction
  gumbot.setServo( SONAR_SERVO_PIN, direction );
  waitForServos();
  
  int tmp = gumbot.getSonar( SONAR_PIN );
  int smallestReading = tmp;
  for( int i = 0; i < 10; ++i )  
  {
    tmp = gumbot.getSonar( SONAR_PIN );     
    if( smallestReading > tmp )
      smallestReading = tmp;
  }

  DEBUG_SONAR( cout << "Sonar Reading (Feet): " << smallestReading * CM_TO_FEET << endl; )
  return smallestReading;
}
