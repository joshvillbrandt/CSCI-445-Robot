// Particle Filtering for 2D Robot
// Josh Villbrandt
// Oct 19, 2009

#ifndef PARTICLE_H
#define PARTICLE_H

#include "State.H"

class Particle
{
private:
	
public:
	Particle(State _state, double _probability);
	~Particle();
	
	State state;
	double probability;
};

#endif
