// Particle Filtering for 2D Robot
// Josh Villbrandt
// Oct 19, 2009

#ifndef STATE_H
#define STATE_H

class State
{
private:
	
public:
	State();
	State(const State &state);
	State(double _x, double _y, double _angle);
	~State();
	
	double x, y, angle;
};

#endif
