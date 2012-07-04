//
//  ofxEdenData.cpp
//  ofxEden
//
//  Created by Patricio González Vivo on 30/10/11.
//  Copyright (c) 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#include "ofxEdenData.h"

ofxEdenData::ofxEdenData()
{ 
    activeLayer = 0; // 0 -> ALL layers
}

void ofxEdenData::load(){
    gui.setPosition(0, 0);
    gui.setup("Panel");
    gui.add(atmosphereCircularForce.setup("Circ. force", 1.0, 0.0, 1.0 ));
    gui.add(atmosphereCircularAngle.setup("Circ. angle", -82.9286, 0, -90.0));
    gui.add(atmosphereTempDiss.setup("Temp. Diss.", 0.871429, 0.75, 1.0));
    gui.add(atmosphereVelDiss.setup("Vel. Diss.", 0.928571, 0.75, 1.0));
    gui.add(atmosphereDenDiss.setup("Den. Diss.", 0.999, 0.75, 1.0));
    atmosphereResolution = 10;
    
    gui.add(topAltitude.setup("Higher point", 735, 500, 1000));
    gui.add(lowAltitude.setup("Lower point", 1005, 600, 1500));
    gui.add(terrainResolution.setup("Terrain res.", 8, 1, 10));
    totalFrames = 6;
    
    gui.add(waterLevel.setup("Water level", 0.0428571, 0.0f, 1.0f));
    gui.add(precipitationAmount.setup("Amount of rain", 1.02857, 0.0f, 2.0f));
    gui.add(precipitationHumidity.setup("Min humidity", 0.15, 0.0f, 1.0f));
    gui.add(precipitationCold.setup("Min cold", 0.221429, 0.0f, 1.0f));
    gui.add(precipitationInclination.setup("Min Inclin.", 0.471429, 0.0f, 1.0f));
    gui.add(precipitationAltitud.setup("Min Alt.", 0.207143, 0.0f, 1.0f));
    gui.add(absortionSoil.setup("Soil Absor.", 0.121429, 0.0f, 1.0f));
    gui.add(depresionFlow.setup("Flow on Dep.", 0.442857, 0.0f, 1.0f));
    
    gui.add(biosphereDiffU.setup("Diff U", 0.239286, 0.0, 0.5));
    gui.add(biosphereDiffV.setup("Diff V", 0.0535714, 0.0, 0.5));
    gui.add(biosphereF.setup("F", 0.0285714, 0.0, 0.5));
    gui.add(biosphereK.setup("K", 0.0821429, 0.0, 0.5));
    gui.add(biosphereMinDist.setup("Min Dist", 0.0771429, 0.0, 1.0));
    gui.add(biosphereMaxDist.setup("Max Dist", 0.0114286, 0.0, 1.0));
    gui.add(biosphereMaxSpeed.setup("Max Speed", 3, 0.0, 6.0));
    gui.add(biosphereMaxForce.setup("Max Force", 0.05, 0.0, 0.1));
    gui.add(biosphereSeparation.setup("Separation", 2.95714, 0.0, 3.0));
    gui.add(biosphereAlineation.setup("Alineation", 1.0, 0.0, 3.0));
    gui.add(biosphereCohesion.setup("Cohesion", 0.985714, 0.0, 3.0));
    gui.add(biosphereBorders.setup("Border", 0.025, 0.0, 1.0));
    biosphereTimeStep = 0.005;
    biosphereFlat = 1.0; 
    biosphereFood = 1.5;
    
    gui.loadFromFile("settings.xml");
}

void ofxEdenData::save(){
    gui.saveToFile("settings.xml");
}

void ofxEdenData::update(){
	
}

void ofxEdenData::draw(){
    gui.draw();
}
