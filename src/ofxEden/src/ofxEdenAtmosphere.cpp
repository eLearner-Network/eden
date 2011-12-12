/*
 *  ofxEdenAtmosphere.cpp
 *  ofxEden
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */
#include "ofxEdenAtmosphere.h"

ofxEdenAtmosphere& ofxEdenAtmosphere::allocate(int _width, int _height, float _scale)
{
	width = _width;
	height = _height;
	scale = _scale;
	
	rows = width/_scale;
	cols = height/_scale;
    
    fluid.allocate(width, height, 0.25);
    
    fluid.setDensityDissipation(data->atmosphereDenDiss);
    fluid.setVelocityDissipation(data->atmosphereVelDiss);
    fluid.setTemperatureDissipation(data->atmosphereTempDiss);
    
	contour.bTrackBlobs = false;
	contour.bTrackFingers = true;
	
	tracker.MOVEMENT_FILTERING = 0;
	
    clouds.allocate(width, height,GL_RGB);
    
	return * this;
};

ofxEdenAtmosphere& ofxEdenAtmosphere::setColdAt(float _x, float _y, float _amount)
{
    setHumidityAt(_x, _y, ofFloatColor(0.0,0.0,_amount,1.0));
    return * this;
};

ofxEdenAtmosphere& ofxEdenAtmosphere::setHotAt(float _x, float _y, float _amount)
{
    setHumidityAt(_x, _y, ofFloatColor(_amount,0.0,0.0,1.0));
    return * this;
};

ofxEdenAtmosphere& ofxEdenAtmosphere::setHumidityAt(float _x, float _y, ofFloatColor _color)
{
    fluid.addTemporalForce(ofVec2f(_x,_y), ofVec2f(0,0), _color, 1.5f);
    return * this;
};

ofxEdenAtmosphere& ofxEdenAtmosphere::circularWind(float force, float angle)
{
	ofVec2f pos;
	pos.set(ofRandom(1),ofRandom(1));
    fluid.setGravity(getCircularForce(pos,angle)*force);
    return * this;
};

void ofxEdenAtmosphere::update(ofxCvGrayscaleImage &blobImage, ofFloatImage &depthFloatImage)
{
    
	contour.findContours(blobImage, 20, (340*240)/3, 10, 20, false);
	tracker.track(&contour);
	
	std::vector<ofxBlob> blobs = tracker.getTrackedFingers();
	
	for (int i = 0 ; i < blobs.size(); i++){
		ofVec2f pos(blobs[i].centroid.x, blobs[i].centroid.y);
		ofVec2f vel(blobs[i].D.x,blobs[i].D.y);
        
        fluid.addTemporalForce(pos, vel, ofFloatColor(0,0,0),3.f);
	}
	
	if(data->atmosphereCircularForce > 0.01)
		circularWind(data->atmosphereCircularForce,data->atmosphereCircularAngle);
	
    fluid.obstaclesBegin();
    ofClear(0);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(50);
    depthFloatImage.draw(0,0);
    //ofSetColor(255);
    //blobImage.draw(0, 0);
    //ofDisableBlendMode();
    fluid.obstaclesEnd();
    
	clouds.begin();
	ofClear(0, 0, 0, 255);
	ofSetColor(255,255);
    fluid.draw();
	clouds.end();
    
	ofDisableAlphaBlending();
};



void ofxEdenAtmosphere::draw(int _x, int _y, int _width, int _height)
{
    ofPushStyle();
	ofPushMatrix();
	ofEnableAlphaBlending();
	
	ofSetColor(255, 255);
	ofTranslate(_x, _y);
	
    clouds.draw(0, 0,_width, _height);
    contour.draw(0, 0, _width, _height);
	
	ofDisableAlphaBlending();
	ofPopMatrix();
	ofPopStyle();
};

ofVec2f	ofxEdenAtmosphere::getCircularForce(ofVec2f _loc, float _angle = 0)
{
	ofVec2f center, toCenter;
	
	center.set(width*0.5,height*0.5);
	
	toCenter = center - _loc;
	toCenter.perpendicular();
	toCenter.normalize();
	
	toCenter.rotate(_angle);
	
	return toCenter;
};
