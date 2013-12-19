// Particle Filtering for 2D Robot
// Josh Villbrandt
// Oct 19, 2009

#include "Particle.H"

Particle::Particle(State _state, double _probability)
{
	state = _state;
	probability = _probability;
}

Particle::~Particle()
{
}
