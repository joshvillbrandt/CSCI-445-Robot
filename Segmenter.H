// Image Segmenter for Robots
// Josh Villbrandt
// Sep 28, 2009

#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Image/Image.H"
#include "Image/PixelsTypes.H"
#include "Blob.H"

class Segmenter
{
private:
	std::string fileName;
	double hueValue, saturationValue, brightnessValue;
	double hueError, saturationError, brightnessError;
	double blobXError, blobYError;
	double blobHueError, blobSaturationError, blobBrightnessError;
	int imageWidth, imageHeight;
	//Image< PixHSV<byte> > inImage;
	Image<int> outImage;
	std::vector<Blob> blobs, lastBlobs;
	int lastBlobName;
	bool loadFile();
	bool saveFile();
	void addToBlob(int x, int y);
	void overwriteBlobName(int oldName, int newName, int x, int y);
	
public:
	Segmenter(); // this should not be used
	Segmenter(std::string fileName);
	~Segmenter();
	
	Image<int> segmentImage(Image< PixHSV<byte> > inImage);
	std::vector<Blob> getBlobs();
	
	double getParameter(std::string name);
	bool setParameter(std::string name, double value);
	bool setParameter(std::string name, double value, bool autoSave);
};

#endif
