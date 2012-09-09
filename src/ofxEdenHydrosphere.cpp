/*
 *  ofxEdenHydrosphere.cpp
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#include "ofxEdenHydrosphere.h"

ofxEdenHydrosphere::ofxEdenHydrosphere(){
	frame = 0;
	data = NULL;
}

ofxEdenHydrosphere& ofxEdenHydrosphere::allocate(int _width, int _height){
	initFbos(_width, _height);
    
    string fragmentShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect hydrosphere;\
    uniform sampler2DRect normals;\
    uniform sampler2DRect conditions;\
    uniform sampler2DRect atmosphere;\
    \
    uniform float precipitation_amount;\
    uniform float precipitation_humidity;\
    uniform float precipitation_altitud;\
    uniform float precipitation_inclination;\
    uniform float precipitation_cold;\
    uniform float depresions_flow;\
    uniform float absortion_soil;\
    \
    vec2 offset[9];\
    \
    void main() {\
        vec2  st = gl_TexCoord[0].st;\
        \
        float water	= texture2DRect(hydrosphere, st).b;\
        float underWaterLevel = texture2DRect(conditions, st).b;\
        float alfa = 0.0;\
        \
        if (underWaterLevel > 0.0){\
            water = max(water,underWaterLevel) * 0.5; \
            alfa = 1.0; \
        } else { \
            float hum	= texture2DRect(atmosphere, st).r * texture2DRect(atmosphere, st).a;\
            float col	= texture2DRect(atmosphere, st).b * texture2DRect(atmosphere, st).a;\
            float alt	= texture2DRect(conditions, st).r;\
            vec4 norm	= ( texture2DRect(normals, st) - 0.5 ) * 2.0;\
            float inc	= ( abs(norm.x) + abs(norm.y) ) * 0.5;\
            float soil	= texture2DRect(conditions, st).g;\
            \
            offset[0] = vec2(-1.0, -1.0);\
            offset[1] = vec2(0.0, -1.0);\
            offset[2] = vec2(1.0, -1.0);\
            \
            offset[3] = vec2(-1.0, 0.0);\
            offset[4] = vec2(0.0, 0.0);\
            offset[5] = vec2(1.0, 0.0);\
            \
            offset[6] = vec2(-1.0, 1.0);\
            offset[7] = vec2(0.0, 1.0);\
            offset[8] = vec2(1.0, 1.0);\
            \
            if (hum > precipitation_humidity) {\
                if ( ( inc >= precipitation_inclination ) && (alt >= precipitation_altitud) )\
                    water += precipitation_amount * hum;\
                \
                if (col > precipitation_cold)\
                    water += precipitation_amount * hum;\
            }\
			\
            float wSource = 0.0;\
            float wValue = 0.0;\
            \
            for (int i = 0; i < 9; i++){\
                float waterOnArea = texture2DRect(hydrosphere, st + offset[i]).b;\
                \
                if ( (waterOnArea > 0.0) && (i != 4)){\
                    vec4 goingTo = ( texture2DRect( normals, st + offset[i] ) - 0.5 ) * 2.0;\
                    vec2 match = vec2(goingTo.x + offset[9-i-1].x, goingTo.y + offset[9-i-1].y);\
                    if ( (match.x <= 1.0) && (match.x >= -1.0) && (match.y <= 1.0) && (match.y >= -1.0) ){\
                        \
                        wSource += 1.0;\
                        wValue += waterOnArea;\
                    }\
                }\
            }\
            \
            if (wSource > 1.0)\
                water += wValue/wSource;\
            \
            water -= soil * absortion_soil;\
            water -= inc * depresions_flow ;\
            \
            water = max(0.0 ,  min( water, 1.0) );\
            alfa = min(water*2.0, 0.8);\
        }\
        \
        gl_FragColor = vec4(0.0,0.0, water , alfa );\
    }";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    shader.linkProgram();
	//loadShader("shaders/hydrosphere.frag");

	return * this;
}

void ofxEdenHydrosphere::update( ofTexture& atmosText, ofxEdenGeosphere& geo ){
    int actual = frame%2;       // Actual buffer
    int other = (frame+1)%2;    // Prev/Next buffer
    
	ofEnableAlphaBlending();
	// Ping
	fbo[ actual ].begin();
	ofClear(0, 0, 0, 0);
	
	shader.begin();
	shader.setUniformTexture("hydrosphere", fbo[ other ].getTextureReference(), 0 );
	shader.setUniformTexture("normals", geo.getNormal(), 1 );
	shader.setUniformTexture("conditions", geo.getConditions(), 2 );
	shader.setUniformTexture("atmosphere", atmosText, 3 );
	
	shader.setUniform1f("absortion_soil", (float)data->absortionSoil);
	shader.setUniform1f("precipitation_altitud", (float)data->precipitationAltitud);
	shader.setUniform1f("precipitation_inclination", (float)data->precipitationInclination);
	shader.setUniform1f("precipitation_cold", (float)data->precipitationCold);
	shader.setUniform1f("precipitation_humidity", (float)data->precipitationHumidity);
	shader.setUniform1f("precipitation_amount", (float)data->precipitationAmount);
	shader.setUniform1f("depresions_flow", (float)data->depresionFlow);
	
	drawFrame(width,height);
	
	shader.end();
	fbo[ actual ].end();
	
	texture = fbo[ actual ].getTextureReference();
	
	frame++;
}