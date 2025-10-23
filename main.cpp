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
#include "camera.h"

enum PolyMode {Fill = 0, Line = 1};
enum Color {Red = 0, Green = 1, Blue = 2};
PolyMode mode = Fill;

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

bool firstMouse = true;
double lastX = 400;
double lastY = 400;

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		double lastX = xpos;
		double lastY = ypos;
		firstMouse = false;
	}
	double xOffset = xpos - lastX;
	double yOffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	camera.yaw += xOffset;
	camera.pitch -= yOffset;

	if (camera.pitch > 89.0f) {
		camera.pitch = 89.0f;
	}
	if (camera.pitch < -89.0f) {
		camera.pitch = -89.0f;
	}
	camera.getMouseDirection();
}

void checkInput(GLFWwindow *window, float deltatime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.cameraPos += camera.cameraSpeed * camera.cameraFront * deltatime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.cameraPos -= camera.cameraSpeed * camera.cameraFront * deltatime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.cameraPos -= camera.cameraSpeed * camera.cameraRight * deltatime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.cameraPos += camera.cameraSpeed * camera.cameraRight * deltatime;
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);

	if (window == NULL) {
		std::cout << "WINDOW FAILED\n";
		glfwTerminate();
		return -1;
	}
	else {
		std::cout << "WINDOW SUCCESS\n";
	}

	GLfloat vertices[] = {
		-0.25f, -0.25f, -0.25f, // 0
		0.25f, -0.25f, -0.25f, // 1
		-0.25f, 0.25f, -0.25f, // 2
		0.25f, 0.25f, -0.25f, // 3

		-0.25f, -0.25f, 0.25f, // 4
		0.25f, -0.25f, 0.25f, // 5
		-0.25f, 0.25f, 0.25f, // 6
		0.25f, 0.25f, 0.25f, // 7
	};

	GLuint indices[] = {
		0, 1, 2,
		1, 2, 3,

		4, 5, 6,
		5, 6, 7,

		2, 3, 6,
		3, 6, 7,

		0, 1, 4,
		1, 4, 5,

		1, 5, 3,
		5, 3, 7,

		0, 4, 2,
		4, 2, 6
	};

	srand(time(NULL));

	glm::vec3 cubePos[300] = {};

	for (unsigned int i = 1; i < sizeof(cubePos) / sizeof(cubePos[0]); i++) {
		glm::vec3 vector;
		for (unsigned int j = 0; j < 3; j++) {
			int isSigned = rand() % 2;
			if (isSigned % 2 == 0) {
				vector[j] = rand() % 26;
			}
			else {
				vector[j] = -(rand() % 26);
			}
		}
		cubePos[i] = vector;
	}

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

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint lightVAO;

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	switch (mode) {
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
	}

	float deltatime;
	float lastframe = 0;

	while (!glfwWindowShouldClose(window)) {
		float currentframe = glfwGetTime();
		deltatime = currentframe - lastframe;
		lastframe = currentframe;

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.up);

		camera.getDirection();

		shaderProgram.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
		shaderProgram.setVec3("lightColor", glm::vec3(0.7f, 0.0f, 0.0f));

		shaderProgram.setMat4("projection", proj);
		shaderProgram.setMat4("view", view);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		for (unsigned int i = 0; i < sizeof(cubePos) / sizeof(cubePos[0]); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePos[i]);
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

			shaderProgram.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		checkInput(window, deltatime);

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