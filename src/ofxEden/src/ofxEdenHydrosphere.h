/*
 *  ofxEdenHydrosphere.h
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */
#ifndef OFXEDENHYDROSPHERE
#define OFXEDENHYDROSPHERE

#include "ofMain.h"
#include "ofxEdenSphere.h"
#include "ofxEdenGeosphere.h"

class ofxEdenHydrosphere : public ofxEdenSphere {
public:
	ofxEdenHydrosphere();
	
	ofxEdenHydrosphere& allocate(int _width, int _height);
	
    ofTexture&	getBlurTexture(){ return fbo[frame%2].getTextureReference(); };
	void update( ofTexture& atmosText, ofxEdenGeosphere& geo );
    
};

#endif
