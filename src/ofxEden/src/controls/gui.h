/*
*  GUI.h
*
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef GUI_CONTROLS_H
#define GUI_CONTROLS_H

#include "ofxEden.h"

ofxEden  *appPtr;

void ofxEden::setupGui()
{
	appPtr = this;

	//panel border color
	gui->mGlobals->mBorderColor.r = 1.0;
	gui->mGlobals->mBorderColor.g = 1.0;
	gui->mGlobals->mBorderColor.b = 1.0;
	gui->mGlobals->mBorderColor.a = 0.4;
	//panel color
	gui->mGlobals->mCoverColor.r = 0.0;
	gui->mGlobals->mCoverColor.g = 0.0;
	gui->mGlobals->mCoverColor.b = 0.0;
	gui->mGlobals->mCoverColor.a = 0.5;
	//control outline color
	gui->mGlobals->mFrameColor.r = 1.0;
	gui->mGlobals->mFrameColor.g = 1.0;
	gui->mGlobals->mFrameColor.b = 1.0;
	gui->mGlobals->mFrameColor.a = 0.3;
	//text color
	gui->mGlobals->mTextColor.r = 1;
	gui->mGlobals->mTextColor.g = 1;
	gui->mGlobals->mTextColor.b = 1;
	gui->mGlobals->mTextColor.a = 1;
	//button color
	gui->mGlobals->mButtonColor.r = 0.0;
	gui->mGlobals->mButtonColor.g = 0.2;
	gui->mGlobals->mButtonColor.b = 0.9;
	gui->mGlobals->mButtonColor.a = 0.8;
	//slider tip color
	gui->mGlobals->mHandleColor.r = 0.1;
	gui->mGlobals->mHandleColor.g = 0.1;
	gui->mGlobals->mHandleColor.b = 0.1;
	//slider color
	gui->mGlobals->mSliderColor.r = 0.0;
	gui->mGlobals->mSliderColor.g = 0.3;
	gui->mGlobals->mSliderColor.b = 0.9;
	gui->mGlobals->mSliderColor.a = 0.8;

	// AtmosphereGUIPanel
	ofxGuiPanel * atmosphereGuiPannel = gui->addPanel(appPtr->atmospherePanel, "Atmosphere", data.atmosphereLoc.x, data.atmosphereLoc.y, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	atmosphereGuiPannel->addSlider(appPtr->atmospherePanel_circular_force, "Circular force", 140, 13, 0.0, 1.0, data.atmosphereCircularForce, kofxGui_Display_Float2, 0);
	atmosphereGuiPannel->addSlider(appPtr->atmospherePanel_circular_angle, "Circular angle", 140, 13, 0, -90.0, data.atmosphereCircularAngle, kofxGui_Display_Float2, 0);
    atmosphereGuiPannel->addSlider(appPtr->atmospherePanel_temperature_dissipation, "Temp. Dissipation", 140, 13, 0.75, 1.0, data.atmosphereTempDiss, kofxGui_Display_Float4, 0);
    atmosphereGuiPannel->addSlider(appPtr->atmospherePanel_velocity_dissipation, "Vel. Dissipation", 140, 13, 0.75, 1.0, data.atmosphereVelDiss, kofxGui_Display_Float4, 0);
    atmosphereGuiPannel->addSlider(appPtr->atmospherePanel_density_dissipation, "Den. Dissipation", 140, 13, 0.75, 1.0, data.atmosphereDenDiss, kofxGui_Display_Float4, 0);
	atmosphereGuiPannel->mObjWidth = width*0.5;
	atmosphereGuiPannel->mObjHeight = height*0.5;
	atmosphereGuiPannel->mObjects[1]->mObjX = 165;
	atmosphereGuiPannel->mObjects[1]->mObjY = 32;
    
    atmosphereGuiPannel->mObjects[2]->mObjY = 66;
	atmosphereGuiPannel->mObjects[3]->mObjX = 165;
	atmosphereGuiPannel->mObjects[3]->mObjY = 66;
    
	atmosphereGuiPannel->mObjects[4]->mObjY = 100;
	atmosphereGuiPannel->adjustToNewContent(100, 0);
	
	// GeosphereGUIPanel
	ofxGuiPanel * geosphereGuiPannel = gui->addPanel(appPtr->geospherePanel, "Geosphere", data.geosphereLoc.x, data.geosphereLoc.y, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	geosphereGuiPannel->addSlider(appPtr->geospherePanel_top_altitude, "Higher point", 140, 13, 500, 1000, data.topAltitude, kofxGui_Display_Int, 0);
	geosphereGuiPannel->addSlider(appPtr->geospherePanel_low_altitude, "Lower point", 140, 13, 600, 1500, data.lowAltitude, kofxGui_Display_Int, 0);
	geosphereGuiPannel->addSlider(appPtr->geospherePanel_terrain_resolution, "Terrain resolution", 140, 13, 1, 10, data.terrainResolution, kofxGui_Display_Int, 0);
	geosphereGuiPannel->mObjWidth = width*0.5;
	geosphereGuiPannel->mObjHeight = height*0.5;
	geosphereGuiPannel->mObjects[1]->mObjX = 165;
	geosphereGuiPannel->mObjects[1]->mObjY = 32;
    
	geosphereGuiPannel->mObjects[2]->mObjY = 66;
	geosphereGuiPannel->adjustToNewContent(100, 0);
	
	// HydrosphereGUIPanel
	ofxGuiPanel * hydrosphereGuiPannel = gui->addPanel(appPtr->hydrospherePanel, "Hydrosphere", data.hydrosphereLoc.x, data.hydrosphereLoc.y, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	hydrosphereGuiPannel->addSlider(appPtr->hydrospherePanel_water_level, "Water level", 140, 13, 0.0f, 1.0f, data.waterLevel, kofxGui_Display_Float2, 0);
	hydrosphereGuiPannel->addSlider(appPtr->hydrospherePanel_precipitation_amount, "Amount of rain", 140, 13, 0.0f, 2.0f, data.precipitationAmount, kofxGui_Display_Float2, 0);
	hydrosphereGuiPannel->addSlider(appPtr->hydrospherePanel_precipitation_humidity, "Min humidity need", 140, 13, 0.0f, 1.0f, data.precipitationHumidity, kofxGui_Display_Float2, 0);
	hydrosphereGuiPannel->addSlider(appPtr->hydrospherePanel_precipitation_cold, "Min cold front need", 140, 13, 0.0f, 1.0f, data.precipitationCold, kofxGui_Display_Float2, 0);
	hydrosphereGuiPannel->addSlider(appPtr->hydrospherePanel_precipitation_inclination, "Min Inclination", 140, 13, 0.0f, 1.0f, data.precipitationInclination, kofxGui_Display_Float2, 0);
	hydrosphereGuiPannel->addSlider(appPtr->hydrospherePanel_precipitation_altitud, "Min Altitud", 140, 13, 0.0f, 1.0f, data.precipitationAltitud, kofxGui_Display_Float2, 0);
	hydrosphereGuiPannel->addSlider(appPtr->hydrospherePanel_absortion_soil, "Soil Absortion", 140, 13, 0.0f, 1.0f, data.absortionSoil, kofxGui_Display_Float2, 0);
	hydrosphereGuiPannel->addSlider(appPtr->hydrospherePanel_depresion_flow, "Flow on Depresions", 140, 13, 0.0f, 1.0f, data.depresionFlow, kofxGui_Display_Float2, 0);
	
	hydrosphereGuiPannel->mObjWidth = width*0.5;
	hydrosphereGuiPannel->mObjHeight = height*0.5;
	hydrosphereGuiPannel->mObjects[1]->mObjX = 165;
	hydrosphereGuiPannel->mObjects[1]->mObjY = 32;
    
	hydrosphereGuiPannel->mObjects[2]->mObjY = 66;
	hydrosphereGuiPannel->mObjects[3]->mObjX = 165;
	hydrosphereGuiPannel->mObjects[3]->mObjY = 66;
    
	hydrosphereGuiPannel->mObjects[4]->mObjY = 100;
	hydrosphereGuiPannel->mObjects[5]->mObjX = 165;
	hydrosphereGuiPannel->mObjects[5]->mObjY = 100;
    
	hydrosphereGuiPannel->mObjects[6]->mObjY = 134;
	hydrosphereGuiPannel->mObjects[7]->mObjX = 165;
	hydrosphereGuiPannel->mObjects[7]->mObjY = 134;
	hydrosphereGuiPannel->adjustToNewContent(100, 0);
	
	// GeosphereGUIPanel
	ofxGuiPanel * biosphereGuiPannel = gui->addPanel(appPtr->biospherePanel, "Biosphere", data.biosphereLoc.x, data.biosphereLoc.y, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
    // Reaction Diffusion (Vegetation)
	biosphereGuiPannel->addSlider(appPtr->biospherePanel_diffU, "Diff U", 140, 13, 0.0, 0.5, data.biosphereDiffU, kofxGui_Display_Float4, 0);
	biosphereGuiPannel->addSlider(appPtr->biospherePanel_diffV, "Diff V", 140, 13, 0.0, 0.5, data.biosphereDiffV, kofxGui_Display_Float4, 0);
	biosphereGuiPannel->addSlider(appPtr->biospherePanel_f, "F", 140, 13, 0.0, 0.5, data.biosphereF, kofxGui_Display_Float4, 0);
	biosphereGuiPannel->addSlider(appPtr->biospherePanel_k, "K", 140, 13, 0.0, 0.5, data.biosphereK, kofxGui_Display_Float4, 0);
    // Flocking (Animals)
    biosphereGuiPannel->addSlider(appPtr->biospherePanel_MinDist, "Min Dist", 140, 13, 0.0, 0.1, data.biosphereMinDist, kofxGui_Display_Float4, 0);
    biosphereGuiPannel->addSlider(appPtr->biospherePanel_MaxDist, "Max Dist", 140, 13, 0.0, 0.1, data.biosphereMaxDist, kofxGui_Display_Float4, 0);
    biosphereGuiPannel->addSlider(appPtr->biospherePanel_MaxSpeed, "Max Speed", 140, 13, 0.0, 6.0, data.biosphereMaxSpeed, kofxGui_Display_Float2, 0);
    biosphereGuiPannel->addSlider(appPtr->biospherePanel_MaxForce, "Max Force", 140, 13, 0.0, 0.1, data.biosphereMaxForce, kofxGui_Display_Float4, 0);
    biosphereGuiPannel->addSlider(appPtr->biospherePanel_Separation, "Separation", 140, 13, 0.0, 3.0, data.biosphereSeparation, kofxGui_Display_Float2, 0);
    biosphereGuiPannel->addSlider(appPtr->biospherePanel_Alineation, "Alineation", 140, 13, 0.0, 3.0, data.biosphereAlineation, kofxGui_Display_Float2, 0);
    biosphereGuiPannel->addSlider(appPtr->biospherePanel_Cohesion, "Cohesion", 140, 13, 0.0, 3.0, data.biosphereCohesion, kofxGui_Display_Float2, 0);
    biosphereGuiPannel->addSlider(appPtr->biospherePanel_Borders, "Border", 140, 13, 0.0, 1.0, data.biosphereBorders, kofxGui_Display_Float2, 0);
    
	biosphereGuiPannel->mObjWidth = width*0.5;
	biosphereGuiPannel->mObjHeight = height*0.5;
	biosphereGuiPannel->mObjects[1]->mObjX = 165;
	biosphereGuiPannel->mObjects[1]->mObjY = 32;
    
	biosphereGuiPannel->mObjects[2]->mObjY = 66;
	biosphereGuiPannel->mObjects[3]->mObjX = 165;
	biosphereGuiPannel->mObjects[3]->mObjY = 66;
    
	biosphereGuiPannel->mObjects[4]->mObjY = 100;
	biosphereGuiPannel->mObjects[5]->mObjX = 165;
	biosphereGuiPannel->mObjects[5]->mObjY = 100;
    
	biosphereGuiPannel->mObjects[6]->mObjY = 134;
	biosphereGuiPannel->mObjects[7]->mObjX = 165;
	biosphereGuiPannel->mObjects[7]->mObjY = 134;
    
    biosphereGuiPannel->mObjects[8]->mObjY = 168;
	biosphereGuiPannel->mObjects[9]->mObjX = 165;
	biosphereGuiPannel->mObjects[9]->mObjY = 168;
    
    biosphereGuiPannel->mObjects[10]->mObjY = 202;
    biosphereGuiPannel->mObjects[11]->mObjX = 165;
	biosphereGuiPannel->mObjects[11]->mObjY = 202;
    
	biosphereGuiPannel->adjustToNewContent(100, 0);

	//do update while inactive
	gui->forceUpdate(false);
	gui->activate(true);

	/****************************
	* Set GUI values on startup
	****************************/
	// GeosphereGUIPanel 
	gui->update(appPtr->atmospherePanel_circular_force, kofxGui_Set_Bool, &appPtr->data.atmosphereCircularForce, sizeof(float));
	gui->update(appPtr->atmospherePanel_circular_angle, kofxGui_Set_Bool, &appPtr->data.atmosphereCircularAngle, sizeof(float));
	gui->update(appPtr->atmospherePanel_temperature_dissipation, kofxGui_Set_Bool, &appPtr->data.atmosphereTempDiss, sizeof(float));
    gui->update(appPtr->atmospherePanel_velocity_dissipation, kofxGui_Set_Bool, &appPtr->data.atmosphereVelDiss, sizeof(float));
    gui->update(appPtr->atmospherePanel_density_dissipation, kofxGui_Set_Bool, &appPtr->data.atmosphereDenDiss, sizeof(float));
    
	// GeosphereGUIPanel 
	gui->update(appPtr->geospherePanel_top_altitude, kofxGui_Set_Bool, &appPtr->data.topAltitude, sizeof(float));
	gui->update(appPtr->geospherePanel_low_altitude, kofxGui_Set_Bool, &appPtr->data.lowAltitude, sizeof(float));
	gui->update(appPtr->geospherePanel_terrain_resolution, kofxGui_Set_Bool, &appPtr->data.terrainResolution, sizeof(float));
	
	// HydrosphereGUIPanel 
	gui->update(appPtr->hydrospherePanel_water_level, kofxGui_Set_Bool, &appPtr->data.waterLevel, sizeof(float));
	gui->update(appPtr->hydrospherePanel_absortion_soil, kofxGui_Set_Bool, &appPtr->data.absortionSoil, sizeof(float));
	gui->update(appPtr->hydrospherePanel_depresion_flow, kofxGui_Set_Bool, &appPtr->data.depresionFlow, sizeof(float));
	gui->update(appPtr->hydrospherePanel_precipitation_inclination, kofxGui_Set_Bool, &appPtr->data.precipitationInclination, sizeof(float));
	gui->update(appPtr->hydrospherePanel_precipitation_altitud, kofxGui_Set_Bool, &appPtr->data.precipitationAltitud, sizeof(float));
	gui->update(appPtr->hydrospherePanel_precipitation_cold, kofxGui_Set_Bool, &appPtr->data.precipitationCold, sizeof(float));
	gui->update(appPtr->hydrospherePanel_precipitation_humidity, kofxGui_Set_Bool, &appPtr->data.precipitationHumidity, sizeof(float));
	gui->update(appPtr->hydrospherePanel_precipitation_amount, kofxGui_Set_Bool, &appPtr->data.precipitationAmount, sizeof(float));
	
	// BiosphereGUIPanel 
    // Reaction Diffusion ( Vegetation )
	gui->update(appPtr->biospherePanel_diffU, kofxGui_Set_Bool, &appPtr->data.biosphereDiffU, sizeof(float));
	gui->update(appPtr->biospherePanel_diffV, kofxGui_Set_Bool, &appPtr->data.biosphereDiffV, sizeof(float));
	gui->update(appPtr->biospherePanel_f, kofxGui_Set_Bool, &appPtr->data.biosphereF, sizeof(float));
	gui->update(appPtr->biospherePanel_k, kofxGui_Set_Bool, &appPtr->data.biosphereK, sizeof(float));
    // Flocking ( Animals ) 
    gui->update(appPtr->biospherePanel_MinDist, kofxGui_Set_Bool, &appPtr->data.biosphereMinDist, sizeof(float));
    gui->update(appPtr->biospherePanel_MaxDist, kofxGui_Set_Bool, &appPtr->data.biosphereMaxDist, sizeof(float));
    gui->update(appPtr->biospherePanel_MaxSpeed, kofxGui_Set_Bool, &appPtr->data.biosphereMaxSpeed, sizeof(float));
    gui->update(appPtr->biospherePanel_MaxForce, kofxGui_Set_Bool, &appPtr->data.biosphereMaxForce, sizeof(float));
    gui->update(appPtr->biospherePanel_Separation, kofxGui_Set_Bool, &appPtr->data.biosphereSeparation, sizeof(float));
    gui->update(appPtr->biospherePanel_Alineation, kofxGui_Set_Bool, &appPtr->data.biosphereAlineation, sizeof(float));
    gui->update(appPtr->biospherePanel_Cohesion, kofxGui_Set_Bool, &appPtr->data.biosphereCohesion, sizeof(float));
    gui->update(appPtr->biospherePanel_Borders, kofxGui_Set_Bool, &appPtr->data.biosphereBorders, sizeof(float));
}

void ofxEden::handleGui(int parameterId, int task, void* dataPased, int length){
	switch(parameterId){
		// Atmosphere
		case atmospherePanel_circular_force:
			if(length == sizeof(float))
				data.atmosphereCircularForce = *(float*)dataPased;
			break;
		case atmospherePanel_circular_angle:
			if(length == sizeof(float))
				data.atmosphereCircularAngle = *(float*)dataPased;
			break;
        case atmospherePanel_temperature_dissipation:
			if(length == sizeof(float))
				data.atmosphereTempDiss = *(float*)dataPased;
                atmosphere.getFluid().setTemperatureDissipation( *(float*)dataPased );
			break;
        case atmospherePanel_density_dissipation:
			if(length == sizeof(float))
				data.atmosphereDenDiss = *(float*)dataPased;
                atmosphere.getFluid().setDensityDissipation( *(float*)dataPased );
			break;
        case atmospherePanel_velocity_dissipation:
			if(length == sizeof(float))
				data.atmosphereVelDiss = *(float*)dataPased;
                atmosphere.getFluid().setVelocityDissipation( *(float*)dataPased );
			break;
		// Geosphere
		case geospherePanel_top_altitude:
			if(length == sizeof(float))
				data.topAltitude = *(float*)dataPased;
			break;
		case geospherePanel_low_altitude:
			if(length == sizeof(float))
				data.lowAltitude = *(float*)dataPased;
			break;
		case geospherePanel_terrain_resolution:
			if(length == sizeof(float))
				data.terrainResolution = *(float*)dataPased;
			break;
		// Hydrosphere
		case hydrospherePanel_water_level:
			if(length == sizeof(float))
				data.waterLevel = *(float*)dataPased;
			break;
		case hydrospherePanel_absortion_soil:
			if(length == sizeof(float))
				data.absortionSoil = *(float*)dataPased;
			break;
		case hydrospherePanel_depresion_flow:
			if(length == sizeof(float))
				data.depresionFlow = *(float*)dataPased;
			break;
		case hydrospherePanel_precipitation_inclination:
			if(length == sizeof(float))
				data.precipitationInclination = *(float*)dataPased;
			break;
		case hydrospherePanel_precipitation_altitud:
			if(length == sizeof(float))
				data.precipitationAltitud = *(float*)dataPased;
			break;
		case hydrospherePanel_precipitation_cold:
			if(length == sizeof(float))
				data.precipitationCold = *(float*)dataPased;
			break;
		case hydrospherePanel_precipitation_humidity:
			if(length == sizeof(float))
				data.precipitationHumidity = *(float*)dataPased;
			break;
		case hydrospherePanel_precipitation_amount:
			if(length == sizeof(float))
				data.precipitationAmount = *(float*)dataPased;
			break;
		// Biosphere
		case biospherePanel_diffU:
			if(length == sizeof(float))
				data.biosphereDiffU = *(float*)dataPased;
			break;
		case biospherePanel_diffV:
			if(length == sizeof(float))
				data.biosphereDiffV = *(float*)dataPased;
			break;
		case biospherePanel_f:
			if(length == sizeof(float))
				data.biosphereF = *(float*)dataPased;
			break;
		case biospherePanel_k:
			if(length == sizeof(float))
				data.biosphereK = *(float*)dataPased;
			break;
            
        case biospherePanel_MinDist:
			if(length == sizeof(float))
				data.biosphereMinDist = *(float*)dataPased;
			break;
        case biospherePanel_MaxDist:
			if(length == sizeof(float))
				data.biosphereMaxDist = *(float*)dataPased;
			break;
        case biospherePanel_MaxSpeed:
			if(length == sizeof(float))
				data.biosphereMaxSpeed = *(float*)dataPased;
			break;
        case biospherePanel_MaxForce:
			if(length == sizeof(float))
				data.biosphereMaxForce = *(float*)dataPased;
			break;
        case biospherePanel_Separation:
			if(length == sizeof(float))
				data.biosphereSeparation = *(float*)dataPased;
			break;
        case biospherePanel_Alineation:
			if(length == sizeof(float))
				data.biosphereAlineation = *(float*)dataPased;
			break;
        case biospherePanel_Cohesion:
			if(length == sizeof(float))
				data.biosphereCohesion = *(float*)dataPased;
			break;
        case biospherePanel_Borders:
			if(length == sizeof(float))
				data.biosphereBorders= *(float*)dataPased;
			break;
	}
	
	data.saveXml();
}
#endif //GUI_CONTROLS_H

