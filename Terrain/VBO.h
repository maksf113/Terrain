#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	//glm::vec3 color;
	glm::vec2 texUV;
};

class VBO 
{
private:
	GLuint ID;
public:
	VBO(std::vector<Vertex>&);
	VBO(GLfloat* vertices, GLsizeiptr size);
	const GLuint id() const;
	void Bind();
	void Unbind();
	void Delete();
};