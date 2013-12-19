// Image Segmenter for Robots
// Josh Villbrandt
// Sep 28, 2009

#include "Segmenter.H"

Segmenter::Segmenter()
{
}

Segmenter::Segmenter(std::string fileName)
{
	hueValue = 0;
	saturationValue = 0;
	brightnessValue = 0;
	hueError = 0;
	saturationError = 0;
	brightnessError = 0;
	blobXError = 0;
	blobYError = 0;
	blobHueError = 0;
	blobSaturationError = 0;
	blobBrightnessError = 0;
	lastBlobName = 0;
	
	this->fileName = fileName;
	loadFile();
}

Segmenter::~Segmenter()
{
}

bool Segmenter::loadFile() {
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

bool Segmenter::saveFile() {
	std::ofstream file;
	file.open(fileName.c_str());
	
	if(file) {
		file << "hueValue " << hueValue << std::endl;
		file << "saturationValue " << saturationValue << std::endl;
		file << "brightnessValue " << brightnessValue << std::endl;
		file << "hueError " << hueError << std::endl;
		file << "saturationError " << saturationError << std::endl;
		file << "brightnessError " << brightnessError << std::endl;
		file << "blobXError " << blobXError << std::endl;
		file << "blobYError " << blobYError << std::endl;
		file << "blobHueError " << blobHueError << std::endl;
		file << "blobSaturationError " << blobSaturationError << std::endl;
		file << "blobBrightnessError " << blobBrightnessError << std::endl;
		
		file.close();
		return true;
	}
	else return false;
}

Image<int> Segmenter::segmentImage(Image< PixHSV<byte> > inImage) {
    // Save last blobs for comparison
    lastBlobs = blobs;
    
    // Prepare the new segmented image
    //this.inImage = inImage;
    imageWidth = inImage.getWidth();
    imageHeight = inImage.getHeight();
    Image<int> newImage(imageWidth, imageHeight, ZEROS);
    outImage = newImage;
    
    // Segment image, create blobs
    for(int i = 0; i < imageWidth; i++) {
        for(int j = 0; j < imageHeight; j++) {
            bool hEqual = false;
            bool sEqual = false;
            bool bEqual = false;
            
            if(inImage.getVal(i, j).H() >= (hueValue - hueError) && inImage.getVal(i, j).H() <= (hueValue + hueError))
                hEqual = true;
            if(inImage.getVal(i, j).S() >= (saturationValue - saturationError) && inImage.getVal(i, j).S() <= (saturationValue + saturationError))
                sEqual = true;
            if(inImage.getVal(i, j).V() >= (brightnessValue - brightnessError) && inImage.getVal(i, j).V() <= (brightnessValue + brightnessError))
                bEqual = true;
            
            if(hEqual && sEqual && bEqual) {
                outImage.setVal(i, j, 1);
                addToBlob(i, j);
            }
        }
    }
    
    // Compare blobs
    return outImage;
}

void Segmenter::addToBlob(int x, int y) {
    // Check adjacent cells for group, find equal blobs
    std::vector<int> adjacentBlobs;
    
    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; j++) {
            if(i >= 0 && i < imageWidth && j >= 0 && j < imageHeight) {
                if(outImage.getVal(i, j) != 0) {
                    adjacentBlobs.push_back(outImage.getVal(i, j));
                }
            }
        }
    }
    
    if(adjacentBlobs.size() == 0) {
        lastBlobName++;
        outImage.setVal(x, y, lastBlobName);
    }
    else if(adjacentBlobs.size() == 1) {
        outImage.setVal(x, y, adjacentBlobs.at(0));
    }
    else {
        int masterBlob = adjacentBlobs.at(0);
        outImage.setVal(x, y, masterBlob);
        
        // Overwrite connecting blobs
        for(int i = 1; i < adjacentBlobs.size(); i++) {
            overwriteBlobName(adjacentBlobs.at(i), masterBlob, x, y);
        }
    }
}

void Segmenter::overwriteBlobName(int oldName, int newName, int x, int y) {
    for(int i = 0; i < imageWidth; i++) {
        for(int j = 0; j <= y; j++) {
            if(outImage.getVal(i, j) == oldName)
                outImage.setVal(i, j, newName);
        }
    }
}

std::vector<Blob> Segmenter::getBlobs() {
    return blobs;
}

double Segmenter::getParameter(std::string name) {
	if(name == "hueValue") return hueValue;
	else if(name == "saturationValue") return saturationValue;
	else if(name == "brightnessValue") return brightnessValue;
	else if(name == "hueError") return hueError;
	else if(name == "saturationError") return saturationError;
	else if(name == "brightnessError") return brightnessError;
	else if(name == "blobXError") return blobXError;
	else if(name == "blobYError") return blobYError;
	else if(name == "blobHueError") return blobHueError;
	else if(name == "blobSaturationError") return blobSaturationError;
	else if(name == "blobBrightnessError") return blobBrightnessError;
	else return 0;
}

bool Segmenter::setParameter(std::string name, double value) {
	return setParameter(name, value, true);
}
	
bool Segmenter::setParameter(std::string name, double value, bool autoSave) {
	bool found = true;
	
	if(name == "hueValue") hueValue = value;
	else if(name == "saturationValue") saturationValue = value;
	else if(name == "brightnessValue") brightnessValue = value;
	else if(name == "hueError") hueError = value;
	else if(name == "saturationError") saturationError = value;
	else if(name == "brightnessError") brightnessError = value;
	else if(name == "blobXError") blobXError = value;
	else if(name == "blobYError") blobYError = value;
	else if(name == "blobHueError") blobHueError = value;
	else if(name == "blobSaturationError") blobSaturationError = value;
	else if(name == "blobBrightnessError") blobBrightnessError = value;
	else found = false;
	
	if(found && autoSave) saveFile();
	
	return found;
}
