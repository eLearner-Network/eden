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
    receiver.setup( SERVER_PORT );
    outHost = "127.0.0.1";
    outPort = 0;
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
	while( receiver.hasWaitingMessages() ){
		ofxOscMessage inM;
		receiver.getNextMessage( &inM );
        
        // MAKE CONECTION
        if (outHost != inM.getRemoteIp()){
            outHost = inM.getRemoteIp();
            sender.setup( outHost , CLIENT_PORT );
        }
        
        // GET INFO
		if ( inM.getAddress() == "/get/points" ){
            /*
            ofxOscMessage outM;
            outM.setAddress( "/points" );
            outM.addIntArg(maskCorners.size());
            sender.sendMessage( outM );
            
            for(int i = 0; i < maskCorners.size(); i++){
                ofxOscMessage pointOut;
                pointOut.setAddress( "/point" );
                float x = maskCorners[i].x / 640;
                float y = maskCorners[i].y / 480;
                
                pointOut.addIntArg(i);
                pointOut.addFloatArg(x);
                pointOut.addFloatArg(y);
                
                sender.sendMessage( pointOut );
            }*/
		} else if ( inM.getAddress() == "/get/altitudes" ){
            ofxOscMessage outM;
            outM.setAddress( "/altitudes" );
            outM.addIntArg(topAltitude);
            outM.addIntArg(lowAltitude);
            sender.sendMessage( outM );
        } else if ( inM.getAddress() == "/get/activelayer" ){
            ofxOscMessage outM;
            outM.setAddress( "/activelayer" );
            outM.addIntArg(activeLayer);
            sender.sendMessage( outM );
        } else if ( inM.getAddress() == "/get/waterlevel" ){
            ofxOscMessage outM;
            outM.setAddress( "/waterlevel" );
            outM.addFloatArg(waterLevel);
            sender.sendMessage( outM );
        } else if ( inM.getAddress() == "/get/hydrosphere" ){
            ofxOscMessage outM;
            outM.setAddress( "/hydrosphere" );
            outM.addFloatArg( precipitationAltitud );       // 0
            outM.addFloatArg( precipitationAmount );        // 1
            outM.addFloatArg( precipitationCold );          // 2
            outM.addFloatArg( precipitationHumidity );      // 3
            outM.addFloatArg( precipitationInclination );   // 4
            outM.addFloatArg( absortionSoil );              // 5
            outM.addFloatArg( depresionFlow );              // 6
            sender.sendMessage( outM );
        } else if ( inM.getAddress() == "/get/atmosphere" ){
            ofxOscMessage outM;
            outM.setAddress( "/atmosphere" );
            outM.addFloatArg( atmosphereCircularForce );    // 0
            outM.addFloatArg( atmosphereCircularAngle );    // 1
            outM.addFloatArg( atmosphereVelDiss );          // 2
            outM.addFloatArg( atmosphereTempDiss );         // 3
            outM.addFloatArg( atmosphereDenDiss );          // 4
            sender.sendMessage( outM );
        } else if ( inM.getAddress() == "/get/vegetal" ){
            ofxOscMessage outM;
            outM.setAddress( "/vegetal" );
            outM.addFloatArg( biosphereDiffU );     // 0
            outM.addFloatArg( biosphereDiffV );     // 1
            outM.addFloatArg( biosphereF );         // 2
            outM.addFloatArg( biosphereK );         // 3
            sender.sendMessage( outM );
        } else if ( inM.getAddress() == "/get/animal" ){
            ofxOscMessage outM;
            outM.setAddress( "/animal" );
            outM.addFloatArg( biosphereMinDist );       // 1
            outM.addFloatArg( biosphereMaxDist );       // 2
            outM.addFloatArg( biosphereMaxSpeed );      // 3
            outM.addFloatArg( biosphereMaxForce );      // 4
            outM.addFloatArg( biosphereBorders );       // 5
            outM.addFloatArg( biosphereSeparation );    // 6
            outM.addFloatArg( biosphereAlineation );    // 7
            outM.addFloatArg( biosphereCohesion);       // 8
            sender.sendMessage( outM );
        } else if ( inM.getAddress() == "/get/calibration"){
            /*
            ofxOscMessage outM;
            outM.setAddress( "/calibration" );
            outM.addFloatArg( center.x / 640 );       // 0
            outM.addFloatArg( center.y / 480 );       // 1
            outM.addFloatArg( scale );          // 2
            sender.sendMessage( outM );*/
        }
            
        // SET INFO
        if ( inM.getAddress() == "/set/point" ){
            /*
            int index = inM.getArgAsInt32(0);
            float x = inM.getArgAsFloat(1);
            float y = inM.getArgAsFloat(2);
            
            maskCorners[index].x = x * 640;
            maskCorners[index].y = y * 480;*/
        } else if ( inM.getAddress() == "/set/lowaltitud" )
            lowAltitude = inM.getArgAsInt32(0);
        else if ( inM.getAddress() == "/set/topaltitud" )
            topAltitude = inM.getArgAsInt32(0);
        else if ( inM.getAddress() == "/set/layer" )
            activeLayer = inM.getArgAsInt32(0);
        else if ( inM.getAddress() == "/set/waterlevel")
            waterLevel = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/precipitationaltitud")
            precipitationAltitud = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/precipitationamount")
            precipitationAmount = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/precipitationcold")
            precipitationCold = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/precipitationhumidity")
            precipitationHumidity = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/precipitationinclination")
            precipitationInclination = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/absortionsoil")
            absortionSoil = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/depresionflow")
            depresionFlow = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/atmospherecircularangle")
            atmosphereCircularAngle = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/atmospherecircularforce")
            atmosphereCircularForce = inM.getArgAsFloat(0); 
        else if ( inM.getAddress() == "/set/atmospheretempdiss")
            atmosphereTempDiss = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/atmosphereveldiss")
            atmosphereVelDiss = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/atmospheredendiss")
            atmosphereDenDiss = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospherediffu")
            biosphereDiffU = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospherediffv")
            biosphereDiffV = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospherek")
            biosphereK = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospheref")
            biosphereF = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospheremindist")
            biosphereMinDist = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospheremaxdist")
            biosphereMaxDist = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospheremaxforce")
            biosphereMaxForce = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospheremaxspeed")
            biosphereMaxSpeed = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biosphereseparation")
            biosphereSeparation = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospherealineation")
            biosphereAlineation = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biospherecohesion")
            biosphereCohesion = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/biosphereborders")
            biosphereBorders = inM.getArgAsFloat(0);/*
        else if ( inM.getAddress() == "/set/scale")
            scale = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/center"){
            center.x = inM.getArgAsFloat(0) * 640;
            center.y = inM.getArgAsFloat(1) * 480;
        } */
        else if (inM.getAddress() == "/save")
            save();
        else if (inM.getAddress() == "/reload")
            load();
	}
}

void ofxEdenData::draw(){
    gui.draw();
}
