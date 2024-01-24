#include "FaultFormationTerrain.h"

void FaultFormationTerrain::CreateFaultFormation(int terrainSize, int iterations, float minHeight, float maxHeight, float falloff,
	float filter, int smoothIter)
{
	m_terrainSize = terrainSize;
	m_minHeight = minHeight;
	m_maxHeight = maxHeight;
	m_falloff = falloff;
	m_heightMap.InitArray2D(terrainSize, terrainSize, 0.0f); // 0.0 = initial height value

	CreateFaultFormationInternal(iterations, minHeight, maxHeight, filter, smoothIter);

	m_heightMap.Normalize(minHeight, maxHeight);

	this->CreateMesh(m_terrainSize);

	//m_triangleList.CreateTriangleList(m_terrainSize, m_terrainSize, this);
}

void FaultFormationTerrain::CreateFaultFormationInternal(int iterations, float minHeight, float maxHeight, 
	float filter, int smoothIter)
{
	float heightRange = maxHeight - minHeight;
	for (int crntIter = 0; crntIter < iterations; crntIter++)
	{
		float height = minHeight + heightRange * m_falloff;

		TerrainPoint p1, p2;

		GenRandomTerrainPoints(p1, p2);

		int dirX = p2.x - p1.x;
		int dirZ = p2.z - p1.z;

		for(int z = 0; z < m_terrainSize; z++)
			for (int x = 0; x < m_terrainSize; x++)
			{
				int dirX_in = x - p1.x;
				int dirZ_in = z - p1.z;

				int crossProduct = dirX_in * dirZ - dirZ_in * dirX;
				if (crossProduct > 0)
				{
					float crntHeight = m_heightMap.Get(x, z);
					m_heightMap.Set(x, z, crntHeight + height);
				}
			}
	}
	for(int i = 0; i < smoothIter; i++)
		ApplySmoothFilter(filter);
}

void FaultFormationTerrain::GenRandomTerrainPoints(TerrainPoint& p1, TerrainPoint& p2)
{
	p1.x = rand() % m_terrainSize;
	p1.z = rand() % m_terrainSize;
	int counter = 0;
	do
	{
		p2.x = rand() % m_terrainSize;
		p2.z = rand() % m_terrainSize;

		if (counter++ > 1000)
		{
			std::cout << "Endless loop detexted in GenRandomTerrainPoints" << std::endl;
			assert(0);
		}
	} while (p1.IsEqual(p2));
}

void FaultFormationTerrain::ApplySmoothFilter(float filter)
{
	for (int z = 0; z < m_terrainSize; z++)
		for (int x = 0; x < m_terrainSize; x++)
			SmoothSinglePoint(x, z, filter);
}

void FaultFormationTerrain::SmoothSinglePoint(int x, int z, float filter)
{
	int xPlus = x + 1 < m_terrainSize ? x + 1 : x;
	int xMinus = x - 1 > -1 ? x - 1 : x;
	int zPlus = z + 1 < m_terrainSize ? z + 1 : z;
	int zMinus = z - 1 > -1 ? z - 1 : z;
	float newVal = m_heightMap.Get(x, z) * filter +
		m_heightMap.Get(xPlus, z) * (1.0f - filter) * 0.25 +
		m_heightMap.Get(xMinus, z) * (1.0f - filter) * 0.25 +
		m_heightMap.Get(x, zPlus) * (1.0f - filter) * 0.25 +
		m_heightMap.Get(x, zMinus) * (1.0f - filter) * 0.25;
	m_heightMap.Set(x, z, newVal);
}