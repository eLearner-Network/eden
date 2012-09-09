/*
 *  ofxTracking.h
 *
 *  Created by Ramsin Khoshabeh on 5/4/08.
 *  Copyright 2008 risenparadigm. All rights reserved.
 *
 * Changelog:
 * 08/15/08 -- Fixed a major bug in the track algorithm
 */

#ifndef OFXTRACKING
#define OFXTRACKING

#include <list>
#include <map>

#include "ofxContourFinder.h"

class ofxTracking {
public:
	ofxTracking();
	~ofxTracking();
	
	//assigns IDs to each blob in the contourFinder
	void track(ofxContourFinder* newBlobs);

	int MOVEMENT_FILTERING;
	std::vector<ofxBlob> getTrackedBlobs();
	std::vector<ofxBlob> getTrackedFingers();

private:
	int trackKnn(ofxContourFinder *newBlobs, ofxBlob *track, int k, double thresh, bool fingers);
	std::vector<ofxBlob> trackedBlobs;		//tracked blobs
	std::vector<ofxBlob> trackedFingers;	//tracked Fingers
	
	int	IDCounter, numEnter,numLeave, size;
};

#endif
