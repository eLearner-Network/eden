//
//  ofxFlocking.h
//  GPU Flocking System
//
//  Using:
//      http://www.gamasutra.com/view/feature/2122/building_a_millionparticle_system.php?page=2
//      Daniel shiffman Flocking Sketch
//
//  Created by Patricio González Vivo on 9/27/11.
//  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
//

/*
 STEPS
 -------
 1. Process birth and death
 2. Update velocities
 3. Update positions
 4. Sort for alpha blending (optional)
 5. Transfer texture data to vertex data
 6. Render particles
 */

#ifndef OFXFLOCKING
#define OFXFLOCKING

#include "ofMain.h"
#include "ofxEdenData.h"

class ofxFlocking {
public:
    ofxFlocking();
    
    ofxFlocking& linkData(ofxEdenData* _data){ data = _data; return * this;};
    ofxFlocking& allocate(int _width, int _height, int _nBoids);
    ofxFlocking& setParticleSize(float _size){particleSize = _size; return * this;};
    ofxFlocking& setTimpeStep(float _timeStep){timeStep = _timeStep; return * this;};

    ofTexture& getTexture(){ return renderFbo.getTextureReference();  };
    
    void    update(ofTexture &normalTex, ofTexture &obstaclesTex, ofTexture &foodTex);
    void    draw();
    
private:
    ofxEdenData *data;
    void    renderFrame(int _width, int _height);
    
    ofImage  sparkImage;
    
    ofShader posUpdateShader;
    ofShader velUpdateShader;
    ofShader renderShader;
    
    ofPoint *particles;
    ofFloatPixels pixels;
    
    ofFbo   posFbo[2];
    ofFbo   velFbo[2];
    ofFbo   renderFbo;
    
    float   width, height;
    float   particleSize, imageWidth, imageHeight;
    float   timeStep, maxDist, minDist, maxSpeed, maxForce;
    float   separation, alineation, cohesion, flat, borders, food;
    int     nBoids, resolution;
    int     frame;
};

#endif
