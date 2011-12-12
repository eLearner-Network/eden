/*
 *  ofxTerrain.cpp
 *  ofxEden
 *
 *  Created by Patricio González Vivo on 06/09/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */
#include "ofxTerrain.h"

bool ofxTerrain::loadHeightmap( ofTexture& heightmap, ofTexture& normalMap, ofTexture& texture, float flResolution, float flHeightScale){
	ofFloatPixels data;
	data.allocate(heightmap.getWidth(), heightmap.getHeight(), OF_PIXELS_RGBA);
	heightmap.readToPixels(data);
	
	ofFloatPixels norms;
	norms.allocate(normalMap.getWidth(), normalMap.getHeight(), OF_PIXELS_RGBA);
	normalMap.readToPixels(norms);
	
	return loadHeightmap( data, norms, texture, flResolution,flHeightScale);
};

bool ofxTerrain::loadHeightmap( ofFloatPixels& data, ofFloatPixels& norms, ofTexture& texture, float flResolution, float flHeightScale){
	int width = data.getWidth();
	int height = data.getHeight();
	
	texture.bind();
	
	// Generate Vertex Field
	nVertexCount = (int) ( width * height * 6 / ( flResolution * flResolution ) );
	
	pVertices	= new ofVec3f[nVertexCount];		// Allocate Vertex Data
	pTexCoords	= new ofVec2f[nVertexCount];		// Allocate Tex Coord Data
	pNormals	= new ofVec3f[nVertexCount];		// Allocate Normals
	
	int nX, nZ, nTri, nIndex=0;                     // Create Variables
	float flX, flZ;
	
	for( nZ = 0; nZ < height-flResolution ; nZ += (int) flResolution )
	{
		for( nX = 0; nX < width-flResolution ; nX += (int) flResolution )
		{
			// Normals by face 
			//ofVec3f p[4];
			
			for( nTri = 0; nTri < 6; nTri++ )
			{
				// Using This Quick Hack, Figure The X,Z Position Of The Point
				flX = (float) nX + ( ( nTri == 1 || nTri == 2 || nTri == 5 ) ? flResolution : 0.0f );
				flZ = (float) nZ + ( ( nTri == 2 || nTri == 4 || nTri == 5 ) ? flResolution : 0.0f );
				
				// Set The Data, Using PtHeight To Obtain The Y Value
				pVertices[nIndex].x = flX - ( width *0.5 );
				pVertices[nIndex].y = data.getColor((int)flX, (int)flZ).r * flHeightScale;
				pVertices[nIndex].z = flZ - ( height *0.5 );
				
				// 3	0 --- 1		nTri reference
				// | \	  \	  |
				// |   \	\ |
				// 4 --- 5	  2
				
				// Stretch The Texture Across The Entire Mesh
				pTexCoords[nIndex].x = flX*2.0;// / width;
				pTexCoords[nIndex].y = flZ*2.0;// / height;
				
				// Normals by vert
				pNormals[nIndex].x = norms.getColor((int)flX, (int)flZ).r;
				pNormals[nIndex].y = norms.getColor((int)flX, (int)flZ).b;
				pNormals[nIndex].z = norms.getColor((int)flX, (int)flZ).g;
				
				/*
				 // Normals by face 
				 if (nTri >= 0 && nTri < 3)		// 0 --- 1	p[ x ] Reference
				 p[nTri] = pVertices[nIndex];	// |	 |
				 else if (nTri == 4)			// |	 |
				 p[3] = pVertices[nIndex];		// 3 --- 2
				 */
				
				// Increment Our Index
				nIndex++;
			}
			
			/*
			 // Normals by face 
			 for (int i = 6; i > 0; i--){
			 if ( i > 3) // nIndex - (6 / 5 / 4)
			 pNormals[nIndex-i] = ((p[0] - p[1]).cross(p[2] - p[1])).normalize();
			 else		// nIndex - (3 / 2 / 1)
			 pNormals[nIndex-i] = ((p[2] - p[3]).cross(p[0] - p[3])).normalize();
			 }*/				
		}
	}
	
	vbo.setVertexData(pVertices, nVertexCount, GL_STREAM_DRAW);
	vbo.setNormalData(pNormals, nVertexCount, GL_STREAM_DRAW);
	vbo.setTexCoordData(pTexCoords, nVertexCount, GL_STREAM_DRAW);
	
	texture.unbind();
	
	// Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
	delete [] pVertices; pVertices = NULL;
	delete [] pTexCoords; pTexCoords = NULL;
	delete [] pNormals; pNormals = NULL;
	
	return true;
};