#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) 
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) 
{
	Camera::FOVdeg = FOVdeg;
	Camera::nearPlane = nearPlane;
	Camera::farPlane = farPlane;
	view = glm::lookAt(Position, Position + Direction, Up);
	projection = glm::perspective(glm::radians(FOVdeg), float(width) / float(height), nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) 
{
	

	glUniformMatrix4fv(glGetUniformLocation(shader.id(), uniform),
		1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Input(GLFWwindow* window, float dt) 
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += speed * Direction * dt;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position += speed * -Direction * dt;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position += speed * -glm::normalize(glm::cross(Direction, Up)) * dt;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += speed * glm::normalize(glm::cross(Direction, Up)) * dt;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		Position += speed * Up * dt;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		Position += speed * -Up * dt;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = 2.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		speed = 0.5f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !locked && firstClickEsc)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		locked = true;
		firstClickEsc = false;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && locked && firstClickEsc)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		locked = false;
		firstClickEsc = false;
	}
	if (locked)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newDirection = glm::rotate(Direction, glm::radians(-rotX), glm::normalize(glm::cross(Direction, Up)));
		if (!(glm::angle(newDirection, Up) <= glm::radians(5.0f)) || glm::angle(newDirection, -Up) <= glm::radians(5.0f))
			Direction = newDirection;

		Direction = glm::rotate(Direction, glm::radians(-rotY), Up);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		firstClickEsc = true;
	}


	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && firstClickF)
	{
		flashLight = !flashLight;
		firstClickF = false;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
	{
		firstClickF = true;
	}

}