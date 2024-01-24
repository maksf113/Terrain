#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) {
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	CreateGLState(vertices, indices);
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
	m_vertices = vertices;
	m_indices = indices;
	CreateGLState(vertices, indices);
}

void Mesh::CreateGLState(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	m_vao.Bind();
	VBO vbo(vertices);
	EBO ebo(indices);
	// link position, normal, texCoord
	m_vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	m_vao.linkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));

	m_vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera) 
{
	shader.Activate();
	m_vao.Bind();

	// number of textures (diffuse) and specular maps
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < m_textures.size(); i++) 
	{
		std::string num;
		std::string type = m_textures[i].Type();

		if (type == "diffuse")
			num = std::to_string(numDiffuse++);

		else if (type == "specular")
			num = std::to_string(numSpecular++);

		m_textures[i].texUnit(shader, (type + num).c_str(), i);
		m_textures[i].Bind();
	}
	shader.setVec3("camPos", camera.Pos());
	camera.Matrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}