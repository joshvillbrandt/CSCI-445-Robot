// Blob for Image Blob
// Josh Villbrandt
// Oct 5, 2009

#include "Blob.H"

Blob::Blob()
{
	mass = 0;
	xSum = 0;
	ySum = 0;
	hSum = 0;
	sSum = 0;
	bSum = 0;
	calculated = false;
	name = -1;
	partOfBlob = -1;
}

Blob::~Blob()
{
}

void Blob::addPixel(int x, int y, int h, int s, int b) {
	xSum += x;
	ySum += y;
	hSum += h;
	sSum += s;
	bSum += b;
	mass++;
	calculated = false;
}

void Blob::merge(Blob b) {
	xSum += b.xSum;
	ySum += b.ySum;
	hSum += b.hSum;
	sSum += b.sSum;
	bSum += b.bSum;
	calculated = false;
}

void Blob::calculate() {
	xAvg = xSum/mass;
	yAvg = ySum/mass;
	hAvg = hSum/mass;
	sAvg = sSum/mass;
	bAvg = bSum/mass;
	calculated = true;
}

bool Blob::equal(Blob b, double blobXError, double blobYError, double blobHError, double blobSError, double blobBError) {
	bool xEqual = false;
	bool yEqual = false;
	bool hEqual = false;
	bool sEqual = false;
	bool bEqual = false;
	
	if(!calculated) calculate();
	if(!b.calculated) b.calculate();
	
	if(xAvg >= (b.xAvg - blobXError) && xAvg <= (b.xAvg + blobXError)) xEqual = true;
	if(yAvg >= (b.yAvg - blobYError) && yAvg <= (b.yAvg + blobYError)) yEqual = true;
	if(hAvg >= (b.hAvg - blobHError) && hAvg <= (b.hAvg + blobHError)) hEqual = true;
	if(sAvg >= (b.sAvg - blobSError) && sAvg <= (b.sAvg + blobSError)) sEqual = true;
	if(bAvg >= (b.bAvg - blobBError) && bAvg <= (b.bAvg + blobBError)) bEqual = true;
	
	if(xEqual && yEqual && hEqual && sEqual && bEqual) return true;
	else return false;
}
