// Particle Filtering for 2D Robot
// Josh Villbrandt
// Oct 19, 2009

#include "Simulation.H"

Simulation::Simulation()
{
}

Simulation::Simulation(std::string fileName, Map map)
{
	numParticles = 0;
    pixlesPerLengthUnit = 1;
	straightDistanceStdDev = 0;
	straightAngleStdDev = 0;
	turnAngleStdDev = 0;
	sonarStdDev = 0;
	compassStdDev = 0;
	
	this->fileName = fileName;
	loadFile();
	
	this->map = map;
	initializeParticles(map.getStartingState());
}

Simulation::~Simulation()
{
}

bool Simulation::loadFile() {
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

bool Simulation::saveFile() {
	std::ofstream file;
	file.open(fileName.c_str());
	
	if(file) {
		file << "numParticles " << numParticles << std::endl;
		file << "pixlesPerLengthUnit " << pixlesPerLengthUnit << std::endl;
		file << "straightDistanceStdDev " << straightDistanceStdDev << std::endl;
		file << "straightAngleStdDev " << straightAngleStdDev << std::endl;
		file << "turnAngleStdDev " << turnAngleStdDev << std::endl;
		file << "sonarStdDev " << sonarStdDev << std::endl;
		file << "compassStdDev " << compassStdDev << std::endl;
		
		file.close();
		return true;
	}
	else return false;
}

double Simulation::getParameter(std::string name) {
	if(name == "numParticles") return numParticles;
	else if(name == "pixlesPerLengthUnit") return pixlesPerLengthUnit;
	else if(name == "straightDistanceStdDev") return straightDistanceStdDev;
	else if(name == "straightAngleStdDev") return straightAngleStdDev;
	else if(name == "turnAngleStdDev") return turnAngleStdDev;
	else if(name == "sonarStdDev") return sonarStdDev;
	else if(name == "compassStdDev") return compassStdDev;
	else return 0;
}

bool Simulation::setParameter(std::string name, double value) {
	return setParameter(name, value, true);
}
	
bool Simulation::setParameter(std::string name, double value, bool autoSave) {
	bool found = true;
	
	if(name == "numParticles") numParticles = value;
	else if(name == "pixlesPerLengthUnit") pixlesPerLengthUnit = value;
	else if(name == "straightDistanceStdDev") straightDistanceStdDev = value;
	else if(name == "straightAngleStdDev") straightAngleStdDev = value;
	else if(name == "turnAngleStdDev") turnAngleStdDev = value;
	else if(name == "sonarStdDev") sonarStdDev = value;
	else if(name == "compassStdDev") compassStdDev = value;
	else found = false;
	
	if(found && autoSave) saveFile();
	
	return found;
}

void Simulation::initializeParticles(State initialState) {
    Particle p(initialState, 1/numParticles);
    
    for(int i = 0; i < numParticles; i++) {
        particles.push_back(p);
    }
}

void Simulation::move(double distance, double measurement) {
    for(int i = 0; i < numParticles; i++) {
        // Random Values
        double dist = distance + randomDoubleFromNormal(straightDistanceStdDev);
        double angle = randomDoubleFromNormal(straightAngleStdDev);
        
        // Set Random Values
        particles[i].state.angle += angle;
        particles[i].state.x += dist*cos(particles[i].state.angle * DEGTORAD);
        particles[i].state.y += dist*sin(particles[i].state.angle * DEGTORAD);
        
        // Clean Angle
        particles[i].state.angle = modDouble(particles[i].state.angle, 360);
        
        // Probability
        double probReadingGivenLocation = (expectedDistance(particles[i].state) - measurement) * sonarStdDev;
        double probLocation = particles[i].probability;
        double probSensorReading = 1.0;
        
        // Set Particle Probability
        particles[i].probability = probReadingGivenLocation * probLocation / probSensorReading;
    }
}

void Simulation::turn(double angle, double measurement) {
    for(int i = 0; i < numParticles; i++) {
        double a = angle + randomDoubleFromNormal(turnAngleStdDev);
        
        particles[i].state.angle += a;
    }
}

void Simulation::resampleParticles() {

}

double Simulation::expectedDistance(State state) {
    double xOpposite = 0, yAdjacent = 0, xHypotenuse = 0, yHypotenuse = 0, hypotenuse = 0;
    
    // Build x component
    if(state.angle > 180) xOpposite = state.x;
    else xOpposite = map.getMap().getWidth() - state.x;
    xHypotenuse = xOpposite / sin(state.angle * DEGTORAD);
    
    // Build y component
    if(state.angle > 270 || state.angle < 90) yAdjacent = state.y;
    else yAdjacent = map.getMap().getHeight() - state.y;
    yHypotenuse = yAdjacent / cos(state.angle * DEGTORAD);
    
    if(yHypotenuse < xHypotenuse) hypotenuse = yHypotenuse;
    else hypotenuse = xHypotenuse;
    
    return hypotenuse;
}

double Simulation::modDouble(double number, int mod) {
    int intPart = (int)number;
    double doublePart = number - intPart;
    return (double)(doublePart + intPart%mod);
}

double Simulation::randomDouble() {
	return double(rand()) / (double(RAND_MAX) + 1.0);
}

double Simulation::randomDoubleFromNormal(const double s) {
	double sum = 0;
	for(int i=0; i<12; i++) {
		sum += randomDouble()*2*s - s;
	}
	return sum/2;
}

State Simulation::getAverageState() {
    double sumX = 0;
    double sumY = 0;
    double sumAngle = 0;
    
    for(int i = 0; i < numParticles; i++) {
        sumX += particles[i].state.x;
        sumY += particles[i].state.y;
        sumAngle += particles[i].state.angle;
    }
    
    State state(sumX/numParticles, sumY/numParticles, sumAngle/numParticles);
    return state;
}

State Simulation::getMostLikelyState() {
    double maxProb = 0;
    State state;
    
    for(int i = 0; i < numParticles; i++) {
        if(particles[i].probability > maxProb) {
            maxProb = particles[i].probability;
            state = particles[i].state;
        }
    }
    
    return state;
}

Image< PixRGB<byte> > Simulation::getImage() {
    // Different types of pixels
    PixRGB<byte> simDot, avgDot, probDot;
    simDot.set(255, 255, 0);
    avgDot.set(255, 0, 0);
    probDot.set(0, 255, 0);
    
    // Reset image to blank map
    image = map.getMap();
    
    // Draw Particles
    for(int i = 0; i < numParticles; i++) {
        plotDot((int)round(particles[i].state.x), (int)round(particles[i].state.y), simDot);
    }
    
    // Draw Average
    State avgState = getAverageState();
    image.setVal((int)round(avgState.x), (int)round(avgState.y), avgDot);
    
    // Draw Most Likely
    State probState = getMostLikelyState();
    image.setVal((int)round(probState.x), (int)round(probState.y), avgDot);
    
    return image;
}

void Simulation::plotDot(int x, int y, PixRGB<byte> dot) {
    if(x >= 0 && x < image.getWidth() && y >= 0 && y < image.getHeight())
        image.setVal(x, y, dot);
}
