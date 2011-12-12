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
    shaderDir = "shader/"; 
    maskCornersSelected = -1;
    receiver.setup( SERVER_PORT );
    outHost = "127.0.0.1";
    outPort = 0;
    activeLayer = 0; // 0 -> ALL layers
}

void ofxEdenData::loadXml(string filePath)
{
    ofxXmlSettings XML;
    
    if (XML.loadFile(filePath)){
        maskCorners.clear();
        
        // Atmosphere
        atmosphereLoc.x = XML.getValue("atmosphere:x",0);
        atmosphereLoc.y = XML.getValue("atmosphere:y",0);
        atmosphereResolution = XML.getValue("atmosphere:resolution",8);
        atmosphereCircularForce = XML.getValue("atmosphere:circular:force",1.0);
        atmosphereCircularAngle = XML.getValue("atmosphere:circular:angle",-2.0);
        atmosphereTempDiss = XML.getValue("atmosphere:temperature:dissipation",0.99f);
        atmosphereVelDiss = XML.getValue("atmosphere:velocity:dissipation",0.9f);
        atmosphereDenDiss = XML.getValue("atmosphere:density:dissipation",0.999f);
        
        // Geosphere
        geosphereLoc.x = XML.getValue("geosphere:x",0);
        geosphereLoc.y = XML.getValue("geosphere:y",0);
        topAltitude	= XML.getValue("geosphere:top",0);
        lowAltitude	= XML.getValue("geosphere:low",0);
        totalFrames = XML.getValue("geosphere:average:frames",0);
        terrainResolution = XML.getValue("geosphere:terrain:resolution",8);
        
        // Hidrosphere
        hydrosphereLoc.x = XML.getValue("hydrosphere:x",0);
        hydrosphereLoc.y = XML.getValue("hydrosphere:y",0);
        waterLevel = XML.getValue("hydrosphere:water:level",0.1);
        
        absortionSoil = XML.getValue("hydrosphere:absortion:soil",0.01);
        depresionFlow = XML.getValue("hydrosphere:flow:depresion",0.03);
        
        precipitationAltitud = XML.getValue("hydrosphere:precipitation:necesary:altitud",0.0);
        precipitationInclination = XML.getValue("hydrosphere:precipitation:necesary:inclination",1.0);
        
        precipitationCold = XML.getValue("hydrosphere:precipitation:necesary:cold",0.1);
        precipitationHumidity = XML.getValue("hydrosphere:precipitation:necesary:humidity",0.1);
        precipitationAmount = XML.getValue("hydrosphere:precipitation:amount",0.1);
        
        // Biosphere
        biosphereLoc.x = XML.getValue("biosphere:vegetation:x",0);
        biosphereLoc.y = XML.getValue("biosphere:vegetation:y",0);
        biosphereDiffU = XML.getValue("biosphere:vegetation:diffU",0.25);
        biosphereDiffV = XML.getValue("biosphere:vegetation:diffV",0.04);
        biosphereK = XML.getValue("biosphere:vegetation:f",0.1);
        biosphereF = XML.getValue("biosphere:vegetation:k",0.047);
        
        biosphereTimeStep = XML.getValue("biosphere:flock:TimeStep",0.005f);
        biosphereMaxDist = XML.getValue("biosphere:flock:maxDist",0.030f);
        biosphereMinDist = XML.getValue("biosphere:flock:minDist",0.025f);
        biosphereMaxSpeed = XML.getValue("biosphere:flock:maxSpeed",3.0f);
        biosphereMaxForce = XML.getValue("biosphere:flock:maxForce",0.005f);
        
        biosphereSeparation = XML.getValue("biosphere:flock:separation",1.5f);
        biosphereAlineation = XML.getValue("biosphere:flock:alineation",1.0f);
        biosphereCohesion = XML.getValue("biosphere:flock:cohesion",1.0f);
        biosphereBorders = XML.getValue("biosphere:flock:borders",1.0f);
        //biosphereFlat = XML.getValue("biosphere:flock:flat",1.0f);
        //biosphereFood = XML.getValue("biosphere:flock:food",1.0);
        
        // Textures
        XML.pushTag("textures");
        scale = XML.getValue("scale",1.0); 
        center.x = XML.getValue("center:x",0); 
        center.y = XML.getValue("center:y",0); 
        
        XML.pushTag("mask");
        int totalMaskCorners = XML.getNumTags("point");
        for(int i = 0; i < totalMaskCorners; i++){
            XML.pushTag("point",i);
            addMaskPoint(XML.getValue("x", 0.0),XML.getValue("y", 0.0));
            XML.popTag();
        }
        XML.popTag();
        XML.popTag();
        cout << maskCorners.size() << " total masking points" << endl;
        
    } else
        cout << " [ FAIL ]" << endl;
}

void ofxEdenData::saveXml(string filePath)
{
    ofxXmlSettings XML;
    
    if (XML.loadFile(filePath)){
        // Atmosphere
        XML.setValue("atmosphere:circular:force",atmosphereCircularForce);
        XML.setValue("atmosphere:circular:angle",atmosphereCircularAngle);
        XML.setValue("atmosphere:temperature:dissipation",atmosphereTempDiss);
        XML.setValue("atmosphere:velocity:dissipation",atmosphereVelDiss);
        XML.setValue("atmosphere:density:dissipation",atmosphereDenDiss);
        
        // Geosphere
        XML.setValue("geosphere:top",topAltitude);
        XML.setValue("geosphere:low",lowAltitude);
        XML.setValue("geosphere:terrain:resolution",terrainResolution);
        
        // Hydrosphere
        XML.setValue("hydrosphere:water:level",waterLevel);
        XML.setValue("hydrosphere:absortion:soil",absortionSoil);
        XML.setValue("hydrosphere:flow:depresion",depresionFlow);
        XML.setValue("hydrosphere:precipitation:necesary:altitud",precipitationAltitud);
        XML.setValue("hydrosphere:precipitation:necesary:inclination",precipitationInclination);
        XML.setValue("hydrosphere:precipitation:necesary:cold",precipitationCold);
        XML.setValue("hydrosphere:precipitation:necesary:humidity",precipitationHumidity);
        XML.setValue("hydrosphere:precipitation:amount",precipitationAmount);
        
        // Biosphere
        XML.setValue("biosphere:vegetation:diffU",biosphereDiffU);
        XML.setValue("biosphere:vegetation:diffV",biosphereDiffV);
        XML.setValue("biosphere:vegetation:f",biosphereK);
        XML.setValue("biosphere:vegetation:k",biosphereF);
        
        XML.setValue("biosphere:flock:TimeStep",biosphereTimeStep);
        XML.setValue("biosphere:flock:maxDist",biosphereMaxDist);
        XML.setValue("biosphere:flock:minDist",biosphereMinDist);
        XML.setValue("biosphere:flock:maxSpeed",biosphereMaxSpeed);
        XML.setValue("biosphere:flock:maxForce",biosphereMaxForce);
        
        XML.setValue("biosphere:flock:separation",biosphereSeparation);
        XML.setValue("biosphere:flock:alineation",biosphereAlineation);
        XML.setValue("biosphere:flock:cohesion",biosphereCohesion);
        XML.setValue("biosphere:flock:borders",biosphereBorders);
        //XML.setValue("biosphere:flock:flat",biosphereFlat);
        //XML.setValue("biosphere:flock:food",biosphereFood);
        
        // Textures
        XML.pushTag("textures");
        XML.setValue("scale",scale); 
        XML.setValue("center:x",center.x); 
        XML.setValue("center:y",center.y);
        
        XML.pushTag("mask");
        int totalMaskCorners = XML.getNumTags("point");
        
        for(int i = 0; i < maskCorners.size(); i++){
            int tagNum = i;
            
            if (i >= totalMaskCorners)
                tagNum = XML.addTag("point");
            
            XML.setValue("point:x",(float)maskCorners[i].x, tagNum);
            XML.setValue("point:y",(float)maskCorners[i].y, tagNum);
        }
        
        if (maskCorners.size() < totalMaskCorners){
            for(int i = maskCorners.size() - 1; i < totalMaskCorners; i++){
                XML.setValue("point:x",0, i);
                XML.setValue("point:y",0, i);
            }
        }
        
        XML.popTag();
        XML.popTag();
    } else
        cout << " [ FAIL ]" << endl;
    XML.saveFile();
}

void ofxEdenData::addMaskPoint(float _x, float _y)
{
    ofPoint p;
    p.set(_x,_y);
    maskCorners.push_back(p);
}

void ofxEdenData::delMaskPoint(int _nPoint)
{
    maskCorners.erase(maskCorners.begin()+_nPoint);
}

void ofxEdenData::update()
{
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
            }
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
            ofxOscMessage outM;
            outM.setAddress( "/calibration" );
            outM.addFloatArg( center.x / 640 );       // 0
            outM.addFloatArg( center.y / 480 );       // 1
            outM.addFloatArg( scale );          // 2
            sender.sendMessage( outM );
        }
            
        // SET INFO
        if ( inM.getAddress() == "/set/point" ){
            int index = inM.getArgAsInt32(0);
            float x = inM.getArgAsFloat(1);
            float y = inM.getArgAsFloat(2);
            
            maskCorners[index].x = x * 640;
            maskCorners[index].y = y * 480;
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
            biosphereBorders = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/scale")
            scale = inM.getArgAsFloat(0);
        else if ( inM.getAddress() == "/set/center"){
            center.x = inM.getArgAsFloat(0) * 640;
            center.y = inM.getArgAsFloat(1) * 480;
        } else if (inM.getAddress() == "/save")
            saveXml();
        else if (inM.getAddress() == "/reload")
            loadXml();
	}
}
