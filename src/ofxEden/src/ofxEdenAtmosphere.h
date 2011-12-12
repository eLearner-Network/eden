/*
 *  ofxEdenAtmosphere.h
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 01/06/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 *  Based on Optical Flow (2010/05/28) by Hidetoshi Shimodaira
 *	shimo@is.titech.ac.jp 2010 GPL
 *	http://www.openprocessing.org/visuals/?visualID=10435
 *
 */

#ifndef OFXEDENATMOSPHERE
#define OFXEDENATMOSPHERE

#include "ofMain.h"

#include "ofxBlob.h"
#include "ofxTracking.h"
#include "ofxContourFinder.h"

#include "ofxEdenData.h"
#include "ofxFluid.h"

class ofxEdenAtmosphere {
public:
	ofxEdenAtmosphere(){ data = NULL; };
	
	ofxEdenAtmosphere& linkData(ofxEdenData* _data){ data = _data; return * this; };
	ofxEdenAtmosphere& allocate(int _width, int _height,float _scale);
	ofxEdenAtmosphere& setColdAt(float _x, float _y, float _amount);
	ofxEdenAtmosphere& setHotAt(float _x, float _y, float _amount);
	ofxEdenAtmosphere& setHumidityAt(float _x, float _y, ofFloatColor _color);
	ofxEdenAtmosphere& circularWind(float force, float angle);
	
    ofxFluid&   getFluid(){return fluid;};
	ofTexture&	getTextureReference() {return clouds.getTextureReference();};
	
    void update(ofxCvGrayscaleImage &blobImage, ofFloatImage &depthFloatImage);
	
	void draw(int _x, int _y, int _width, int _height);
	
private:
    ofFbo               clouds;
    
    ofxFluid            fluid;
	ofxEdenData			*data;
	ofxContourFinder	contour;
	ofxTracking			tracker;
	
	ofVec2f	getCircularForce(ofVec2f _loc, float _angle);
	
	float scale;
	int width, height;
	int cols,rows;
};

#endif