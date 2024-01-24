#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <stb\stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shaderClass.h"

class Texture {
private:
	GLuint ID;
	const char* type;
	GLuint unit;
	
public:
	aiString path;
	Texture(const char* image, const char* texType, GLuint slot);
	Texture(aiString sPath, std::string directory, std::string texType, GLuint slot);

	// assign texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	GLuint id() { return ID; }
	const char* Type() { return type; }
	void Bind();
	void Unbind();
	void Delete();

};