/*
 * Final demonstration using all modules. Use this on demonstration day.
 *
 * Author: Jason Bunk
 * Web page: http://sites.google.com/site/jasonbunk
 * License: Apache License Version 2.0, January 2004
 * Copyright (c) 2015 Jason Bunk
 */

#include "stdafx.h"
#include "SimulationOutputRenderer.h"
#include "TryIncludeJPhysics.h"
#include "main.h"
#include <stdio.h>
#include <iomanip>
using std::cout; using std::endl;
#include "Utils/SUtils.h"


double SimulationOutputRenderer::GetGridWidth_ForDrawingPlanes() const
{
	return 6.0;
}


void SimulationOutputRenderer::InitBeforeSimStart()
{
	gGameSystem.fixed_time_step = -1; //not fixed
	gGameSystem.fixed_timestep_randomizer__stddev = -1; //negative: don't randomizes
	
	glPointSize(9.0); //size of stars as points
	glLineWidth(2.0); //usually 2.0
	
	time_step_between_draws = 0.01;
}

void SimulationOutputRenderer::UpdateSystemStuff_EachPhysicsStep(double frametime)
{
	time_accumulated_since_last_draw += frametime;
	if(time_accumulated_since_last_draw > time_step_between_draws) {
		time_accumulated_since_last_draw = 0.0;
		drawNextStep = true;
	}
}

bool SimulationOutputRenderer::OpenDataFile(std::string filename)
{
	FILE* dataFile = fopen(filename.c_str(), "rb");
	if(dataFile != NULL) {
		cout<<"successfully opened data file \'"<<filename<<"\'"<<endl;
		dataFiles.push_back(dataFile);
		return true;
	} else {
		cout<<"error: unable to open data file \'"<<filename<<"\'"<<endl;
	}
	return false;
}


void SimulationOutputRenderer::RespondToKeyStates()
{
    //if(gGameSystem.GetKeyboard()->keyboard['k']) {}
}


static bool ReadOnePosition(FILE* fromHere, phys::vec3 & intoHere)
{
	const int FLTBYTES = 4;
	float temp;
	
	if(fread(&temp, FLTBYTES, 1, fromHere) < 1){return false;}
	intoHere.x = temp;
	
	if(fread(&temp, FLTBYTES, 1, fromHere) < 1){return false;}
	intoHere.y = temp;
	
	if(fread(&temp, FLTBYTES, 1, fromHere) < 1){return false;}
	intoHere.z = temp;
	
	return true;
}

SimulationOutputRenderer::~SimulationOutputRenderer() {
	if(dataFiles.empty() == false) {
		auto DFILE = dataFiles.begin();
		while(DFILE != dataFiles.end()) {
			fclose(*DFILE);
			DFILE = dataFiles.erase(DFILE);
		}
	}
}


void SimulationOutputRenderer::draw()
{
	if(AutoSaveRenderedImages) {
		drawNextStep = true;
	}
	
	glPointParameterf(GL_POINT_SIZE_MIN, 0.001);
	glPointParameterf(GL_POINT_SIZE_MAX, 10000.0);
	glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, 10000.0);
	GLfloat atten[3] = {0.0f, 0.0f, 1.0f};
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, atten);
	glPointSize(400.0); //size of stars as points
	
	
	if(drawNextStep && dataFiles.empty() == false) {
		bool needToClearLastDrawn = true;
		phys::vec3 oneStarPos;
		int particlesReadThisStep = 0;
		
		glBegin(GL_POINTS);
		
		for(; particlesReadThisStep < numParticlesPerStep; particlesReadThisStep++) {
			
			bool done_reading = false;
			auto DFILE = dataFiles.begin();
			int coloriter = -1;
			while(DFILE != dataFiles.end()) {
				coloriter++;
				if(ReadOnePosition(*DFILE, oneStarPos)) {
					
					if(needToClearLastDrawn) {
						lastDrawnStars.clear();
						needToClearLastDrawn = false;
					}
					lastDrawnStars.push_back(std::pair<int,phys::vec3>(coloriter,oneStarPos));
					
					switch(coloriter) {
					case 0: glColor4ub(255, 25, 25, 255); break;
					case 1: glColor4ub(25, 255,255, 255); break;
					case 2: glColor4ub(255, 25,255, 255); break;
					case 3: glColor4ub(255,255, 25, 255); break;
					case 4: glColor4ub(25, 255, 25, 255); break;
					case 5: glColor4ub(25,  25,225, 255); break;
					default:glColor4ub(255, 255,255, 255); break;
					}
					
					glVertex3d(oneStarPos.x, oneStarPos.y, oneStarPos.z);
					
					DFILE++;
				} else {
					cout<<"done reading file"<<endl;
					done_reading = true;
					fclose(*DFILE);
					DFILE = dataFiles.erase(DFILE);
				}
			}
			if(done_reading) {
				framesSoFar++;
				glEnd();
				return;
			}
		}
		
		glEnd();
		
		framesSoFar++;
		drawNextStep = false;
	} else if(lastDrawnStars.empty()==false) {
		glBegin(GL_POINTS);
		int coloriter = 0;
		int numStarsDrawnn = lastDrawnStars.size();
		for(int i=0; i<numStarsDrawnn; i++) {
					
					switch(lastDrawnStars[i].first) {
					case 0: glColor4ub(255, 25, 25, 255); break;
					case 1: glColor4ub(25, 255,255, 255); break;
					case 2: glColor4ub(255, 25,255, 255); break;
					case 3: glColor4ub(255,255, 25, 255); break;
					case 4: glColor4ub(25, 255, 25, 255); break;
					case 5: glColor4ub(25,  25,225, 255); break;
					default:glColor4ub(255, 255,255, 255); break;
					}
			/*if(i < (numStarsDrawnn/2)) {
				glColor4ub(255, 25, 25, 255);
			} else {
				glColor4ub(25, 25, 255, 255);
			}*/
			
			glVertex3d(lastDrawnStars[i].second.x, lastDrawnStars[i].second.y, lastDrawnStars[i].second.z);
		}
		glEnd();
	}
}


bool SimulationOutputRenderer::FormatTextInfo(char* text_buffer, int line_n)
{
if(text_buffer != nullptr)
{
	if(line_n == 0) {
		sprintf____s(text_buffer, "framesSoFar: %d", framesSoFar);
	}
	else if(line_n == 1) {
		sprintf____s(text_buffer, "time: %f", ((float)framesSoFar)*0.25f);
	}
	else if(line_n == 2) {
		int lastpart = 0;//lastDrawnStars.size()-1;
		sprintf____s(text_buffer, "object 0 was at: (%f, %f, %f)", lastDrawnStars.empty()?0.0f:((float)lastDrawnStars[lastpart].second.x),
																   lastDrawnStars.empty()?0.0f:((float)lastDrawnStars[lastpart].second.y),
																   lastDrawnStars.empty()?0.0f:((float)lastDrawnStars[lastpart].second.z));
	}
	else {return false;}
	return true;
}
	return false;
}









