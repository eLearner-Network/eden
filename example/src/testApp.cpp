#include "testApp.h"

//-------------------------------------------------------------- SETING
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetVerticalSync(false);
    
    ofSetDataPathRoot("data/");
    
    ofLog(OF_LOG_NOTICE, "Loading ofxComposer");
    composer.load("config.xml");
    
    ofLog(OF_LOG_NOTICE, "Loading xml data Settings");
	data.load();
    
    ofLog(OF_LOG_NOTICE, "Loading Kinect Drivers");
    kinect.init();
    kinect.setRegistration(true);
    kinect.open();
    
    width = 640;
	height = 480;
	numPixels = width * height;
    
    ofLog(OF_LOG_NOTICE, "Allocating INPUT images");
	blobImage.allocate(width,height);
	depthFloatImage.allocate(width, height,OF_IMAGE_GRAYSCALE);
    
    ofLog(OF_LOG_NOTICE, "Starting Atmosphere");
	atmosphere.linkData(&data);
	atmosphere.allocate(width,height, data.atmosphereResolution );
	
    ofLog(OF_LOG_NOTICE, "Starting Geosphere");
	geosphere.linkData(&data);
	geosphere.allocate(width,height);
	
    ofLog(OF_LOG_NOTICE, "Starting Hydrosphere");
	hydrosphere.linkData(&data);
	hydrosphere.allocate(width,height);
	
    ofLog(OF_LOG_NOTICE, "Starting Biosphere");
	biosphere.linkData(&data);
	biosphere.allocate(width,height);
    biosphere.flock.linkData(&data);
	
    ofLog(OF_LOG_NOTICE, "Starting multi-texturing mapping");
	textures.linkData(&data);
	textures.allocate(width*2, height*2);
    
    light.setPosition(ofVec3f(0,800,-100));
	//light.setDiffuseColor(ofColor(192, 160, 128));
	cam.setDistance(3000);
    
    bEdit = false;
    bTerrain = false;
    bCalibrated = true;
    bMouse = false;
    
    ofHideCursor();
    ofSetFullscreen(true);
    composer.setEdit(false);
}

void testApp::calibrate(){
    ofSetFullscreen(true);
    
    bCalibrated = false;
    calibrator.init(&kinect);
}

//-------------------------------------------------------------- LOOP
void testApp::update(){
    data.update();
    kinect.update();

    composer[1]->setTexture(kinect.getTextureReference());
    composer[3]->setTexture(kinect.getDepthTextureReference());
    
    if(kinect.isFrameNew()) {
        
        if (!bCalibrated){
            bCalibrated = calibrator.update( composer[2] );
            
            if (bCalibrated){
                float diff = data.lowAltitude - data.topAltitude;
                
                data.lowAltitude = calibrator.getSurfaceDistance();
                //data.topAltitude = data.lowAltitude - diff;
                data.topAltitude = calibrator.getCleanDistance();
            }
            
        } else {
            kinect.setDepthClipping(data.topAltitude,data.lowAltitude);
            
            float *depthRaw = kinect.getDistancePixels();
            
            numPixels = width * height; 
            
            unsigned char * blobPixels = blobImage.getPixels();
            float * depthFloatPixels = depthFloatImage.getPixels();
            
            // ... and extract the range it neads
            int farThreshold = data.topAltitude; //minDist;
            int nearThreshold = data.lowAltitude; //maxDist;
            
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
            depthFloatImage.setFromPixels(depthFloatPixels, width, height, OF_IMAGE_GRAYSCALE );
            depthFloatImage.reloadTexture();
            
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
            
            if (data.activeLayer == 0)
                composer[2]->setTexture(textures.getTextureReference());
            else if (data.activeLayer == 1)
                composer[2]->setTexture( depthFloatImage.getTextureReference());
            else if (data.activeLayer == 2)
                composer[2]->setTexture( geosphere.getAverage() );
            else if (data.activeLayer == 3)
                composer[2]->setTexture( geosphere.getNormal() );
            else if (data.activeLayer == 4)
                composer[2]->setTexture( geosphere.getNoise() );
            else if (data.activeLayer == 5)
                composer[2]->setTexture( geosphere.getConditions() );
            else if (data.activeLayer == 6)
                composer[2]->setTexture( hydrosphere.getTextureReference() );
            else if (data.activeLayer == 7)
                composer[2]->setTexture( biosphere.getTextureReference() );
            else if (data.activeLayer == 8)
                composer[2]->setTexture( biosphere.getAnimalLife() );
            else if (data.activeLayer == 9)
                composer[2]->setTexture( atmosphere.getTextureReference() );
        }
        
        if (bTerrain){
            ofFloatPixels heightMap;
            heightMap.allocate(geosphere.getAverage().getWidth(), geosphere.getAverage().getHeight(), OF_PIXELS_RGBA);
            geosphere.getAverage().readToPixels(heightMap);
            
            ofFloatPixels norms;
            norms.allocate(geosphere.getNormal().getWidth(), geosphere.getNormal().getHeight(), OF_PIXELS_RGBA);
            geosphere.getNormal().readToPixels(norms);
            
            int width = heightMap.getWidth();
            int height = heightMap.getHeight();
            float flResolution = data.terrainResolution;
            float flHeightScale = (data.lowAltitude - data.topAltitude)*0.5;
            float textureScale = 1.0;
            
            if (data.activeLayer == 0)
                textureScale = 2.0;
            
            
            // Generate Vertex Field
            nVertexCount = (int) ( width * height * 6 / ( flResolution * flResolution ) );
            
            pVertices	= new ofVec3f[nVertexCount];		// Allocate Vertex Data
            pTexCoords	= new ofVec2f[nVertexCount];		// Allocate Tex Coord Data
            pNormals	= new ofVec3f[nVertexCount];		// Allocate Normals
            
            int nX, nZ, nTri, nIndex=0;                     // Create Variables
            float flX, flZ;
            
            for( nZ = 0; nZ < height-flResolution ; nZ += (int) flResolution ){
                for( nX = 0; nX < width-flResolution ; nX += (int) flResolution ){
                    for( nTri = 0; nTri < 6; nTri++ ){
                        // Using This Quick Hack, Figure The X,Z Position Of The Point
                        flX = (float) nX + ( ( nTri == 1 || nTri == 2 || nTri == 5 ) ? flResolution : 0.0f );
                        flZ = (float) nZ + ( ( nTri == 2 || nTri == 4 || nTri == 5 ) ? flResolution : 0.0f );
                        
                        // Set The Data, Using PtHeight To Obtain The Y Value
                        pVertices[nIndex].x = flX - ( width *0.5 );
                        pVertices[nIndex].y = heightMap.getColor((int)flX, (int)flZ).r * flHeightScale;
                        pVertices[nIndex].z = flZ - ( height *0.5 );
                        
                        // 3	0 --- 1		nTri reference
                        // | \	  \	  |
                        // |   \	\ |
                        // 4 --- 5	  2
                        
                        // Stretch The Texture Across The Entire Mesh
                        pTexCoords[nIndex].x = flX * textureScale;// / width;
                        pTexCoords[nIndex].y = flZ * textureScale;// / height;
                        
                        // Normals by vert
                        pNormals[nIndex].x = norms.getColor((int)flX, (int)flZ).r;
                        pNormals[nIndex].y = norms.getColor((int)flX, (int)flZ).b;
                        pNormals[nIndex].z = norms.getColor((int)flX, (int)flZ).g;
                        
                        // Increment Our Index
                        nIndex++;
                    }				
                }
            }
            
            vbo.setVertexData(pVertices, nVertexCount, GL_STREAM_DRAW);
            vbo.setNormalData(pNormals, nVertexCount, GL_STREAM_DRAW);
            vbo.setTexCoordData(pTexCoords, nVertexCount, GL_STREAM_DRAW);
            
            // Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
            delete [] pVertices; pVertices = NULL;
            delete [] pTexCoords; pTexCoords = NULL;
            delete [] pNormals; pNormals = NULL;
        }
    }
    
    composer.update();
    
    ofSetWindowTitle( ofToString( ofGetFrameRate()));
}


void testApp::draw(){
    if(bEdit)    
        ofBackgroundGradient(ofColor::gray, ofColor::black);
    else
        ofBackground(ofColor::black);

    if (bCalibrated && bTerrain){
        ofPushMatrix();
        
        ofSetColor(255,255);
        
		cam.begin();
		ofEnableLighting();
		light.enable();
        
		drawTerrain();
		
		light.disable();
		ofDisableLighting();
		cam.end();
        ofPopMatrix();
	}
    
    ofSetColor(255,255);
    composer.draw();
    
    if(bEdit)
        data.draw();
}

void testApp::exit(){
    data.save();
    kinect.close();
}

void testApp::drawTerrain(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	
	ofPushMatrix();
	
	ofSetColor(255);
	
	composer[2]->getTextureReference().bind();
    ofEnableAlphaBlending();
	
	glEnable(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);
	vbo.draw(GL_TRIANGLES , 0, nVertexCount); 
	
	composer[2]->getTextureReference().unbind();
	
	ofPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
}

//-------------------------------------------------------------- EVENTS
void testApp::keyPressed(int key){
    if( key == 'e' ){
		bEdit = !bEdit;
	} else if ( ( key >= 48 ) && ( key <= 57 ) ) {
        data.activeLayer = key - 48;
    } else if (key == 't'){
        bTerrain = !bTerrain;
    } else if (key == 'f'){
        ofToggleFullscreen();
    } else if (key == 'c'){
        calibrate();
    } else if (key == 'm'){
        bMouse = !bMouse;
        
        if (bMouse){
            ofShowCursor();
        } else {
            ofHideCursor();
        }
    }
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button){
  
}

void testApp::mouseReleased(int x, int y, int button){
    
}

void testApp::windowResized(int w, int h){
    
}

void testApp::gotMessage(ofMessage msg){
}


void testApp::dragEvent(ofDragInfo dragInfo){
    /*
    if( dragInfo.files.size() > 0 ){
		for(int i = 0; i < dragInfo.files.size(); i++){
            composer.addPatch( dragInfo.files[i], dragInfo.position );
		}
	}*/
}