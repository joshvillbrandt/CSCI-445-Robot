// Particle Filtering for 2D Robot
// Josh Villbrandt
// Oct 19, 2009

#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <Image/Image.H>
#include <Image/PixelsTypes.H>
#include "State.H"
#include "Particle.H"
#include "Map.H"

#define DEGTORAD (2.0 * M_PI / 360.0)

class Simulation
{
private:
	std::string fileName;
	double numParticles, pixlesPerLengthUnit;
	double straightDistanceStdDev, straightAngleStdDev, turnAngleStdDev;
	double sonarStdDev, compassStdDev;
	std::vector<Particle> particles;
	Image< PixRGB<byte> > image;
	Map map;
	
	bool loadFile();
	bool saveFile();
	
	double expectedDistance(State state);
	double randomDouble();
	double randomDoubleFromNormal(const double s);
	void initializeParticles(State initialState);
	void resampleParticles();
	void plotDot(int x, int y, PixRGB<byte> dot);
	double modDouble(double number, int mod);
	
public:
    Simulation(); // this should not be used
	Simulation(std::string fileName, Map map);
	~Simulation();
	double getParameter(std::string name);
	bool setParameter(std::string name, double value);
	bool setParameter(std::string name, double value, bool autoSave);
	
	void move(double distance, double measurement);
	void turn(double angle, double measurement);
	State getAverageState();
	State getMostLikelyState();
	Image< PixRGB<byte> > getImage();
};

#endif
