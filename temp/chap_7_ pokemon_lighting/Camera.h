#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera
{
private:
	float YAW = -90.0f;
	float PITCH = 0.0f;
	float SPEED = 2.5f;
	float SENSITIVITY = 0.1f;
	float ZOOM = 45.0f;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -0.0);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	bool mv[6];
	float firstMouse = true;
	float lastX;
	float lastY;
	// pointer to the mouse callback function that we can insert into glfwMouseCallback
	void mouse_callback(double xoffset, double yoffset);
	Camera();
	Camera(GLFWwindow* window);
	void updateCameraVectors();
	glm::mat4 getViewMatrix();
	void move_events(float deltaTime);
	void cameraKeyMovement(GLFWwindow* window, int key, int scancode, int action, int mods);
};

