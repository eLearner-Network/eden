/*
 *  ofxEdenBiosphere.h
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */
#ifndef OFXEDENBIOSPHERE
#define OFXEDENBIOSPHERE

#define LIFEGLOW

#include "ofMain.h"
#include "ofxEdenSphere.h"
#include "ofxEdenGeosphere.h"
#include "ofxFlocking.h"

class ofxEdenBiosphere : public ofxEdenSphere {
public:
	ofxEdenBiosphere();
	
	ofxEdenBiosphere& allocate(int _width, int _height);
	
	void update(ofTexture& hydroText, ofxEdenGeosphere& geo);
	
	ofTexture&	getAnimalLife(){ return flock.getTexture(); };
	
    ofxFlocking flock;
};

#endif
