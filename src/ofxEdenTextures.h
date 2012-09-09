/*
 *  ofxEdenTextures.h
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#ifndef OFXEDENTEXTURES
#define OFXEDENTEXTURES

#define LIFEGLOW

#include "ofMain.h"
#include "ofxEdenSphere.h"
#include "ofxEdenGeosphere.h"
#include "ofxEdenBiosphere.h"

#include "ofxWater.h"

class ofxEdenTextures : public ofxEdenSphere {
public:
	ofxEdenTextures();
	
	ofxEdenTextures& allocate(int _width, int _height);
	
	void update(ofTexture& hydroText, ofxEdenBiosphere& bio, ofxEdenGeosphere& geo);
	
    ofxWater    water;
private:
    
	ofImage		snow;
	ofImage		ground[2];
	ofImage		rock[2];
	ofImage		vegetation[3];
};

#endif
