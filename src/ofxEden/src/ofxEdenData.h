/*
 *  ofxEdenData.h
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#ifndef OFXEDENDATA
#define OFXEDENDATA

#include "ofMain.h"
#include "ofxGui.h"

class ofxEdenData {
public:
	ofxEdenData();
	
    void    load();
    void    save();
    void    update();
    void    draw();
    
    ofxPanel        gui;
        
    ofxIntSlider    topAltitude,lowAltitude;
    int             totalFrames, activeLayer;
	
	ofxFloatSlider	atmosphereCircularForce, atmosphereCircularAngle;
    ofxFloatSlider  atmosphereTempDiss, atmosphereVelDiss, atmosphereDenDiss;
	int   atmosphereResolution;
	
	ofxFloatSlider	waterLevel, absortionSoil, depresionFlow;
	ofxFloatSlider	precipitationInclination,precipitationAltitud,precipitationCold,precipitationHumidity,precipitationAmount;
	
	ofxFloatSlider	biosphereDiffU,biosphereDiffV,biosphereF,biosphereK;
    ofxFloatSlider  biosphereTimeStep, biosphereMaxDist, biosphereMinDist, biosphereMaxSpeed, biosphereMaxForce;
    ofxFloatSlider  biosphereSeparation, biosphereAlineation, biosphereCohesion;
    ofxFloatSlider  biosphereBorders;
    float           biosphereFlat, biosphereFood;
	
	ofxIntSlider    terrainResolution;
};

#endif