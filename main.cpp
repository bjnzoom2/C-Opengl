#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void colorChange(GLFWwindow* window) {
	srand(time(NULL));

	float red = float((rand() % 11)) / 10;
	float green = float((rand() % 11)) / 10;
	float blue = float((rand() % 11)) / 10;

	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(window);
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 800;
	int height = 600;

	GLFWwindow* window = glfwCreateWindow(width, height, "Test window", NULL, NULL);

	if (window == NULL) {
		std::cout << "WINDOW FAILED\n";
		glfwTerminate();
		return -1;
	}
	else {
		std::cout << "WINDOW SUCCESS\n";
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window)) {
		double timePassed = glfwGetTime();

		if (int(timePassed) % 1 == 0) {
			colorChange(window);
		}

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}