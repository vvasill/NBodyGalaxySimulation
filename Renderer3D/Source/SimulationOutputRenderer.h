#ifndef ___SIMULATION_OUTPUT_RENDERER_H____
#define ___SIMULATION_OUTPUT_RENDERER_H____

#include "SimulationSystem.h"
#include <fstream>
#include <string>


class SimulationOutputRenderer : public SimulationSystem
{
	std::ifstream * dataFile;
	int framesSoFar;
	bool drawNextStep;
	std::vector<phys::vec3> lastDrawnStars;
	double time_accumulated_since_last_draw;
	double time_step_between_draws;
	
public:
	int numParticlesPerStep;
	
	
	SimulationOutputRenderer() : SimulationSystem(), dataFile(nullptr),
													drawNextStep(true),
													framesSoFar(0),
													numParticlesPerStep(1),
													time_accumulated_since_last_draw(0.0),
													time_step_between_draws(0.1) {}
	~SimulationOutputRenderer() {if(dataFile != nullptr){dataFile->close(); delete dataFile;}}
	
	void OpenDataFile(std::string filename);
	
	virtual int GetNumFramesDraw() {return framesSoFar;}
	virtual void UpdateSystemStuff_EachPhysicsStep(double frametime);
	virtual void draw();
	virtual double GetGridWidth_ForDrawingPlanes() const;
	virtual void RespondToKeyStates();
	virtual bool FormatTextInfo(char* text_buffer, int line_n);
	virtual void InitBeforeSimStart();
};




#endif
