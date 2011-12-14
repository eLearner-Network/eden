/*
 *  ofxEden.cpp
 *  gpuHidro
 *
 *  Created by Patricio González Vivo on 16/08/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#include "ofxEden.h"
#include "controls/gui.h"

//-------------------------------------------------------------- Constructor/Destructor

ofxEden::ofxEden() {
	ofAddListener(ofEvents.mouseMoved, this, &ofxEden::_mouseMoved);
	ofAddListener(ofEvents.mousePressed, this, &ofxEden::_mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &ofxEden::_mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &ofxEden::_mouseReleased);
	ofAddListener(ofEvents.keyPressed, this, &ofxEden::_keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &ofxEden::_keyReleased);
	ofAddListener(ofEvents.setup, this, &ofxEden::_setup);
	ofAddListener(ofEvents.update, this, &ofxEden::_update);
	ofAddListener(ofEvents.draw, this, &ofxEden::_draw);
    ofAddListener(ofEvents.exit , this, &ofxEden::_exit);
	
	bGui	= false;
	bDebug	= false;
	bTerrain= false;
	bMask	= false;
    bMouse  = false; 
}

//-------------------------------------------------------------- Setup
void ofxEden::_setup(ofEventArgs &e){
	//ofDisableAlphaBlending();
    
	// Load Settings
    cout << "Loading xml data Settings" << endl;
	data.loadXml();
	
    cout << "Starting openNI Drivers for Kinect Sensor" << endl;
    context.setup();
	depth.setup(&context);
	width = 640;
	height = 480;
	numPixels = width * height;

    cout << "Starting GUI" << endl;
	gui = ofxGui::Instance(this);
	setupGui();
	
    cout << "Allocating INPUT images" << endl;
	blobImage.allocate(width,height);
	depthFloatImage.allocate(width, height,OF_IMAGE_GRAYSCALE);

    cout << "Starting Atmosphere" << endl;
	atmosphere.linkData(&data);
	atmosphere.allocate(width,height, data.atmosphereResolution );
	
	cout << "Starting Geosphere" << endl;
	geosphere.linkData(&data);
	geosphere.allocate(width,height);
	
	cout << "Starting Hydrosphere" << endl;
	hydrosphere.linkData(&data);
	hydrosphere.allocate(width,height);
	
	cout << "Starting Biosphere" << endl;
	biosphere.linkData(&data);
	biosphere.allocate(width,height);
    biosphere.flock.linkData(&data);
	
	cout << "Starting multi-texturing mapping" << endl;
	textures.linkData(&data);
	textures.allocate(width*2, height*2);
	
	cout << "Setting Lights, Camera and blur filter" << endl;
	light.setPosition(ofVec3f(0,800,-100));
	light.setDiffuseColor(ofColor(192, 160, 128));
	cam.setDistance(3000);
    
    ofSetFullscreen(true);
}

//-------------------------------------------------------------- Update
void ofxEden::_update(ofEventArgs &e) {
	// INPUT DATA
	// -----------------------------------------------------
	data.update();
    
	// Update Kinect Information
    context.update();
    depth.update();
    
    // Extract a the raw depth image from openNI drivers...
    xn::DepthGenerator	depth_generator;
	xn::DepthMetaData	dmd;
    
	depth_generator = depth.getXnDepthGenerator();
	depth_generator.GetMetaData(dmd);
    
	const XnDepthPixel* depthRaw = dmd.Data();
    
    //ofShortPixels pixels = openNI.getDepthRawPixels();
    //unsigned short * depthRaw = pixels.getPixels();
    
    unsigned char * blobPixels = blobImage.getPixels();
    float * depthFloatPixels = depthFloatImage.getPixels();
	
    // ... and extract the range it neads
    int farThreshold = data.topAltitude;
    int nearThreshold = data.lowAltitude;
	
    for(int i = 0; i < numPixels; i++, depthRaw++) {
        if(*depthRaw <= nearThreshold && *depthRaw >= farThreshold){
            blobPixels[i] = 0;
            depthFloatPixels[i] += ofMap(*depthRaw, nearThreshold, farThreshold, 0.0f,1.0f);
            depthFloatPixels[i] *= 0.5;
        } else if ( *depthRaw < nearThreshold ){
            if ( *depthRaw == 0)
                blobPixels[i] = 0;
            else 
                blobPixels[i] = 255;
        } else if ( *depthRaw > farThreshold ){
            blobPixels[i] = 0;
            depthFloatPixels[i] = 0.0f;
        } else {
            blobPixels[i] = 0;
        }
        
        int x = i%width;
        int y = i/height;
		
        if ( depthFloatPixels[i] <= data.waterLevel){
            if (ofRandom(100000) < 1.0)
                atmosphere.setHotAt(x,y,0.5);
        } else {
            if (ofRandom(100000) < 1.0)
                atmosphere.setColdAt(x,y,0.5);
        }
    }
        
    // Save the results in a blobImage for the sky and depthFloatImage for the heightmap
    blobImage.flagImageChanged();
    depthFloatImage.setFromPixels(depthFloatPixels, width, height, OF_IMAGE_GRAYSCALE);
        
    // PROCESS DATA
    // -----------------------------------------------------
    // Atmosphere
    atmosphere.update(blobImage, depthFloatImage);
    ofDisableAlphaBlending();
        
    // Geosphere
    geosphere.update( depthFloatImage );
    
	// Hydrosphere
	hydrosphere.update( atmosphere.getTextureReference(), geosphere );
	
	// Biosphere
	biosphere.update(hydrosphere.getTextureReference(), geosphere );
	
	// FinalMix
	textures.update(hydrosphere.getBlurTexture(), biosphere, geosphere);
}

//-------------------------------------------------------------- Draw
void ofxEden::_draw(ofEventArgs &e){
	ofEnableAlphaBlending();
	
	ofBackground(0);
	ofSetColor(255,255);
	
	if (!bTerrain){
		scale = (ofGetWindowSize().y / height) * data.scale; 
		
		ofPushMatrix();
		ofTranslate(ofGetWindowWidth()*0.5, ofGetWindowHeight()*0.5);
		ofScale(scale, scale,1);
        ofSetColor(255,255);
        
        if (data.activeLayer == 0)
            textures.draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
        else if (data.activeLayer == 1)
            depthFloatImage.draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
        else if (data.activeLayer == 2)
            geosphere.drawDepth(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
        else if (data.activeLayer == 3)
            geosphere.getNormal().draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
        else if (data.activeLayer == 4)
            geosphere.getNoise().draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
        else if (data.activeLayer == 5)
            geosphere.getConditions().draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
        else if (data.activeLayer == 6){
            geosphere.drawDepth(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            hydrosphere.draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
            ofDisableBlendMode();
        } else if (data.activeLayer == 7){
            geosphere.drawDepth(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            biosphere.draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
            ofDisableBlendMode();
        } else if (data.activeLayer == 8) {
            ofSetColor(255,255,0);
            geosphere.drawDepth(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            biosphere.getAnimalLife().draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
            ofDisableBlendMode();
        } else if (data.activeLayer == 9){
            ofSetColor(0, 255);
            ofRect(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
            ofEnableAlphaBlending();
            ofSetColor(172);
            blobImage.draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
            ofSetColor(255);
            atmosphere.draw(-width*0.5 + data.center.x ,-height*0.5 + data.center.y,width,height);
            ofDisableBlendMode();
        }
		ofPopMatrix();
		
	} else {
		cam.begin();
		ofEnableLighting();
		light.enable();
		
		drawTerrain();
		
		light.disable();
		ofDisableLighting();
		cam.end();
	}
	
	if (bMask){
		ofPushStyle();
		
		ofPushMatrix();
		ofTranslate(ofGetWindowWidth()*0.5, ofGetWindowHeight()*0.5);
		ofScale(scale, scale,1);
		
		for(int i = 0; i < data.maskCorners.size(); i++){
			ofNoFill();
			
			if ( data.maskCornersSelected == i){
				ofFill();
				ofSetColor(255, 0, 0,100);
			} else {
				ofSetColor(255, 0, 0,250);

				if ( ofDist(mouse.x, mouse.y, data.maskCorners[i].x, data.maskCorners[i].y) <= 10 )
					ofFill(); 
			}
			
			ofCircle( data.maskCorners[i].x - width*0.5 + data.center.x ,data.maskCorners[i].y - height*0.5 + data.center.y, 10);
		}
		
		ofPopMatrix();
		ofPopStyle();
	}

	ofDisableAlphaBlending();
	
	if (bDebug){
		// Atmosphere System
		ofFill();
		ofSetColor(0, 255);
		ofRect(data.atmosphereLoc.x, data.atmosphereLoc.y, width*0.5, height*0.5);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofSetColor(100,255);
		depthFloatImage.draw(data.atmosphereLoc.x, data.atmosphereLoc.y, width*0.5, height*0.5);
		ofSetColor(255,255);
        atmosphere.draw(data.atmosphereLoc.x, data.atmosphereLoc.y, width*0.5, height*0.5);
		ofDisableBlendMode();
		
		// Geosphere System
		ofFill();
		ofSetColor(255,255);
		geosphere.draw(data.geosphereLoc.x, data.geosphereLoc.y,width*0.5, height*0.5);
		
		// Hidrosphere System
		ofSetColor(255,255);
		geosphere.drawDepth(data.hydrosphereLoc.x,data.hydrosphereLoc.y, width*0.5, height*0.5);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
		hydrosphere.draw(data.hydrosphereLoc.x,data.hydrosphereLoc.y,width*0.5,height*0.5);
        ofDisableBlendMode();
		
		// Biosphere System
		ofSetColor(150,255);
		geosphere.drawDepth(data.biosphereLoc.x,data.biosphereLoc.y, width*0.5, height*0.5);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofSetColor(255,255);
		biosphere.draw(data.biosphereLoc.x,data.biosphereLoc.y,width*0.5,height*0.5);
		ofDisableBlendMode();
	}
	
	ofPushStyle();
	if (bGui)
		gui->draw();
	ofPopStyle();
	
	//ofEnableAlphaBlending();
	//ofSetColor(255, 255, 255,255);
} 

void ofxEden::drawTerrain(){
	terrain.loadHeightmap( geosphere.getAverage(), geosphere.getNormal(), textures.getTextureReference(), data.terrainResolution, 100);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	
	ofPushMatrix();
	ofTranslate(data.center.x, 0.0 ,data.center.y); 
	
	ofSetColor(255);
	
	textures.getTextureReference().bind();
	
	glEnable(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);
	terrain.draw();
	
	textures.getTextureReference().unbind();
	
	ofPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
}

//-------------------------------------------------------------- Events
void ofxEden::_keyPressed(ofKeyEventArgs &e){
	switch (e.key) {
		case 's':
			bGui = !bGui;
			break;
		case 'd':
            if (bMask){
                if(data.maskCornersSelected != -1)
                    data.delMaskPoint(data.maskCornersSelected);
            } else
                bDebug = !bDebug;
			break;
		case 't':
			bTerrain = !bTerrain;
			break;
		case 'm':
			bMask = !bMask;
			break;
        case 'a':
            if (bMask)
                data.addMaskPoint(mouse.x, mouse.y);
			break;
        case 'p':
            bMouse = !bMouse;
            if (bMouse)
                ofShowCursor();
            else
                ofHideCursor();
		case OF_KEY_LEFT:
			data.center.x--;
			data.saveXml();
			break;
		case OF_KEY_RIGHT:
			data.center.x++;
			data.saveXml();
			break;
		case OF_KEY_UP:
			data.center.y--;
			data.saveXml();
			break;
		case OF_KEY_DOWN:
			data.center.y++;
			data.saveXml();
			break;
		case ',':
			data.scale -= 0.001;
			data.saveXml();
			break;
		case '.':
			data.scale += 0.001;
			data.saveXml();
			break;
        case '0':
            data.activeLayer = 0;
            break;
        case '1':
            data.activeLayer = 1;
            break;
        case '2':
            data.activeLayer = 2;
            break;
        case '3':
            data.activeLayer = 3;
            break;
        case '4':
            data.activeLayer = 4;
            break;
        case '5':
            data.activeLayer = 5;
            break;
        case '6':
            data.activeLayer = 6;
            break;
        case '7':
            data.activeLayer = 7;
            break;
        case '8':
            data.activeLayer = 8;
            break;
        case '9':
            data.activeLayer = 9;
            break;
	}
}

void ofxEden::_keyReleased(ofKeyEventArgs &e){
}

void ofxEden::_mouseMoved(ofMouseEventArgs &e){
	if (bMask){
		mouse.x = ( e.x - ofGetWindowWidth()*0.5)/scale  + width*0.5 - data.center.x;
		mouse.y = ( e.y - ofGetWindowHeight()*0.5)/scale + height*0.5 - data.center.y;
	}
}

void ofxEden::_mousePressed(ofMouseEventArgs &e){
	if (bGui)
		gui->mousePressed( e.x, e.y, e.button );
	else if ( bDebug ){
		if ( (e.x > data.atmosphereLoc.x ) && (e.x < data.atmosphereLoc.x + width*0.5 ) &&
			(e.y > data.atmosphereLoc.y ) && (e.y < data.atmosphereLoc.y + height*0.5) )
			bGui = true;
		if ( (e.x > data.geosphereLoc.x ) && (e.x < data.geosphereLoc.x + width*0.5 ) &&
			(e.y > data.geosphereLoc.y ) && (e.y < data.geosphereLoc.y + height*0.5) )
			bGui = true;
		if ( (e.x > data.hydrosphereLoc.x ) && (e.x < data.hydrosphereLoc.x + width*0.5 ) &&
			(e.y > data.hydrosphereLoc.y ) && (e.y < data.hydrosphereLoc.y + height*0.5) )
			bGui = true;
		if ( (e.x > data.biosphereLoc.x ) && (e.x < data.biosphereLoc.x + width*0.5 ) &&
			(e.y > data.biosphereLoc.y ) && (e.y < data.biosphereLoc.y + height*0.5) )
			bGui = true;
	} else if ( bMask) {
		mouse.x = (e.x - ofGetWindowWidth()*0.5)/scale  + width*0.5 - data.center.x;
		mouse.y = (e.y - ofGetWindowHeight()*0.5)/scale + height*0.5 - data.center.y;
		
		for(int i = 0; i < data.maskCorners.size(); i++){
			if ( ofDist(mouse.x, mouse.y, data.maskCorners[i].x, data.maskCorners[i].y) <= 10 )
				data.maskCornersSelected = i;
		}
	
	}
};

void ofxEden::_mouseDragged(ofMouseEventArgs &e){
	if (bGui)
		gui->mouseDragged(e.x, e.y, e.button);
	else if ( bMask && ( data.maskCornersSelected != -1 ) ) {
		mouse.x = (e.x - ofGetWindowWidth()*0.5)/scale + width*0.5 - data.center.x;
		mouse.y = (e.y - ofGetWindowHeight()*0.5)/scale + height*0.5 - data.center.y;
		
		data.maskCorners[data.maskCornersSelected].x = mouse.x;
		data.maskCorners[data.maskCornersSelected].y = mouse.y;
	}

};

void ofxEden::_mouseReleased(ofMouseEventArgs &e){
	if (bGui)
		gui->mouseReleased(e.x, e.y, 0);
	
	if (bMask)
		if (data.maskCornersSelected != -1) {
			data.maskCornersSelected = -1;
			data.saveXml();
		}
	
}

void ofxEden::_exit(ofEventArgs &e){
}