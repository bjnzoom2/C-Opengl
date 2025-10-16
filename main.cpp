#include <iostream>
#include <vector>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "stb_image.h"

enum PolyMode {Fill = 0, Line = 1};
enum Color {Red = 0, Green = 1, Blue = 2};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoords;\n"
"out vec4 color;\n"
"out vec2 texCoords;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   color = vec4(aColor, 1.0);\n"
"   texCoords = aTexCoords;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 color;\n"
"in vec2 texCoords;\n"
"uniform sampler2D textureImage;\n"
"void main()\n"
"{\n"
"   FragColor = texture(textureImage, texCoords);\n"
"}\0";

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
		-0.25f, -0.25f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		0.25f, -0.25f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		0.0f, 0.25f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.0f
	};

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, windowWidth, windowHeight);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shaderProgram, "textureImage"), 0);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}