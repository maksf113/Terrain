#pragma once

#include "Array2D.h"
#include "Utilities.h"
#include "shaderClass.h"
#include "Camera.h"
#include  "Mesh.h"

class BaseTerrain : public Mesh
{
public:
	BaseTerrain() {}

	void InitTerrain(float worldScale, float textureScale,
		const std::vector<std::string>& textureFilenames);
	void Draw(Shader& shader, Camera& camera);

	void LoadFromFile(const char* pFilename);

	const float GetHeight(int x, int z) const;

	void GetMinMax(float& min, float& max) const;

	const float GetWorldScale() const;
	const int GetSize() const;

protected:
	int m_terrainSize = 0; // square
	float m_worldScale = 1.0f;
	float m_minHeight = 0.0f;
	float m_maxHeight = 0.0f;
	float m_textureScale = 1.0f;
	Array2D<float> m_heightMap;


	void LoadHeightMapFile(const char* pFilename);

	void CreateMesh(int terrainSize);
	void InitMesh();
	void InitVertex(int x, int z, Vertex& vertex);
	void InitVertices();
	void InitIndices();
	void CalcNormals();
};
