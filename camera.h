#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
	public:
		Camera(glm::vec3 camPos, glm::vec3 camTarget) {
			cameraPos = camPos;
			cameraTarget = camTarget;
			cameraDir = glm::normalize(cameraPos - cameraTarget);
			cameraRight = glm::normalize(glm::cross(up, cameraDir));
			cameraUp = glm::cross(cameraRight, cameraDir);
		}

		Camera(glm::vec3 camPos) {
			cameraPos = camPos;
			cameraTarget = cameraPos + cameraFront;
			cameraDir = glm::normalize(cameraPos - cameraTarget);
			cameraRight = glm::normalize(glm::cross(up, cameraDir));
			cameraUp = glm::cross(cameraRight, cameraDir);
		}

		glm::vec3 cameraPos;
		glm::vec3 cameraTarget;
		glm::vec3 cameraDir;
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight;
		glm::vec3 cameraUp;
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		float pitch;
		float yaw = -90.0f;
		glm::vec3 direction;

		float cameraSpeed = 4.0f;

		void getDirection() {
			cameraTarget = cameraPos + cameraFront;
			cameraDir = glm::normalize(cameraPos - cameraTarget);
			cameraRight = glm::normalize(glm::cross(up, cameraDir));
			cameraUp = glm::cross(cameraRight, cameraDir);
		}

		void getMouseDirection() {
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

			cameraFront = glm::normalize(direction);
		}
};

#endif
