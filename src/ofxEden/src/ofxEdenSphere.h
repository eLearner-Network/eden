/*
 *  ofxEdenSphere.h
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */
#ifndef OFXEDENSPHERE
#define OFXEDENSPHERE

#include "ofMain.h"

#include "ofxEdenData.h"

class ofxEdenSphere {
public:
	ofxEdenSphere(){ frame = 0; data = NULL; };
	
	void linkData(ofxEdenData* _data){ data = _data; };
	void loadShader(string toShaderPath){ shader.load("",toShaderPath);};
	void initFbos(int _width, int _height){
		width = _width;
		height = _height;
		
		texture.allocate(width,height,GL_RGB);
        
		// Cleans the FBOs
		for (int i = 0; i < 2; i++){
			fbo[i].allocate(width,height,GL_RGB);
			fbo[i].begin();
			ofClear(0, 0, 0, 255);
			fbo[i].end();
		}
	};
	
	ofTexture&	getTextureReference(){ return texture; };
	
	void draw(int _x, int _y, int _width, int _height){
		ofSetColor(255,255);
		texture.draw(_x,_y,_width,_height);
	};
	
    void drawFrame(int _width=0, int _height=0){
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(_width, 0); glVertex3f(_width, 0, 0);
        glTexCoord2f(_width, _height); glVertex3f(_width, _height, 0);
        glTexCoord2f(0,_height);  glVertex3f(0,_height, 0);
        glEnd();
    }
    
	ofxEdenData *data;
	ofShader	shader;
	ofTexture	texture;
	ofFbo		fbo[2];
	
	int			frame, width, height;
};
#endif
