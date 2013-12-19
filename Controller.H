// PID Controller for Robots
// Josh Villbrandt
// Sep 19, 2009

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <fstream>
#include <sstream>

class Controller
{
private:
	std::string fileName;
	double proportionalGain, derivativeGain, integralGain;
	double integralMax, integralMin, desiredValue;
	double previousError, errorIntegral;
	bool loadFile();
	bool saveFile();
	
public:
    Controller(); // this should not be used
	Controller(std::string fileName);
	~Controller();
	double update(double currentValue);
	double getParameter(std::string name);
	bool setParameter(std::string name, double value);
	bool setParameter(std::string name, double value, bool autoSave);
};

#endif
