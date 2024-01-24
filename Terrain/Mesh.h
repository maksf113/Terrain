#pragma once
#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh 
{
protected:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;
	VAO m_vao;
	void CreateGLState(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

public:
	Mesh() {}
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

	virtual void Draw(Shader& shader, Camera& camera);
};