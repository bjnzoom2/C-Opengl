#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum PolyMode {Fill = 0, Line = 1};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\0";

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 800;
	int height = 800;

	GLFWwindow* window = glfwCreateWindow(width, height, "Test window", NULL, NULL);

	if (window == NULL) {
		std::cout << "WINDOW FAILED\n";
		glfwTerminate();
		return -1;
	}
	else {
		std::cout << "WINDOW SUCCESS\n";
	}

	GLfloat vertices1[] = {
		-0.75f, -0.25f, 0.0f,
		-0.25f, -0.25f, 0.0f,
		-0.50f, 0.25f, 0.0f
	};

	GLfloat vertices2[] = {
		0.75f, -0.25f, 0.0f,
		0.25f, -0.25f, 0.0f,
		0.50f, 0.25f, 0.0f
	};

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

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

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint VAO1, VBO1;

	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint VAO2, VBO2;

	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices1) / sizeof(vertices1[0]));

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2) / sizeof(vertices2[0]));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}