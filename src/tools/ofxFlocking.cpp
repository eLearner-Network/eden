//
//  ofxFlocking.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/27/11.
//  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#include "ofxFlocking.h"

ofxFlocking::ofxFlocking(){
    frame = 0;
    timeStep = 0.005f;
    
    particleSize = 30.0f;
    
    maxDist = 0.030;
    minDist = 0.025;
    maxSpeed = 3.0f;
    maxForce = 0.05f;
    
    separation = 1.5f;
    alineation = 1.0f;
    cohesion = 1.0f;
    food = 1.5f;
    flat = 1.5f;
    borders = 0.1f;
    
}

ofxFlocking& ofxFlocking::allocate(int _width, int _height, int _nBoids){
    width = _width;
    height = _height;
    
    resolution = (int) sqrt(_nBoids);
    nBoids = resolution * resolution;
    
    cout << "- flocking system, resolution " << resolution << " boids n: " << nBoids << endl;
    
    string fragmentVelUpdateShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect posData;\
    uniform sampler2DRect prevVelData;\
    \
    uniform sampler2DRect normalTex;\
    uniform sampler2DRect obstacleTex;\
    uniform sampler2DRect foodTex;\
    \
    uniform vec2  screen;\
    uniform int   resolution;\
    \
    uniform float timestep;\
    uniform float minDist;\
    uniform float maxDist;\
    uniform float maxSpeed;\
    uniform float maxForce;\
    \
    uniform float separation;\
    uniform float alineation;\
    uniform float cohesion;\
    \
    uniform float borders;\
    \
    vec2 offset[9];\
    \
    vec2 limit(vec2 vector, float max){\
        float lengthSquared = dot(vector,vector);\
        if( lengthSquared > max*max && lengthSquared > 0.0 ) {\
            float ratio = max/sqrt(lengthSquared);\
            vector.x *= ratio;\
            vector.y *= ratio;\
        }\
        return vector;\
    }\
    \
    vec2 avoid(vec2 pos, vec2 target, bool weight){\
        vec2 steer = target - pos;\
        if(weight) steer *= 1.0/sqrt(distance(pos,target));\
        return steer;\
    }\
    \
    void main(void){\
        vec2 st = gl_TexCoord[0].st;\
        \
        float life = texture2DRect( prevVelData, st ).z;\
        \
        vec2 pos = texture2DRect( posData, st).xy;\
        vec2 vel = texture2DRect( prevVelData, st ).xy;\
        vec2 acc = vec2(0.0,0.0);\
        \
        vec2  texSt = pos * screen;\
        \
        float foodPresent = texture2DRect(foodTex, texSt).g;\
        \
        if ( foodPresent > 0.0 )\
            life += foodPresent * 0.05;\
        else \
            life -= 0.1;\
        \
        if (life > 0.0){\
            offset[0] = vec2(-1.0, -1.0);\
            offset[1] = vec2(0.0, -1.0);\
            offset[2] = vec2(1.0, -1.0);\
            \
            offset[3] = vec2(-1.0, 0.0);\
            offset[4] = vec2(0.0, 0.0);\
            offset[5] = vec2(1.0, 0.0);\
            \
            offset[6] = vec2(-1.0, 1.0);\
            offset[7] = vec2(0.0, 1.0);\
            offset[8] = vec2(1.0, 1.0);\
            \
            float obst = texture2DRect( obstacleTex , texSt).b;\
            if (obst > 0.0){\
                \
                float lessDense = 2.0;\
                int lessDenseOffset = 4;\
                \
                float moreFood = 0.0;\
                int moreFoodOffset = 4;\
                \
                for (int i = 0; i < 9; i++){\
                    if (i != 4){\
                        float nearby = texture2DRect(obstacleTex , texSt + offset[i] ).b;\
                        if ( nearby < lessDense){\
                            lessDense = nearby;\
                            lessDenseOffset = i;\
                        }\
                        \
                        float foodAmount = texture2DRect(foodTex, texSt + offset[i] ).g;\
                        if ( foodAmount > moreFood){\
                            foodAmount = foodAmount;\
                            moreFoodOffset = i;\
                        }\
                    }\
                }\
                \
                acc += (offset[lessDenseOffset] + offset[moreFoodOffset]) * 0.5;\
                \
            } else {\
                vec2 sep = vec2(0.0,0.0);\
                vec2 ali = vec2(0.0,0.0);\
                vec2 coh = vec2(0.0,0.0);\
                \
                float near = 0.0;\
                float toNear = 0.0;\
                \
                for (int x = 0; x < resolution; x++){\
                    for ( int y = 0; y < resolution; y++){\
                        \
                        if(st != vec2(x,y)){\
                            float tLife = texture2DRect( prevVelData, vec2(x,y) ).z;\
                            \
                            if (tLife > 0.0){\
                                vec2 tPos = texture2DRect( posData, vec2(x,y) ).xy;\
                                vec2 tVel = texture2DRect( prevVelData, vec2(x,y) ).xy;\
                                \
                                vec2 diff = pos - tPos;\
                                float d = length(diff);\
                                \
                                if ( d < maxDist){\
                                    ali += tVel;\
                                    coh += tPos;\
                                    near++;\
                                    \
                                    if ( d < minDist){\
                                        sep += (normalize(diff) / d);\
                                        toNear++;\
                                    }\
                                }\
                            }\
                        }\
                    }\
                }\
                \
                if (toNear > 0.0){\
                    sep /= toNear;\
                    sep = normalize(sep);\
                    sep *= maxSpeed;\
                    sep -= vel;\
                    sep = limit(sep,maxForce);\
                }\
                \
                if (near > 0.0){\
                    ali /= near;\
                    ali = normalize(ali);\
                    ali *= maxSpeed;\
                    ali -= vel;\
                    ali = limit(ali,maxForce);\
                    \
                    coh /= near;\
                    coh -= pos;\
                    coh = normalize(coh);\
                    coh *= maxSpeed;\
                    coh -= vel;\
                    coh = limit(coh,maxForce);\
                }\
                \
                acc += sep * separation;\
                acc += ali * alineation;\
                acc += coh * cohesion;\
                \
                acc += avoid(pos,vec2(pos.x,1.0),true) * borders;\
                acc += avoid(pos,vec2(pos.x,0.0),true) * borders;\
                acc += avoid(pos,vec2(1.0,pos.y),true) * borders;\
                acc += avoid(pos,vec2(0.0,pos.y),true) * borders;\
            }\
            \
            vel += acc;\
            vel = limit(vel,maxSpeed);\
            \
            vec2 nextPos = pos;\
            nextPos += vel * timestep;\
            \
            if ( nextPos.x < 0.0)\
                vel.x = 0.5 * abs(vel.x);\
            \
            if ( nextPos.x > 1.0)\
                vel.x = -0.5 * abs(vel.x);\
            \
            if (nextPos.y < 0.0)\
                vel.y = 0.5 * abs(vel.y);\
            \
            if ( nextPos.y > 1.0)\
                vel.y = -0.5 * abs(vel.y);\
        } \
        \
        life = clamp(life,0.0,1.0);\
        \
        gl_FragColor.rgb = vec3(vel.x,vel.y,life);\
        gl_FragColor.a = 1.0;\
    }";
    velUpdateShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentVelUpdateShader);
    velUpdateShader.linkProgram();
    //velUpdateShader.load("","shaders/flockingVelUpdate.frag");
    
    string fragmentPosUpdateShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect prevPosData;\
    uniform sampler2DRect velData;\
    \
    uniform float timestep;\
    \
    void main(void){\
        vec2 st = gl_TexCoord[0].st;\
        \
        vec2 pos = texture2DRect( prevPosData, st ).xy;\
        vec2 vel = texture2DRect( velData, st ).xy;\
        \
        float life = texture2DRect( velData, st ).z;\
        \
        if (life > 0.0)\
            pos += vel * timestep;\
        \
        gl_FragColor.rgba = vec4(pos.x,pos.y,life,1.0);\
    }";
    posUpdateShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentPosUpdateShader);
    posUpdateShader.linkProgram();
    //posUpdateShader.load("","shaders/flockingPosUpdate.frag");
    
    
    renderShader.setGeometryInputType(GL_POINTS);
	renderShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	renderShader.setGeometryOutputCount(6);
    
    string fragmentRenderShader = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect sparkTex;\
    void main() {\
        vec2 st = gl_TexCoord[0].st;\
        gl_FragColor = texture2DRect(sparkTex, st);\
    }";
    renderShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentRenderShader);
    
    string vertexRenderShader = "#version 120 \n \
    #extension GL_EXT_gpu_shader4 : enable \n \
    #extension GL_ARB_texture_rectangle : enable \n \
    \
    uniform sampler2DRect posTex;\
    uniform vec2 screen;\
    uniform int resolution;\
    \
    void main() {\
        vec4 pos = gl_Vertex;\
        \
        pos.x *= screen.x;\
        pos.y *= screen.y;\
        pos.z = clamp(pos.z,0.0,1.0);\
        \
        gl_Position = pos;\
        gl_FrontColor =  gl_Color;\
    }";
    renderShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexRenderShader);
    
    string geometryRenderShader = "#version 120 \n \
    #extension GL_EXT_geometry_shader4 : enable \n \
    #extension GL_EXT_gpu_shader4 : enable \n \
    \
    uniform float size;\
    uniform float imgWidth, imgHeight; \
    \
    void main(void){\
        for(int i = 0; i < gl_VerticesIn; i++){\
            float life = size * gl_PositionIn[i].z;\
            \
            gl_Position = gl_ModelViewProjectionMatrix * ( gl_PositionIn[i] + vec4(-life,-life,0.0,0.0));\
            gl_TexCoord[0].x = 0.0;  \
            gl_TexCoord[0].y = 0.0;  \
            EmitVertex();\
            \
            gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(life,-life,0.0,0.0));\
            gl_TexCoord[0].x = imgWidth;\
            gl_TexCoord[0].y = 0.0;\
            EmitVertex();\
            \
            gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(life,life,0.0,0.0));\
            gl_TexCoord[0].x = imgWidth;  \
            gl_TexCoord[0].y = imgHeight;  \
            EmitVertex();\
            EndPrimitive();\
            \
            gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-life,-life,0.0,0.0));\
            gl_TexCoord[0].x = 0.0;  \
            gl_TexCoord[0].y = 0.0;  \
            EmitVertex();\
            \
            gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(-life,life,0.0,0.0));\
            gl_TexCoord[0].x = 0.0;  \
            gl_TexCoord[0].y = imgHeight;  \
            EmitVertex();\
            \
            gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + vec4(life,life,0.0,0.0));\
            gl_TexCoord[0].x = imgWidth;  \
            gl_TexCoord[0].y = imgHeight;  \
            EmitVertex();\
            EndPrimitive();\
        }\
    }";
    renderShader.setupShaderFromSource(GL_GEOMETRY_SHADER, geometryRenderShader);
    renderShader.linkProgram();
    //renderShader.load("shaders/flockingRender.vert","shaders/flockingRender.frag","shaders/flockingRender.geom");
    
    sparkImage.loadImage("textures/life/spark.png");
    imageWidth = sparkImage.getWidth();
    imageHeight = sparkImage.getHeight();

    // 1. Process birth and death
    float * pos = new float[nBoids*3];    
    float offset = 1.0/resolution;
    for (int x = 0; x < resolution; x++){
        for (int y = 0; y < resolution; y++){
            int i = resolution * y + x;
            
            pos[i*3 + 0] = ofRandom(1.0); //x*offset;
            pos[i*3 + 1] = ofRandom(1.0); //y*offset;
            pos[i*3 + 2] = 1.0;
            //pos[i*4 + 3] = 1.0;
        }
    }
    
    ofTexture posTex;
    posTex.allocate(resolution, resolution, GL_RGB16F);
    posTex.loadData(pos, resolution, resolution, GL_RGB);
    
    delete(pos);
    
    float * vel = new float[nBoids*3];
    for (int i = 0; i < nBoids; i++){
        vel[i*3 + 0] = ofRandom(-1,1);
        vel[i*3 + 1] = ofRandom(-1,1);
        vel[i*3 + 2] = 1.0;
        //vel[i*4 + 3] = 1.0;
    }
    
    ofTexture velTex;
    velTex.allocate(resolution, resolution, GL_RGB16F);
    velTex.loadData(vel, resolution, resolution, GL_RGB);

    delete(vel);
    
    pixels.allocate(resolution,resolution, 4);
    particles = new ofPoint[nBoids];
    for (int i = 0; i < nBoids; i++){
        particles[i].set(0.0,0.0,0.0);
    }
    
    for(int i = 0; i < 2; i++){
        posFbo[i].allocate(resolution,resolution,GL_RGB16F);
        posFbo[i].begin();
        ofClear(0);
        posTex.draw(0,0);
        posFbo[i].end();
        
        velFbo[i].allocate(resolution,resolution,GL_RGB16F);
        velFbo[i].begin();
        ofClear(0);
        velTex.draw(0,0);
        velFbo[i].end();
    }
    
    renderFbo.allocate(width,height,GL_RGBA);
    renderFbo.begin();
    ofClear(0);
    renderFbo.end();
    
    return * this;
}

void ofxFlocking::update(ofTexture &normalTex, ofTexture &obstaclesTex, ofTexture &foodTex){
    int actual = (frame)%2; // Actual buffer
    int prev = (frame+1)%2; // Prev/Next buffer
    
    // 2. Update Velocities
    velFbo[actual].begin();
    ofClear(0);
    velUpdateShader.begin();
    velUpdateShader.setUniformTexture("posData", posFbo[prev].getTextureReference(), 0);
    velUpdateShader.setUniformTexture("prevVelData", velFbo[prev].getTextureReference(), 1);
    
    velUpdateShader.setUniformTexture("normalTex", normalTex, 2);
    velUpdateShader.setUniformTexture("obstacleTex", obstaclesTex, 3);
    velUpdateShader.setUniformTexture("foodTex", foodTex, 4);
    
    velUpdateShader.setUniform1i("resolution", (int)resolution); 
    velUpdateShader.setUniform2f("screen", (float)width, (float)height);
    
    velUpdateShader.setUniform1f("timestep", data->biosphereTimeStep);
    velUpdateShader.setUniform1f("minDist", data->biosphereMinDist);
    velUpdateShader.setUniform1f("maxDist", data->biosphereMaxDist);
    velUpdateShader.setUniform1f("maxSpeed", data->biosphereMaxSpeed);
    velUpdateShader.setUniform1f("maxForce", data->biosphereMaxForce);
    
    velUpdateShader.setUniform1f("separation", data->biosphereSeparation);
    velUpdateShader.setUniform1f("alineation", data->biosphereAlineation);
    velUpdateShader.setUniform1f("cohesion", data->biosphereCohesion);
    velUpdateShader.setUniform1f("borders", data->biosphereBorders);
    //velUpdateShader.setUniform1f("flat", data->biosphereFlat);
    //velUpdateShader.setUniform1f("food", data->biosphereFood);
    
    renderFrame(resolution,resolution);
    velUpdateShader.end();
    velFbo[actual].end();

    // 3. Update Positions
    posFbo[actual].begin();
    ofClear(0);
    posUpdateShader.begin();
    posUpdateShader.setUniformTexture("prevPosData", posFbo[prev].getTextureReference(), 0);
    posUpdateShader.setUniformTexture("velData", velFbo[actual].getTextureReference(), 1);
    posUpdateShader.setUniform1f("timestep",(float) timeStep );
    renderFrame(resolution,resolution);
    posUpdateShader.end();
    posFbo[actual].end();
    

    posFbo[actual].readToPixels(pixels);
    for(int x = 0; x < resolution; x++){
        for(int y = 0; y < resolution; y++){
            ofFloatColor p = pixels.getColor(x, y);
            int i = y * resolution + x;
            particles[i].set(p.r,p.g,p.b);
        }
    }
        
    // 4. sort for alpha blending (optional)
    
    
    // 5. Transfer texture data to vertex data
    renderFbo.begin();
    ofClear(0);
    renderShader.begin();
    renderShader.setUniformTexture("posTex", posFbo[actual].getTextureReference(), 0);
    renderShader.setUniformTexture("sparkTex", sparkImage.getTextureReference() , 1);
    renderShader.setUniform1i("resolution", (int)resolution); 
    renderShader.setUniform2f("screen", (float)width, (float)height);
    renderShader.setUniform1f("size", (float)particleSize); 
    renderShader.setUniform1f("imgWidth", (float)sparkImage.getWidth());
    renderShader.setUniform1f("imgHeight", (float)sparkImage.getHeight());
    
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    ofSetColor(255);
    glBegin( GL_POINTS );
    for(int i = 0; i < nBoids; i++){
        glVertex3d(particles[i].x, particles[i].y, particles[i].z);
    }
    ofDisableBlendMode();
    
    glEnd();
    renderShader.end();
    renderFbo.end();

    ofEnableBlendMode( OF_BLENDMODE_ALPHA );
    
    // Switch buffers
    frame++;
}

void ofxFlocking::renderFrame(int _width, int _height){
    // Rendering canvas frame used to apply the texture
    ofSetColor(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(_width, 0); glVertex3f(_width, 0, 0);
	glTexCoord2f(_width, _height); glVertex3f(_width, _height, 0);
	glTexCoord2f(0,_height); glVertex3f(0,_height, 0);
	glEnd();
}

void ofxFlocking::draw(){
    renderFbo.draw(0,0);
}