#include "MidpointDisplacementTerrain.h"

void MidpointDisplacementTerrain::CreateMidpointDisplacement(int terrainSize, float roughness,
	float minHeight, float maxHeight)
{
	if (roughness < 0.0f)
	{
		std::cout << "Roughnes must be positive: " << roughness << std::endl;
		exit(0);
	}
	m_terrainSize = terrainSize;
	m_minHeight = minHeight;
	m_maxHeight = maxHeight;

	m_heightMap.InitArray2D(terrainSize, terrainSize, 0.0f); // 0.0 = initial height value

	CreateMidpointDisplacementF32(roughness);

	m_heightMap.Normalize(minHeight, maxHeight);

	this->CreateMesh(m_terrainSize);
}

void MidpointDisplacementTerrain::CreateMidpointDisplacementF32(float roughness)
{
	int rectSize = CalcNextPowerOfTwo(m_terrainSize);
	float crntHeight = (float)rectSize / 2.0f;
	float heightReduce = pow(2.0f, -roughness);
	while (rectSize > 0)
	{
		DiamondStep(rectSize, crntHeight);
		SquareStep(rectSize, crntHeight);
		rectSize /= 2;
		crntHeight *= heightReduce;
	}
}

void MidpointDisplacementTerrain::DiamondStep(int rectSize, float crntHeight)
{
	int halfRectSize = rectSize / 2;
	for(int z = 0; z < m_terrainSize; z += rectSize)
		for (int x = 0; x < m_terrainSize; x += rectSize)
		{
			int next_x = (x + rectSize) % m_terrainSize;
			int next_z = (z + rectSize) % m_terrainSize;
			if (next_x < x)
				//next_x = m_terrainSize - 1;
				next_x = x;
			if (next_z < z)
				//next_z = m_terrainSize - 1;
				next_z = z;
			float topLeft = m_heightMap.Get(x, z);
			float topRight = m_heightMap.Get(next_x, z);
			float bottomLeft = m_heightMap.Get(x, next_z);
			float bottomRight = m_heightMap.Get(next_x, next_z);

			int mid_x = (x + halfRectSize) % m_terrainSize;
			if (mid_x < x)
				mid_x = x;
			int mid_z = (z + halfRectSize) % m_terrainSize;
			if (mid_z < z)
				mid_z = z;

			float randValue = RandomFloatRange(-crntHeight, crntHeight);
			float midValue = (topLeft + topRight + bottomLeft + bottomRight) / 4.0f;

			m_heightMap.Set(mid_x, mid_z, midValue + randValue);
		}
}

void MidpointDisplacementTerrain::SquareStep(int rectSize, float crntHeight)
{
	// only left and upper edge sufficient (wrapping)
	int halfRectSize = rectSize / 2;
	for(int z = 0; z < m_terrainSize; z += rectSize)
		for (int x = 0; x < m_terrainSize; x += rectSize)
		{
			int next_x = (x + rectSize) % m_terrainSize;
			int next_z = (z + rectSize) % m_terrainSize;

			if (next_x < x)
				next_x = x;
			if (next_z < z)
				next_z = z;

			int mid_x = (x + halfRectSize) % m_terrainSize;
			int mid_z = (z + halfRectSize) % m_terrainSize;

			if (mid_x < x)
				mid_x = x;
			if (mid_z < z)
				mid_z = z;

			int prev_mid_x = (x - halfRectSize + m_terrainSize) % m_terrainSize;
			int prev_mid_z = (z - halfRectSize + m_terrainSize) % m_terrainSize;

			if (prev_mid_x > x)
				prev_mid_x = mid_x;
			if (prev_mid_z > z)
				prev_mid_z = mid_z;

			float crntTopLeft = m_heightMap.Get(x, z);
			float crntTopRight = m_heightMap.Get(next_x, z);
			float crntCenter = m_heightMap.Get(mid_x, mid_z);
			float crntBottomLeft = m_heightMap.Get(x, next_z);
			float prevZCenter = m_heightMap.Get(mid_x, prev_mid_z);
			float prevXCenter = m_heightMap.Get(prev_mid_x, mid_z);

			float crntLeftMid = (crntTopLeft + crntBottomLeft + prevXCenter + crntCenter) / 4.0f 
				+ RandomFloatRange(-crntHeight, crntHeight);
			float crntTopMid = (crntTopLeft + crntTopRight + crntCenter + prevZCenter) / 4.0f
				+ RandomFloatRange(-crntHeight, crntHeight);

			m_heightMap.Set(mid_x, z, crntTopMid);
			m_heightMap.Set(x, mid_z, crntLeftMid);
		}
}

float MidpointDisplacementTerrain::RandomFloatRange(float min, float max)
{
	float val = (float)rand();
	val *= (max - min) / (float)RAND_MAX;
	val += min;
	return val;
}

int MidpointDisplacementTerrain::CalcNextPowerOfTwo(int i)
{
	float log = log2f((float)i);
	return (int)pow(2, (int)log + 1);
}