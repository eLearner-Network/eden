/*
 *  ofxEdenGeosphere.cpp
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#include "ofxEdenGeosphere.h"

ofxEdenGeosphere::ofxEdenGeosphere()
{
    cicles  = 0;
	frame	= 0;
	frames	= NULL;
	data = NULL;
    
}

ofxEdenGeosphere& ofxEdenGeosphere::allocate(int _width, int _height)
{
    initFbos(_width, _height);
    
    cout << "- average shader" << endl;
    string fragmentAverageShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect tex0;\
    uniform sampler2DRect tex1;\
    uniform sampler2DRect tex2;\
    uniform sampler2DRect tex3;\
    uniform sampler2DRect tex4;\
    uniform sampler2DRect tex5;\
    uniform sampler2DRect tex6;\
    uniform sampler2DRect tex7;\
    uniform sampler2DRect tex8;\
    uniform sampler2DRect tex9;\
    \
    uniform float steps;\
    \
    void main(void) {\
        vec2  st = gl_TexCoord[0].st;\
        \
        float a = texture2DRect(tex0, st).r;\
        float b = texture2DRect(tex1, st).r;\
        float c = texture2DRect(tex2, st).r;\
        float d = texture2DRect(tex3, st).r;\
        float e = texture2DRect(tex4, st).r;\
        float f = texture2DRect(tex5, st).r;\
        float g = texture2DRect(tex6, st).r;\
        float h = texture2DRect(tex7, st).r;\
        float i = texture2DRect(tex8, st).r;\
        float j = texture2DRect(tex9, st).r;\
        \
        float value = a + b + c + d + e + f + g + h + i + j;\
        value /= steps;\
        \
        gl_FragColor.rgb = vec3( value, value, value);\
        gl_FragColor.a = 1.0;\
    }";
    averageShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentAverageShader);
    averageShader.linkProgram();
	//averageShader.load("", "shaders/average.frag");
	averageTexture.allocate(_width,_height,GL_RGB16F);
    
    for (int i = 0; i < 2; i++){
        fbo[i].allocate(width,height,GL_RGB16F);
        fbo[i].begin();
        ofClear(255,255);
        fbo[i].end();
    }
    
    //setTotalFrames(data->totalFrames, fbo[0].getTextureReference());
    setTotalFrames(10, fbo[0].getTextureReference());
    
	cout << "- normalmap shader" << endl;
	normalFbo.allocate(width, height,GL_RGB16F);
    string fragmentNormalShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect tex;\
    uniform float xOffset;\
    uniform float yOffset;\
    \
    void main(){\
        float sCoord		= gl_TexCoord[0].s;\
        float tCoord		= gl_TexCoord[0].t;\
        \
        float center		= texture2DRect(tex, vec2( sCoord, tCoord ) ).r;\
        \
        float topLeft		= texture2DRect(tex, vec2(sCoord - xOffset	, tCoord - yOffset	) ).r;\
        float left			= texture2DRect(tex, vec2(sCoord - xOffset	, tCoord			) ).r;\
        float bottomLeft	= texture2DRect(tex, vec2(sCoord - xOffset	, tCoord + yOffset	) ).r;\
        float top			= texture2DRect(tex, vec2(sCoord			, tCoord - yOffset	) ).r;\
        float bottom		= texture2DRect(tex, vec2(sCoord			, tCoord + yOffset	) ).r;\
        float topRight		= texture2DRect(tex, vec2(sCoord + xOffset	, tCoord - yOffset	) ).r;\
        float right			= texture2DRect(tex, vec2(sCoord + xOffset	, tCoord			) ).r;\
        float bottomRight	= texture2DRect(tex, vec2(sCoord + xOffset	, tCoord + yOffset	) ).r;\
        \
        float dX = topRight + 2.0 * right + bottomRight - topLeft - 2.0 * left - bottomLeft;\
        \
        float dY = bottomLeft + 2.0 * bottom + bottomRight - topLeft - 2.0 * top - topRight;\
        \
        vec3 N = normalize( vec3( dX, dY, 0.01) );\
        \
        N *= 0.5;\
        N += 0.5;\
        \
        gl_FragColor.rgb = N;\
        gl_FragColor.a = 1.0;\
    }";
    normalShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentNormalShader);
    normalShader.linkProgram();
	//normalShader.load("", "shaders/normal.frag");
	normalTexture.allocate(_width,_height,GL_RGB16F);
	
    cout << "- noise shader" << endl;
	noiseFbo.allocate(width, height,GL_RGB16F );
    string fragmentNoiseShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect normals;\
    vec2 offset[9];\
    \
    void main(void) {\
        vec2  st	= gl_TexCoord[0].st;\
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
        vec2 normalC = vec2( texture2DRect( normals, st + offset[4] ).r, texture2DRect( normals, st + offset[4] ).g);\
        float noise  = 0.0;\
        for( int i = 0; i < 9; i++ ){\
            if (i != 4){\
                vec2 normalI = vec2( texture2DRect( normals, st + offset[i] ).r, texture2DRect( normals, st + offset[i] ).g);\
                vec2 normalV = normalI - normalC;\
                float normalA = length(normalV);\
                noise += normalA;\
            }\
        }\
        noise = 1.0 - (noise * 0.125);\
        \
        gl_FragColor.rgb = vec3(noise,noise,noise);\
        gl_FragColor.a = 1.0;\
    }";
    noiseShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentNoiseShader);
    noiseShader.linkProgram();
	//noiseShader.load("", "shaders/noise.frag");
	noiseTexture.allocate(_width,_height,GL_RGB16F );
	
    cout << "- conditions shader" << endl;
    string fragmentShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect heightmap;\
    uniform sampler2DRect noise;\
    \
    uniform float waterLevel;\
    \
    void main(void) {\
        vec2  st	= gl_TexCoord[0].st;\
        \
        float value = clamp(texture2DRect(heightmap, st).r,0.0,1.0);\
        float n		= clamp(texture2DRect(noise, st).r,0.0,1.0);\
        \
        vec3 color = vec3(0.0,0.0,0.0);\
        \
        if ( value <= (waterLevel * 0.5)){\
            color.b = ( value / (waterLevel * 0.5) ) * ( 1.0 - 0.431 ) + 0.431 ;\
        } else if ( value <= waterLevel ){\
            color.b = 1.0;\
        } else if ( value <= ((1.0-waterLevel)*0.5+waterLevel) ) {\
            color.g = n; \
            color.r	= ((value - waterLevel ) / ( 1.0 - waterLevel ));\
        } else { \
            color.r	= ((value - waterLevel ) / ( 1.0 - waterLevel ));\
            color.g	= ( 1.0 - (value - ((1.0-waterLevel )*0.5 + waterLevel))/( 1.0 - (( 1.0 - waterLevel ) * 0.5 + waterLevel)) ) * n;\
        }\
        \
        gl_FragColor.rgb = color;\
        gl_FragColor.a = 1.0;\
    }";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    shader.linkProgram();
	//loadShader("shaders/conditions.frag");
    conditionsFbo.allocate(width, height,GL_RGB16F);
	texture.allocate(width,height,GL_RGB16F);
    
    cout << "- change Fbo" << endl;
    changeFbo.allocate(width,height,GL_RGB);
	changeTexture.allocate(_width,_height,GL_RGB);
	
	return * this;
}

ofxEdenGeosphere& ofxEdenGeosphere::setTotalFrames(int n, ofTexture& cleanTexture)
{
	totalFrames = n;
	
	delete [] frames;
	frames = new ofTexture[n];
	for (int i = 0; i < totalFrames; i++){
		frames[i] = ofTexture();
		frames[i].allocate(width,height,GL_RGB16F);
        frames[i] = cleanTexture;
	}
}

void ofxEdenGeosphere::update(ofFloatImage &fImage )
{
	ofDisableAlphaBlending();
	frames[frame] = fImage.getTextureReference();
    
    averageTexture = average(frames, totalFrames);
    
	normalTexture = normal( averageTexture );
	noiseTexture = noise( normalTexture );
	texture = conditions( averageTexture , noiseTexture );
	
	changeTexture = change();
	
	frame = (frame+1)%totalFrames;
}

ofTexture  ofxEdenGeosphere::average(ofTexture *framesArray, int nTotal)
{
    int actual = frame%2;
    int other = (frame+1)%2;
    
	fbo[ actual ].begin();
	ofClear(0, 0, 0, 255);
	averageShader.begin();
    
    for (int i = 0; i < nTotal; i++){
        string texName = "tex"+ofToString(i);
        averageShader.setUniformTexture(texName.c_str() , framesArray[i] , i );
    }
    
	averageShader.setUniform1f("steps", (float)nTotal);
	
	drawFrame(width,height);
	
	averageShader.end();
	fbo[ actual ].end();
    
	/*
     fbo[ other ].begin();
     blurShader.begin();
     blurShader.setUniformTexture("tex", fbo[actual].getTextureReference() , 0);
     blurShader.setUniform1f("fade_const", (float)1.0);
     drawFrame(width,height);
     blurShader.end();
     fbo[ other ].end();*/
    
	return fbo[ actual ].getTextureReference();
}

ofTexture ofxEdenGeosphere::normal(ofTexture& heightmap)
{
	normalFbo.begin();
	ofClear(0, 0, 0, 255);
	normalShader.begin();
	normalShader.setUniform1f("xOffset", 1.0f);
	normalShader.setUniform1f("yOffset", 1.0f);
	
	heightmap.draw(0, 0);
	
	normalShader.end();
	normalFbo.end();
	
	return normalFbo.getTextureReference();
}

ofTexture ofxEdenGeosphere::noise(ofTexture& normalmap)
{
	noiseFbo.begin();
	ofClear(0, 0, 0, 255);
	noiseShader.begin();
	
	normalmap.draw(0, 0);
	
	noiseShader.end();
	noiseFbo.end();
	
	return noiseFbo.getTextureReference();
}

ofTexture ofxEdenGeosphere::conditions(ofTexture& heightmap, ofTexture& noiseText)
{
	conditionsFbo.begin();
	ofClear(0, 0, 0, 255);
	shader.begin();
	shader.setUniformTexture("heightmap", heightmap, 0 );
	shader.setUniformTexture("noise", noiseText, 1 );
	shader.setUniform1f("waterLevel", data->waterLevel);
	
	drawFrame(width,height);
	
	shader.end();
	conditionsFbo.end();
	
	return conditionsFbo.getTextureReference();
}

ofTexture ofxEdenGeosphere::change()
{
	//ofDisableBlendMode();
    //ofDisableAlphaBlending();
    //ofEnableAlphaBlending();
	
	changeFbo.begin();
	ofClear(0, 0, 0, 255);
	ofSetColor(255, 255);
	
	frames[frame].draw(0,0);
	
	ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
	frames[(frame+totalFrames-3)%totalFrames].draw(0,0);
	ofDisableBlendMode();
	 
	changeFbo.end();
	
	return changeFbo.getTextureReference();
}

void ofxEdenGeosphere::drawDepth(int _x, int _y, int _width, int _height)
{
	ofSetColor(255,255);
	averageTexture.draw(_x,_y,_width,_height);
}