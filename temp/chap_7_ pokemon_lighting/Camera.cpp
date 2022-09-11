#include "Camera.h"


Camera::Camera() {
	//dumb
}

Camera::Camera(GLFWwindow* window) {
	int HEIGHT;
	int WIDTH;
	glfwGetWindowSize(window, &WIDTH, &HEIGHT);
	firstMouse = true;
	lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;

	for (int i = 0; i < sizeof(mv); i++) {
		mv[i] = false;
	}
}

void Camera::mouse_callback(double xoffset, double yoffset) {

	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

	YAW += xoffset;
	PITCH += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (PITCH > 89.0f)
		PITCH = 89.0f;
	if (PITCH < -89.0f)
		PITCH = -89.0f;

	updateCameraVectors();
};

void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
	front.y = sin(glm::radians(PITCH));
	front.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
	cameraFront = glm::normalize(front);
	// also re-calculate the Right and Up vector
	cameraRight = glm::normalize(glm::cross(cameraFront, up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::cameraKeyMovement(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_A) { if (action == GLFW_PRESS) { std::cout << "left" << std::endl; mv[0] = true; } else if (action == GLFW_RELEASE) { mv[0] = false; } }
	if (key == GLFW_KEY_D) { if (action == GLFW_PRESS) { mv[1] = true; } else if (action == GLFW_RELEASE) { mv[1] = false; } }
	if (key == GLFW_KEY_LEFT_CONTROL) { if (action == GLFW_PRESS) { mv[2] = true; } else if (action == GLFW_RELEASE) { mv[2] = false; } }
	if (key == GLFW_KEY_SPACE) { if (action == GLFW_PRESS) { mv[3] = true; } else if (action == GLFW_RELEASE) { mv[3] = false; } }
	if (key == GLFW_KEY_W) { if (action == GLFW_PRESS) { mv[4] = true; } else if (action == GLFW_RELEASE) { mv[4] = false; } }
	if (key == GLFW_KEY_S) { if (action == GLFW_PRESS) { mv[5] = true; } else if (action == GLFW_RELEASE) { mv[5] = false; } }
}

void Camera::move_events(float deltaTime) {

	float velocity = SPEED * deltaTime;

	std::string str;
	

	std::cout << str;	// left
	if (mv[0]) { cameraPos -= cameraRight * velocity; }
	// right
	if (mv[1]) { cameraPos += cameraRight * velocity; }
	// down
	if (mv[2]) { cameraPos.y -= velocity; }
	// up
	if (mv[3]) { cameraPos.y += velocity; }
	// forward
	if (mv[5]) { cameraPos -= cameraFront * velocity; }
	//backward
	if (mv[4]) { cameraPos += cameraFront * velocity; }
}