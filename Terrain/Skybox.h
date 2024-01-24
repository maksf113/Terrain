#pragma once
#include "Mesh.h"

class Skybox
{
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint cubemapTexture;
	GLuint textureUnit;
	float Vertices[24] =
	{
		//   Coordinates
		-1.0f, -1.0f,  1.0f,//        7--------6
		 1.0f, -1.0f,  1.0f,//       /|       /|
		 1.0f, -1.0f, -1.0f,//      4--------5 |
		-1.0f, -1.0f, -1.0f,//      | |      | |
		-1.0f,  1.0f,  1.0f,//      | 3------|-2
		 1.0f,  1.0f,  1.0f,//      |/       |/
		 1.0f,  1.0f, -1.0f,//      0--------1
		-1.0f,  1.0f, -1.0f
	};

	unsigned int Indices[36] =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};
	void GenBuffers()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), &Indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GenCubeMapTex()
	{
		glGenTextures(1, &cubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void GenFacesFromFile(const char* path)
	{
		std::string facesCubemap[6] =
		{
			std::string(path) + "/right.png",
			std::string(path) + "/left.png",
			std::string(path) + "/top.png",
			std::string(path) + "/bottom.png",
			std::string(path) + "/front.png",
			std::string(path) + "/back.png"
		};

		for (unsigned int i = 0; i < 6; i++)
		{
			int Width, Height, numColCh;
			unsigned char* data = stbi_load(facesCubemap[i].c_str(), &Width, &Height, &numColCh, 0);
			GLenum format = GL_RGBA;
			if (numColCh == 1)
				format = GL_RED;
			else if (numColCh == 3)
				format = GL_RGB;
			else if (numColCh == 4)
				format = GL_RGBA;

			if (data)
			{
				GLclearError();
				stbi_set_flip_vertically_on_load(false);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
				glTexImage2D
				(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_RGB,
					Width,
					Height,
					0,
					format,
					GL_UNSIGNED_BYTE,
					data
				);
				GLcheckError();
			}
			else
				std::cout << "Failed to load image: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}



public:
	Skybox(const char* path, GLuint slot) : textureUnit(slot)
	{
		GenBuffers();
		GenCubeMapTex();
		GenFacesFromFile(path);
	}

	void Draw(Shader shader, Camera camera)
	{
		shader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetDirection(), camera.GetUp())));
		projection = glm::perspective(glm::radians(camera.GetFOVdeg()), (float)camera.width / camera.height, camera.GetNearPlane(), camera.GetFarPlane());

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setInt("skybox", textureUnit);
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		//glDisable(GL_DEPTH_TEST);
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//glEnable(GL_DEPTH_TEST);
	}

};
