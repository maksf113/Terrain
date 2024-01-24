#include <stb\stb_image_write.h>
#include "Skybox.h"
#include "Terrain.h"
#include "FaultFormationTerrain.h"
#include "MidpointDisplacementTerrain.h"


#define numPointLights 3

unsigned int width = 1980;
unsigned int height = 1080;
float prevTime = 0.0f;
float crntTime = 0.0f;
float dt = 0.0f;

int main(void) 
{
	// Initialize GLFW
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	// Tell GLFW what version of OpenGL we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(width, height, "Chapter 2 - program1", NULL, NULL);
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, width, height);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	
	

	Shader skyboxShader("skybox.vert", "skybox.frag");
	Skybox skybox("skybox/violet_scaled", 0);


	Camera camera(width, height, glm::vec3(0.0f, 10.5f, 0.0f));
	camera.SetDir(glm::vec3(-1.2, -0.7, -0.0));
	camera.SetPos(glm::vec3(23.3, 24.0, -1.2));
	int counter = 0;
	
	FaultFormationTerrain terrain;
	//MidpointDisplacementTerrain terrain;
	float worldScale = 0.1f;
	float textureScale = 10.0f;
	std::vector<std::string> textureFilenames;
	textureFilenames.push_back("Textures/ter0.jpg");
	textureFilenames.push_back("Textures/ter1.jpg");
	textureFilenames.push_back("Textures/ter2.png");
	textureFilenames.push_back("Textures/ter3.png");
	terrain.InitTerrain(worldScale, textureScale, textureFilenames);
	float minHeight = 0.0f;
	float maxHeight = 20.0f;
	int iterations = 1000;
	int terrainSize = 300;
	float filter = 1.0;
	int smoothIter = 0;
	float falloff = 0.1;
	terrain.CreateFaultFormation(terrainSize, iterations, minHeight, maxHeight, falloff, filter, smoothIter);
	float roughness = 1.0f;
	//terrain.CreateMidpointDisplacement(terrainSize, roughness, minHeight, maxHeight);
	checkOpenGLError();
	Shader terrainShader("terrain.vert", "terrain.frag");
	terrainShader.Activate();
	terrainShader.setMat4("camMatrix", camera.GetMatrix());
	terrainShader.setFloat("minHeight", minHeight);
	terrainShader.setFloat("maxHeight", maxHeight);
	terrainShader.setFloat("h0", 2.0f);
	terrainShader.setFloat("h1", 5.0f);
	terrainShader.setFloat("h2", 8.0f);
	terrainShader.setFloat("h3", 13.0f);

	while (!glfwWindowShouldClose(window)) 
	{
		crntTime = glfwGetTime();
		dt = crntTime - prevTime;
		counter++;
		if (dt >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string(1.0 / dt * counter);
			std::string ms = std::to_string(dt / counter * 1000);
			std::string newTitle = "Project 8 - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			counter = 0;
			prevTime = crntTime;
			glm::vec3 pos = camera.GetPosition();
			glm::vec3 dir = camera.GetDirection();
			//std::cout << "pos: " << pos.x << "\t" << pos.y << "\t" << pos.z << "\n";
			//std::cout << "dir: " << dir.x << "\t" << dir.y << "\t" << dir.z << "\n\n";
		}
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, float(crntTime / 5.0), glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, glm::vec3(-terrainSize * worldScale / 2.0, 0.0, -terrainSize * worldScale / 2.0));
		glm::mat3 transposeInverse = glm::transpose(glm::inverse(glm::mat3(model)));
		

		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		skyboxShader.Activate();
		skybox.Draw(skyboxShader, camera); 
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		
		
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);
		camera.Input(window, dt);

		terrainShader.Activate();
		terrainShader.setMat4("camMatrix", camera.GetMatrix());
		terrainShader.setMat4("model", model);
		terrainShader.setMat3("transposeInverse", transposeInverse);
		terrain.Draw(terrainShader, camera);
		checkOpenGLError();


		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	checkOpenGLError();
	skyboxShader.Delete();
	terrainShader.Delete();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}



