#pragma once
#include "Terrain.h"

class FaultFormationTerrain : public BaseTerrain
{
private:
	float m_falloff;
	struct TerrainPoint
	{
		int x = 0;
		int z = 0;

		void Print()
		{
			std::cout << x << "\t" << z << std::endl;
		}
		bool IsEqual(TerrainPoint& p) {
			return ((x == p.x) && (z == p.z));
		}
	};

	void CreateFaultFormationInternal(int iterations, float minHeight, float maxHeight,
		float filter, int smoothIter);

	void GenRandomTerrainPoints(TerrainPoint& p1, TerrainPoint& p2);
	void ApplySmoothFilter(float filter);
	void SmoothSinglePoint(int x, int z, float filter);
public:
	FaultFormationTerrain() {}
	void CreateFaultFormation(int terrainSize, int iterations, float minHeight, float maxHeight, float falloff,
		float filter, int smoothIter);
};

	
