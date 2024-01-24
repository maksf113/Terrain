#pragma once
#include "Terrain.h"

class MidpointDisplacementTerrain : public BaseTerrain
{
public:
	MidpointDisplacementTerrain() {}
	void CreateMidpointDisplacement(int size, float roughness, float minHeight, float maxHeight);
private:
	void CreateMidpointDisplacementF32(float roughness);
	void DiamondStep(int rectSize, float crntHeight);
	void SquareStep(int rectSize, float crntHeight);
	float RandomFloatRange(float min, float max);
	int CalcNextPowerOfTwo(int i);
};
