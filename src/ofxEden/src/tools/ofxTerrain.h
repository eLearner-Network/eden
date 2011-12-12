/*
 *  ofxTerrain.h
 *  Eden Proyect
 *
 *  Created by Patricio González Vivo on 02/08/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 *  And NeHe Vertex Buffer Object tutorial 
 *	http://nehe.gamedev.net/tutorial/vertex_buffer_objects/22002/
 *
 */

#ifndef OFXTERRAIN
#define OFXTERRAIN

#include "ofMain.h"

class ofxTerrain {
public:
	ofxTerrain(){ };	
	~ofxTerrain(){ };

	bool loadHeightmap( ofTexture& heightmap, ofTexture& normalMap, ofTexture& texture, float flResolution = 10.0, float flHeightScale = 100.0 );
    bool loadHeightmap( ofFloatPixels& data, ofFloatPixels& norms, ofTexture& texture, float flResolution = 10.0, float flHeightScale = 100.0);
	
	void draw(){ vbo.draw(GL_TRIANGLES , 0, nVertexCount); };

private:
	ofVbo			vbo;
	
	// Mesh Data
	ofVec3f*		pNormals;
    ofVec3f*		pVertices;      // Vertex Data
    ofVec2f*		pTexCoords;     // Texture Coordinates

	int				nVertexCount;	// Vertex Count
};
	

#endif