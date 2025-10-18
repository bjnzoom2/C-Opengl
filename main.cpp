#include <iostream>
#include <vector>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"

enum PolyMode {Fill = 0, Line = 1};
enum Color {Red = 0, Green = 1, Blue = 2};

void checkInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	unsigned int windowWidth = 800;
	unsigned int windowHeight = 800;

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Test window", NULL, NULL);

	if (window == NULL) {
		std::cout << "WINDOW FAILED\n";
		glfwTerminate();
		return -1;
	}
	else {
		std::cout << "WINDOW SUCCESS\n";
	}

	GLfloat vertices[] = {
		-0.25f, -0.25f, -0.25f,   0.0f, 0.0f, // 0
		0.25f, -0.25f, -0.25f,   1.0f, 0.0f, // 1
		-0.25f, 0.25f, -0.25f,   0.0f, 1.0f, // 2
		0.25f, 0.25f, -0.25f,   1.0f, 1.0f, // 3

		-0.25f, -0.25f, 0.25f,   0.0f, 0.0f, // 4
		0.25f, -0.25f, 0.25f,   1.0f, 0.0f, // 5
		-0.25f, 0.25f, 0.25f,   0.0f, 1.0f, // 6
		0.25f, 0.25f, 0.25f,   1.0f, 1.0f, // 7

		-0.25f, 0.25f, -0.25f,   0.0f, 0.0f, // 8
		0.25f, 0.25f, -0.25f,   1.0f, 0.0f, // 9

		-0.25f, -0.25f, 0.25f,   0.0f, 1.0f, // 10
		0.25f, -0.25f, 0.25f,   1.0f, 1.0f, // 11

		0.25f, -0.25f, -0.25f,   0.0f, 0.0f, // 12
		0.25f, 0.25f, -0.25f,   0.0f, 1.0f, // 13

		-0.25f, -0.25f, 0.25f,   1.0f, 0.0f, // 14
		-0.25f, 0.25f, 0.25f,   1.0f, 1.0f, // 15
	};

	GLuint indices[] = {
		0, 1, 2,
		1, 2, 3,

		4, 5, 6,
		5, 6, 7,

		8, 9, 6,
		9, 6, 7,

		0, 1, 10,
		1, 10, 11,

		12, 5, 13,
		5, 13, 7,

		0, 14, 2,
		14, 2, 15
	};

	glm::vec3 cubePos[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		
	};

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, windowWidth, windowHeight);

	Shader shaderProgram("vertexShader.vert", "fragmentShader.frag"); 

	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int texWidth, texHeight, texColorChannels;
	std::filesystem::path filename = R"(C:\Users\luken\Downloads\smile.jpg)";
    unsigned char* data = stbi_load(filename.string().c_str(), &texWidth, &texHeight, &texColorChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture";
	}

	stbi_image_free(data);

	PolyMode mode = Fill;

	switch (mode) {
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
	}


	float num = 0;

	while (!glfwWindowShouldClose(window)) {
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		shaderProgram.setInt("textureImage", 0);

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		shaderProgram.setMat4("projection", proj);
		shaderProgram.setMat4("view", view);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		for (unsigned int i = 0; i < sizeof(cubePos) / sizeof(cubePos[0]); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePos[i]);
			model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			shaderProgram.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		checkInput(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram.ID);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}