#pragma once
#include <gl/glew.h>
#include "VBO.h"

class VAO 
{
private:
	GLuint ID;
public:
	VAO();
	const GLuint id() const;
	void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, 
		GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};