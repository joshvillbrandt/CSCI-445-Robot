// PID Controller for Robots
// Josh Villbrandt
// Sep 19, 2009

#include "Controller.H"

Controller::Controller()
{
}

Controller::Controller(std::string fileName)
{
	proportionalGain = 1;
	derivativeGain = 1;
	integralGain = 1;
	integralMax = 0;
	integralMin = 0;
	desiredValue = 0;
	previousError = 0;
	errorIntegral = 0;
	
	this->fileName = fileName;
	loadFile();
}

Controller::~Controller()
{
}

bool Controller::loadFile() {
	std::ifstream file;
	file.open(fileName.c_str());
	
	if(file) {
		while(!file.eof()) {
			std::string line;
			getline(file, line);
			
			if(line != "") {
				std::istringstream lineStream(line);
				std::string name;
				double value;
				lineStream >> name >> value;
				
				setParameter(name, value, false);
			}
		}
		
		file.close();
		return true;
	}
	else return false;
}

bool Controller::saveFile() {
	std::ofstream file;
	file.open(fileName.c_str());
	
	if(file) {
		file << "proportionalGain " << proportionalGain << std::endl;
		file << "derivativeGain " << derivativeGain << std::endl;
		file << "integralGain " << integralGain << std::endl;
		file << "integralMax " << integralMax << std::endl;
		file << "integralMin " << integralMin << std::endl;
		file << "desiredValue " << desiredValue << std::endl;
		
		file.close();
		return true;
	}
	else return false;
}

double Controller::update(double currentValue) {
	double period = 1; // could update this to be a real time
	
	// establish error components
	double currentError = desiredValue - currentValue;
	double errorDerivative = (currentError - previousError) / period;
	errorIntegral += currentError * period;
	
	// Check Integral Bounds
	if(errorIntegral > integralMax) errorIntegral = integralMax;
	else if(errorIntegral < integralMin) errorIntegral = integralMin;
	
	// Store this error for later
	previousError = currentError;
	
	// return sum of components
	return currentError*proportionalGain + errorDerivative*derivativeGain + errorIntegral*integralGain;
}

double Controller::getParameter(std::string name) {
	if(name == "proportionalGain") return proportionalGain;
	else if(name == "derivativeGain") return derivativeGain;
	else if(name == "integralGain") return integralGain;
	else if(name == "integralMax") return integralMax;
	else if(name == "integralMin") return integralMin;
	else if(name == "desiredValue") return desiredValue;
	else if(name == "previousError") return previousError;
	else if(name == "errorIntegral") return errorIntegral;
	else return 0;
}

bool Controller::setParameter(std::string name, double value) {
	return setParameter(name, value, true);
}
	
bool Controller::setParameter(std::string name, double value, bool autoSave) {
	bool found = true;
	
	if(name == "proportionalGain") proportionalGain = value;
	else if(name == "derivativeGain") derivativeGain = value;
	else if(name == "integralGain") integralGain = value;
	else if(name == "integralMax") integralMax = value;
	else if(name == "integralMin") integralMin = value;
	else if(name == "desiredValue") desiredValue = value;
	else found = false;
	
	if(found && autoSave) saveFile();
	
	return found;
}
