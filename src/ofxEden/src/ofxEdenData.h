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
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#define SERVER_PORT 1984
#define CLIENT_PORT 1985

class ofxEdenData {
public:
	ofxEdenData();
	
	void    loadXml(string filePath = "config.xml");
	void    saveXml(string filePath = "config.xml");
    
    void    addMaskPoint(float _x, float _y);
    void    delMaskPoint(int _nPoint);
    
    void    update();
    
    ofxOscReceiver  receiver;
    ofxOscSender    sender;
    string          outHost;
    int             outPort;
    
	ofVec2f	atmosphereLoc, geosphereLoc, hydrosphereLoc, biosphereLoc;
	string	shaderDir;
	
	int		topAltitude,lowAltitude, totalFrames, activeLayer;
	
	float	atmosphereCircularForce, atmosphereCircularAngle;
    float   atmosphereTempDiss, atmosphereVelDiss, atmosphereDenDiss;
	int		atmosphereResolution;
	
	float	waterLevel, absortionSoil, depresionFlow;
	float	precipitationInclination,precipitationAltitud,precipitationCold,precipitationHumidity,precipitationAmount;
	
	float	biosphereDiffU,biosphereDiffV,biosphereF,biosphereK;
    float   biosphereTimeStep, biosphereMaxDist, biosphereMinDist, biosphereMaxSpeed, biosphereMaxForce;
    float   biosphereSeparation, biosphereAlineation, biosphereCohesion;
    float   biosphereBorders;
    float   biosphereFlat, biosphereFood;
	
	int		terrainResolution;
	
	ofPoint	center;
	float	scale;
	
    vector<ofPoint> maskCorners;
	int		maskCornersSelected;
};

#endif