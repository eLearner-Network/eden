/*
 *  ofxEdenTextures.cpp
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#include "ofxEdenTextures.h"

ofxEdenTextures::ofxEdenTextures(){
	frame = 0;
	data = NULL;
}

ofxEdenTextures& ofxEdenTextures::allocate(int _width, int _height){
	initFbos(_width, _height);
    
    string fragmentShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect depth;\
    uniform sampler2DRect noise;\
    uniform sampler2DRect normals;\
    uniform sampler2DRect geosphere;\
    uniform sampler2DRect hydrosphere;\
    uniform sampler2DRect biosphere;\
    uniform sampler2DRect snow;\
    uniform sampler2DRect water;\
    uniform sampler2DRect ground0;\
    uniform sampler2DRect ground1;\
    uniform sampler2DRect rock0;\
    uniform sampler2DRect rock1;\
    uniform sampler2DRect vegetation0;\
    uniform sampler2DRect vegetation1;\
    uniform sampler2DRect vegetation2;\
    uniform float waterLevel;\
    float oceanF = 0.4;\
    float beachF = 0.009;\
    float greenPunchF = 1.5;\
    uniform float snowF;\
    float map(float value, float inputMin, float inputMax, float outputMin, float outputMax) {\
        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);\
        if (outputMax < outputMin){\
            if( outVal < outputMax ) outVal = outputMax;\
            else if( outVal > outputMin ) outVal = outputMin;\
        } else {\
            if( outVal > outputMax ) outVal = outputMax;\
            else if( outVal < outputMin ) outVal = outputMin;\
        }\
        return outVal;\
    }\
    \
    void main(void) { \
        vec2  st	= gl_TexCoord[0].st;\
        vec2  stSmall = st*0.5;\
        vec4  norm	= ( texture2DRect(normals, stSmall) - 0.5 ) * 2.0;\
        float dpt	= texture2DRect( depth, stSmall ).r;\
        float inc	= ( abs(norm.x) + abs(norm.y) ) * 0.50;\
        float n		= texture2DRect( noise, stSmall ).b;\
        float alt	= texture2DRect( geosphere, stSmall ).r;\
        float grn	= texture2DRect( geosphere, stSmall ).g;\
        float wtr	= texture2DRect( geosphere, stSmall ).b;\
        float hyd	= texture2DRect( hydrosphere, stSmall ).b;\
        float veg	= texture2DRect( biosphere, stSmall ).g;\
        vec3 color	= texture2DRect( ground0, st ).rgb;\
        if ( dpt <= waterLevel ) {\
            float wtrF = map(dpt, 0.0, oceanF, 0.0, 1.0);\
            color = mix(texture2DRect( water, st ).rgb, color, wtrF);\
            if ( dpt <= oceanF ){\
                float factor = map(dpt, 0.0, oceanF, 0.0, 1.0);\
                color = mix(color,texture2DRect( water, st ).rgb, factor);\
            }\
        } else {\
            vec3 noiseColor = mix(texture2DRect( rock0, st ).rgb, texture2DRect( rock1, st ).rgb,n);\
            color = mix(color,texture2DRect( ground1, st ).rgb,alt);\
            color = mix(noiseColor,color,inc);\
            if ( veg > 0.001 ){\
                vec3 vegIntensity = mix(texture2DRect( vegetation1, st).rgb,texture2DRect( vegetation2, st).rgb, grn );\
                vec3 vegAltitud = mix(vegIntensity,texture2DRect( vegetation0, st ).rgb, alt );\
                color = mix(color,vegAltitud, veg*greenPunchF);\
            }\
            if ( hyd > 0.1 ){\
                if (alt >= snowF ){\
                    float factor = map( alt, snowF, 1.0, 0.0, 1.0);\
                    color = mix(color,texture2DRect( snow, st).rgb,factor );\
                }\
                color.b = mix(hyd,color.b,alt);\
            }\
        }\
        gl_FragColor.rgb = color;\
        gl_FragColor.a = 1.0;\
    }";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    shader.linkProgram();
    //loadShader("shaders/textures.frag");
	
    snow = ofImage();
    snow.allocate(width, height, OF_IMAGE_COLOR);
	snow.loadImage("textures/snow/0.png");
	
    water.loadBackground("textures/water/1.png");
	
	for (int i = 0; i < 2; i++){
		string imageFile = "textures/ground/"+ofToString(i)+".png";
        ground[i] = ofImage();
        ground[i].allocate(width, height, OF_IMAGE_COLOR);
		ground[i].loadImage(imageFile);
	}
	
	for (int i = 0; i < 2; i++){
		string imageFile = "textures/rock/"+ofToString(i)+".png";
        rock[i] = ofImage();
		rock[i].allocate(width, height, OF_IMAGE_COLOR);
        rock[i].loadImage(imageFile);
	}
	
	for (int i = 0; i < 3; i++){
		string imageFile = "textures/vegetation/"+ofToString(i)+".png";
        vegetation[i] = ofImage();
        vegetation[i].allocate(width, height, OF_IMAGE_COLOR);
		vegetation[i].loadImage(imageFile);
	}
	
	return * this;
}

void ofxEdenTextures::update(ofTexture& hydroText, ofxEdenBiosphere& bio, ofxEdenGeosphere& geo){
    
    ofEnableAlphaBlending();
    
    // Generate water waves
    water.begin();
    geo.getChange().draw(0,0,width,height);
    water.end();
    
    // Generate multi Texturing on x2 scale
	ofEnableAlphaBlending();
	fbo[1].begin();
	ofSetColor(255, 255);
	ofClear(0, 0, 0, 255);
	shader.begin();
	
	shader.setUniformTexture("depth", geo.getAverage(), 0);
	shader.setUniformTexture("normals", geo.getNormal(), 1);
	shader.setUniformTexture("noise", geo.getNoise(), 2);
	
	shader.setUniformTexture("geosphere", geo.getTextureReference(), 3);
	shader.setUniformTexture("biosphere", bio.getTextureReference(), 4);
	shader.setUniformTexture("hydrosphere", hydroText, 5);
	
	shader.setUniformTexture("water",water.getTextureReference(), 6);
	shader.setUniformTexture("snow",snow.getTextureReference(), 7);
	
	shader.setUniformTexture("ground0", ground[0].getTextureReference() , 8);
	shader.setUniformTexture("ground1", ground[1].getTextureReference() , 9);
	
	shader.setUniformTexture("rock0",rock[0].getTextureReference() , 10);
	shader.setUniformTexture("rock1",rock[1].getTextureReference() , 11);
	
	shader.setUniformTexture("vegetation0", vegetation[0].getTextureReference() , 12);
	shader.setUniformTexture("vegetation1", vegetation[1].getTextureReference() , 13);
	shader.setUniformTexture("vegetation2", vegetation[2].getTextureReference() , 14);
	
    shader.setUniform1f("waterLevel", (float) data->waterLevel );
	shader.setUniform1f("snowF", (float)(0.5));
	
    drawFrame(width, height);
    
	shader.end();
    
#ifdef LIFEGLOW // Add lifeGlow
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(55, 255, 0);
    bio.getAnimalLife().draw(0,0, width, height);
    ofDisableBlendMode();
#endif
    
	fbo[1].end();
    
	texture = fbo[1].getTextureReference();
    frame++;
}
