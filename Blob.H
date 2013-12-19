// Blob for Image Blob
// Josh Villbrandt
// Oct 5, 2009

#ifndef BLOB_H
#define BLOB_H

class Blob
{
private:
	
public:
	int mass, xSum, ySum, hSum, sSum, bSum;
	double xAvg, yAvg, hAvg, sAvg, bAvg;
	bool calculated;
	int name, partOfBlob;
	
	Blob();
	~Blob();
	void addPixel(int x, int y, int h, int s, int b);
	void merge(Blob b);
	void calculate();
	bool equal(Blob b, double blobXError, double blobYError, double blobHError, double blobSError, double blobBError);
};

#endif
