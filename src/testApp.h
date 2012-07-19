#pragma once

#include "ofMain.h"

#include "ofxComposer.h"

#include "ofxKinect.h"

#include "ofxOpenCv.h"

#include "ofxEdenData.h"
#include "ofxEdenAtmosphere.h"
#include "ofxEdenGeosphere.h"
#include "ofxEdenHydrosphere.h"
#include "ofxEdenBiosphere.h"
#include "ofxEdenTextures.h"

#include "ofxKinectAutoCalibrator.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    void calibrate();
    void drawTerrain();
    
    ofxComposer             composer;
    ofxKinectAutoCalibrator calibrator;
    
    ofxKinect kinect;
    ofxCvGrayscaleImage	blobImage;			// This goes to Atmosphere System
    ofFloatImage		depthFloatImage;	// This goes to Geosphere Syste
    
	// Processing
	// ---------------------------------------------
	ofxEdenAtmosphere	atmosphere;		// Atmosphere System ( CPU contourFinder + CPU fingerTracking + CPU fluidSolver + GPU fluidDrawing )
	ofxEdenGeosphere	geosphere;		// Geosphere System ( GPU based )
	ofxEdenHydrosphere	hydrosphere;	// Hydrosphere System ( GPU ParticleSystem )
	ofxEdenBiosphere	biosphere;		// Biosphere System ( GPU GrayScott )
	ofxEdenTextures		textures;		// Final Post-Processing Layer
    
    // Terrain
    // ---------------------------------------------
    ofLight				light;
	ofEasyCam			cam;
	ofVbo               vbo;
	ofVec3f*            pNormals;
    ofVec3f*            pVertices;      // Vertex Data
    ofVec2f*            pTexCoords;     // Texture Coordinates
	int                 nVertexCount;	// Vertex Count
    
    ofxEdenData         data;
    ofPoint             mouse;
	int                 width,height, numPixels;
    
    bool                bEdit, bTerrain, bCalibrated,bMouse;
};
