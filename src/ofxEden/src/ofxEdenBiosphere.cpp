/*
 *  ofxEdenBiosphere.cpp
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#include "ofxEdenBiosphere.h"
ofxEdenBiosphere::ofxEdenBiosphere(){
	frame = 0;
	data = NULL;
}

ofxEdenBiosphere& ofxEdenBiosphere::allocate(int _width, int _height){
	initFbos(_width, _height);
    
    string fragmentShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    float kernel[9];\
    vec2 offset[9];\
    uniform sampler2DRect conditions;\
    uniform sampler2DRect biosphere;\
    uniform sampler2DRect hydrosphere;\
    uniform sampler2DRect change;\
    uniform float diffU;\
    uniform float diffV;\
    uniform float f;\
    uniform float k;\
    uniform float width;\
    \
    void main(void) {\
        vec2  st	= gl_TexCoord[0].st;\
        float alt	= texture2DRect( conditions, st ).r;\
        float nut	= texture2DRect( conditions, st ).g;\
        float wtr	= texture2DRect( conditions, st ).b;\
        float hyd	= texture2DRect( hydrosphere, st ).b;\
        float chn	= texture2DRect( change, st ).r;\
        float bioV	= 0.0;\
        float bioU	= 0.0;\
        \
        if ( wtr == 0.0 ){\
            bioV	= texture2DRect( biosphere, st ).g + hyd * 0.5;\
            bioU	= texture2DRect( biosphere, st ).b ;\
			\
            kernel[0] = 0.707106781;\
            kernel[1] = 1.0;\
            kernel[2] = 0.707106781;\
            kernel[3] = 1.0;\
            kernel[4] = -6.82842712;\
            kernel[5] = 1.0;\
            kernel[6] = 0.707106781;\
            kernel[7] = 1.0;\
            kernel[8] = 0.707106781;\
            \
            offset[0] = vec2( -1.0, -1.0);\
            offset[1] = vec2(  0.0, -1.0);\
            offset[2] = vec2(  1.0, -1.0);\
            \
            offset[3] = vec2( -1.0, 0.0);\
            offset[4] = vec2(  0.0, 0.0);\
            offset[5] = vec2(  1.0, 0.0);\
            \
            offset[6] = vec2( -1.0, 1.0);\
            offset[7] = vec2(  0.0, 1.0);\
            offset[8] = vec2(  1.0, 1.0);\
            \
            vec2 lap  = vec2( 0.0, 0.0 );\
            \
            for( int i=0; i < 9; i++ ){\
                vec2 tmp = vec2(texture2DRect( biosphere, st + offset[i] ).g, texture2DRect( biosphere, st + offset[i] ).b);\
                lap	+= tmp * kernel[i];\
            }\
            \
            float F	= f + hyd * 0.025 - 0.0005;\
            float K	= k + hyd * 0.025 - 0.0005;\
            \
            float uvv = bioU * bioV * bioV;\
            float du = diffU * lap.g - uvv + F * (1.0 - bioU);\
            float dv = diffV * lap.r + uvv - (F + K) * bioV;\
            bioU += du * 0.6;\
            bioV += dv * 0.6;\
            \
            bioV -= alt*0.001;\
            bioU -= alt*0.001;\
            \
            bioV -= (1.0-nut)*0.001;\
            bioU -= (1.0-nut)*0.001;\
            \
            if (chn <= 0.1) \
                chn = 1.0;\
            \
            bioV *= chn;\
            bioU *= chn;\
        }\
        \
        gl_FragColor.rgb = vec3(0.0, clamp( bioV, 0.0, 1.0 ), clamp( bioU, 0.0, 1.0 ));\
        gl_FragColor.a = 1.0;\
    }";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    shader.linkProgram();
	//loadShader("shaders/biosphere.frag");
    
    flock.allocate(width, height, 640).setParticleSize(20).setTimpeStep(0.0005);
	
	return * this;
}

void ofxEdenBiosphere::update(ofTexture& hydroText, ofxEdenGeosphere& geo){
    int actual = frame%2;       // Actual buffer
    int other = (frame+1)%2;    // Prev/Next buffer
    
	ofEnableAlphaBlending();
	
	//------------------------------------------------------------- Vegetation 
	
	// Ping
	fbo[actual].begin();
	ofClear(0, 0, 0, 0);
	
	shader.begin();
	shader.setUniformTexture("conditions", geo.getConditions() , 0);
	shader.setUniformTexture("biosphere", fbo[other].getTextureReference(), 1);
	shader.setUniformTexture("hydrosphere", hydroText, 2);
	shader.setUniformTexture("change", geo.getChange(), 3);
	
	shader.setUniform1f("width", (float)width);
	shader.setUniform1f("diffU", (float)(data->biosphereDiffU) );
	shader.setUniform1f("diffV", (float)(data->biosphereDiffV) );
	shader.setUniform1f("f", (float)(data->biosphereF) );
	shader.setUniform1f("k", (float)(data->biosphereK) );
	
	drawFrame(width,height);
	
	shader.end();
	fbo[actual].end();
	
	texture = fbo[actual].getTextureReference();
	
    #ifdef LIFEGLOW
    //--------------------------------------------------------------- Animal life (Flocking)
    flock.update(geo.getNormal(), hydroText, fbo[actual].getTextureReference() );
    #endif
    
	frame++;
}