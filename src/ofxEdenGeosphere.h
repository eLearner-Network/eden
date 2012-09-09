/*
 *  ofxEdenGeosphere.h
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */
#ifndef OFXEDENGEOSPHERE
#define OFXEDENGEOSPHERE

#include "ofMain.h"
#include "ofxEdenSphere.h"



class ofxEdenGeosphere : public ofxEdenSphere {
public:
	ofxEdenGeosphere();
	~ofxEdenGeosphere(){ delete [] frames; };

	ofxEdenGeosphere& allocate(int _width, int _height);
	ofxEdenGeosphere& setTotalFrames(int n, ofTexture& cleanTexture);
	
	void update( ofFloatImage &fImage );
	
	ofTexture&	getAverage(){ return averageTexture; };
	ofTexture&	getChange(){ return changeTexture; };
	ofTexture&	getNormal(){ return normalTexture; };
	ofTexture&	getNoise(){ return noiseTexture; };
	ofTexture&	getConditions(){ return texture; };
	
	void drawDepth(int _x, int _y, int _width, int _height);
	
private:
	// blur 
    ofTexture	blur(ofTexture& tex);
    ofShader    blurShader;
    
	// heightmap average ( GPU MultiTexture Average )
    ofTexture   average( ofTexture *framesArray, int nTotal );
    ofShader    averageShader;
    
    ofFbo       fbo[2];
	ofTexture	averageTexture;
	
	ofTexture	change();
	ofFbo		changeFbo;
	ofTexture	changeTexture;
	
	ofTexture	*frames;
	int			totalFrames;	// For the depth average ( actualFRAME and TOTALaverageFRAMES)
    int         cicles;
    
	// Depresions calculation ( GPU NormalMap )
	ofTexture	normal(ofTexture& heightmap);
	ofFbo		normalFbo;
	ofShader	normalShader;
	ofTexture	normalTexture;
	
	// Noise calculation ( GPU )
	ofTexture	noise(ofTexture& normalmap);
	ofFbo		noiseFbo;
	ofShader	noiseShader;
	ofTexture	noiseTexture;
	
	// Conditions interpretation ( GPU Coloring)
	ofTexture	conditions(ofTexture& heightmap, ofTexture& noiseText);
	ofFbo		conditionsFbo; 
};

#endif

