#include "Terrain.h"

void BaseTerrain::InitTerrain(float worldScale, float textureScale, 
	const std::vector<std::string>& textureFilenames)
{
	m_worldScale = worldScale;
	m_textureScale = textureScale;
	for (int i = 0; i < textureFilenames.size(); i++)
	{
		Texture texture(textureFilenames[i].data(), "diffuse", i);
		m_textures.push_back(texture);
	}	
}


void BaseTerrain::LoadFromFile(const char* pFilename)
{
	LoadHeightMapFile(pFilename);
	//m_triangleList.CreateTriangleList(m_terrainSize, m_terrainSize, this);
}


void BaseTerrain::LoadHeightMapFile(const char* pFilename)
{
	int fileSize = 0;
	unsigned char* p = (unsigned char*)ReadBinaryFile(pFilename, fileSize); // checks in ReadBinaryFile

	assert(fileSize % sizeof(float) == 0);

	m_terrainSize = sqrtf(fileSize / sizeof(float));

	m_heightMap.InitArray2D(m_terrainSize, m_terrainSize, p);
}

void BaseTerrain::Draw(Shader& shader, Camera& camera)
{
	glm::mat4 VP = camera.GetMatrix();
	shader.Activate();
	shader.setMat4("camMatrix", VP);
	this->Mesh::Draw(shader, camera);
}

const float BaseTerrain::GetHeight(int x, int z) const
{
	return m_heightMap.Get(x, z);
}

void BaseTerrain::GetMinMax(float& min, float& max) const
{
	m_heightMap.GetMinMax(min, max);
}

const float BaseTerrain::GetWorldScale() const
{
	return m_worldScale;
}

void BaseTerrain::CreateMesh(int terrainSize)
{
	m_terrainSize = terrainSize;
	InitMesh();
	Mesh::CreateGLState(m_vertices, m_indices); // ^m_vertices, m_indices already initialized
}

void BaseTerrain::InitMesh()
{
	m_vertices.resize(m_terrainSize * m_terrainSize);
	InitVertices();
	int numQuads = (m_terrainSize - 1) * (m_terrainSize - 1);
	m_indices.resize(numQuads * 6);
	InitIndices();
	CalcNormals();
}
void BaseTerrain::InitVertex(int x, int z, Vertex& vertex)
{
	float y = this->GetHeight(x, z);
	float worldScale = this->GetWorldScale();
	vertex.position = glm::vec3(x * worldScale, y, z * worldScale);
	float size = (float)this->GetSize();
	vertex.texUV = glm::vec2((float)x / size, (float)z / size) * m_textureScale;
	vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f); // for triangle normals accumulation
}



void BaseTerrain::InitVertices()
{
	int index = 0;

	for (int z = 0; z < m_terrainSize; z++)
		for (int x = 0; x < m_terrainSize; x++)
		{
			assert(index < m_vertices.size());
			InitVertex(x, z, m_vertices[index]);
			index++;
		}
}

void BaseTerrain::InitIndices()
{
	int index = 0;

	for (int z = 0; z < m_terrainSize - 1; z++)
		for (int x = 0; x < m_terrainSize - 1; x++)
		{
			unsigned int indexBottomLeft = z * m_terrainSize + x;
			unsigned int indexBottomRight = z * m_terrainSize + x + 1;
			unsigned int indexTopLeft = (z + 1) * m_terrainSize + x;
			unsigned int indexTopRight = (z + 1) * m_terrainSize + x + 1;

			// top left triangle
			m_indices[index++] = indexBottomLeft;
			m_indices[index++] = indexTopLeft;
			m_indices[index++] = indexTopRight;

			// bottom right triangle
			m_indices[index++] = indexBottomLeft;
			m_indices[index++] = indexTopRight;
			m_indices[index++] = indexBottomRight;
		}
}
void BaseTerrain::CalcNormals()
{
	GLuint index = 0;
	// calculate normal of a triangle and add contribution to vertices
	for (GLuint i = 0; i < m_indices.size();)
	{
		GLuint index0 = m_indices[i++];
		GLuint index1 = m_indices[i++];
		GLuint index2 = m_indices[i++];

		glm::vec3 v1 = m_vertices[index1].position - m_vertices[index0].position;
		glm::vec3 v2 = m_vertices[index2].position - m_vertices[index0].position;
		glm::vec3 normal = glm::cross(v2, v1);
		normal = glm::normalize(normal);

		m_vertices[index0].normal += normal;
		m_vertices[index1].normal += normal;
		m_vertices[index2].normal += normal;
	}
	// normalize vertex normals
	for (GLuint i = 0; i < m_vertices.size(); i++)
		m_vertices[i].normal = normalize(m_vertices[i].normal);
}

const int BaseTerrain::GetSize() const
{
	return this->m_terrainSize;
}
