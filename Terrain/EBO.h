#pragma once
#include <gl/glew.h>
#include <vector>

class EBO 
{
private:
	GLuint ID;
public:
	EBO() {}
	EBO(std::vector<GLuint>& indices);
	EBO(GLuint* indices, GLsizeiptr size);
	const GLuint id() const;
	void Bind();
	void Unbind();
	void Delete();
}; 