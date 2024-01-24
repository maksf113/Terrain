#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera
{
private:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	float FOVdeg = 45.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
public:
	int width;
	int height;

	float speed = 0.005f;
	float sensitivity = 30.0f; 

	bool firstClickEsc = true;
	bool locked = false;
	bool firstClickF = true;
	bool flashLight = true;

	Camera(int width, int height, glm::vec3 position);
	glm::vec3 Pos() { return Position; }
	// update camera + send to vert shader
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);    
	void Matrix(Shader& shader, const char* uniform);
	void Input(GLFWwindow* window, float dt);
	const glm::vec3 GetDirection() const { return Direction; }
	const glm::vec3 GetPosition() const { return Position; }
	const glm::vec3 GetUp() const { return Up; }
	const glm::mat4 GetProjection() const { return projection; }
	const glm::mat4 GetView() const { return view; }
	const glm::mat4 GetMatrix() const { return cameraMatrix; }
	const float GetFOVdeg() const { return FOVdeg; }
	const float GetNearPlane() const { return nearPlane; }
	const float GetFarPlane() const { return farPlane; }
	void SetFOVdeg(float fovDeg) { FOVdeg = fovDeg; }
	void SetPos(glm::vec3 pos) { Position = pos; }
	void SetDir(glm::vec3 dir) { Direction = dir; }
	void SetUp(glm::vec3 up) { Up = up; }
	void SetFarPlane(float FarPlane) { farPlane = FarPlane; }
};
