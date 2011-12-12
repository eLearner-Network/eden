/*
 *  ofxEden.h
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 16/08/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#ifndef OFXEDEN
#define OFXEDEN

#include "ofMain.h"

#include "ofxGui.h"
#include "ofxOpenNI.h"
#include "ofxOpenCv.h"

#include "ofxEdenData.h"

#include "ofxEdenAtmosphere.h"
#include "ofxEdenGeosphere.h"
#include "ofxEdenHydrosphere.h"
#include "ofxEdenBiosphere.h"

#include "ofxEdenTextures.h"

#include "ofxTerrain.h"

class ofxEden : public ofxGuiListener {

// ofxGUI setup stuff
enum {
	atmospherePanel,
	atmospherePanel_circular_force,
	atmospherePanel_circular_angle,
    atmospherePanel_temperature_dissipation,
    atmospherePanel_velocity_dissipation,
    atmospherePanel_density_dissipation,
	
	geospherePanel,
	geospherePanel_top_altitude,
	geospherePanel_low_altitude,
	geospherePanel_terrain_resolution,
	
	hydrospherePanel,
	hydrospherePanel_water_level,
	hydrospherePanel_absortion_soil,
	hydrospherePanel_depresion_flow,
	hydrospherePanel_precipitation_humidity,
	hydrospherePanel_precipitation_amount,
	hydrospherePanel_precipitation_inclination,
	hydrospherePanel_precipitation_altitud,
	hydrospherePanel_precipitation_cold,
	
	biospherePanel,
	biospherePanel_diffU,
	biospherePanel_diffV,
	biospherePanel_f,
	biospherePanel_k,
    biospherePanel_Separation,
    biospherePanel_Alineation,
    biospherePanel_Cohesion,
    biospherePanel_MaxDist,
    biospherePanel_MinDist,
    biospherePanel_MaxSpeed,
    biospherePanel_MaxForce,
    biospherePanel_Borders
};
	
public:
	// Constructor/Destructor
	ofxEden();
	
	//Basic Events called every frame
    void _setup(ofEventArgs &e);
    void _update(ofEventArgs &e);
	void _draw(ofEventArgs &e);
    void _exit(ofEventArgs &e);
    //Mouse Events
	void _mouseMoved(ofMouseEventArgs &e);
    void _mousePressed(ofMouseEventArgs &e);
    void _mouseDragged(ofMouseEventArgs &e);
    void _mouseReleased(ofMouseEventArgs &e);
    //Key Events
    void _keyPressed(ofKeyEventArgs &e);
    void _keyReleased(ofKeyEventArgs &e);
	
	// Render
	void drawTerrain();
	
private:
	
	// INIT
	// ---------------------------------------------
	// ofxGUI
	ofxGui*			gui;
	void			setupGui();
	void			handleGui(int parameterId, int task, void* data, int length);
	
	// INPUT
	// ---------------------------------------------
	// Kinect ( openNI -> openCV )
	ofxOpenNI           openNI;
	ofxCvGrayscaleImage	blobImage;			// This goes to Atmosphere System
    ofFloatImage		depthFloatImage;	// This goes to Geosphere Syste
	
	// PROCESSING
	// ---------------------------------------------
	ofxEdenAtmosphere	atmosphere;		// Atmosphere System ( CPU contourFinder + CPU fingerTracking + CPU fluidSolver + GPU fluidDrawing )
	ofxEdenGeosphere	geosphere;		// Geosphere System ( GPU based )
	ofxEdenHydrosphere	hydrosphere;	// Hydrosphere System ( GPU ParticleSystem )
	ofxEdenBiosphere	biosphere;		// Biosphere System ( GPU GrayScott )
	ofxEdenTextures		textures;		// Final Post-Processing Layer
	
	// Tools
	ofLight				light;
	ofEasyCam			cam;
	ofxTerrain			terrain;		// Terrain (VBO Mesh)
	
    // INFO
	ofxEdenData		data;
    ofPoint			mouse;
	float			scale;
	int				width,height, numPixels;
	bool			bGui, bDebug, bTerrain, bMask, bMouse;
	
};

#endif