// Particle Filtering for 2D Robot
// Josh Villbrandt
// Oct 19, 2009

#include "State.H"

State::State()
{
	x = 0;
	y = 0;
	angle = 0;
}

State::State(const State &state)
{
	x = state.x;
	y = state.y;
	angle = state.angle;
}

State::State(double _x, double _y, double _angle)
{
	x = _x;
	y = _y;
	angle = _angle;
}

State::~State()
{
}
