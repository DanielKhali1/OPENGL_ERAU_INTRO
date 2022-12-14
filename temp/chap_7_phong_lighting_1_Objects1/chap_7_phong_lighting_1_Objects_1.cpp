#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include <fstream>
#include "MatrixHelper.h"
#include <string>
#include <vector>
#include <SOIL2/SOIL2.h>
#include "mesh.h"
#include "Camera.h"


float WIDTH = 600;
float HEIGHT = 600;

using namespace std;

glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 4.0);

float deltaTime;

Camera camera;

bool lockcamerain = true;

void init(GLFWwindow* window) { }

void display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	WIDTH = static_cast<float> (width);
	HEIGHT = static_cast<float> (height);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE) { if (action == GLFW_PRESS) { lockcamerain = !lockcamerain; } }
	
	
	if (!lockcamerain) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	else {

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	camera.cameraKeyMovement(window, key, scancode, action, mods);
}
void mouse_callback(GLFWwindow* window, double xposz, double yposz) {

	float xpos = static_cast<float>(xposz);
	float ypos = static_cast<float>(yposz);
	if (lockcamerain) {

		if (camera.firstMouse)
		{
			camera.lastX = xpos;
			camera.lastY = ypos;
			camera.firstMouse = false;
		}
		float xoffset = xpos - camera.lastX;
		float yoffset = camera.lastY - ypos; // reversed since y-coordinates go from bottom to top

		camera.lastX = xpos;
		camera.lastY = ypos;

		camera.mouse_callback(xoffset, yoffset);

		glfwSetCursorPos(window, WIDTH / 2.0f, HEIGHT / 2.0f);
		camera.lastX = WIDTH / 2.0f;
		camera.lastY = HEIGHT / 2.0f;
	}
};

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D model loading", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);

	camera = Camera::Camera(window);
	glfwSetCursorPos(window, WIDTH / 2.0, HEIGHT / 2.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);


	mesh brick_plane = mesh::mesh("shader.vert", "shader.frag");
	brick_plane.loadOBJ("plane.obj");
	brick_plane.loadTexture("brick.jpg", GL_TEXTURE1);

	mesh helmet = mesh::mesh("shader.vert", "shader.frag");
	helmet.loadOBJ("Grrek_Helmet_01.obj");
	helmet.loadTexture("Grrek_Helmet_low_map_GreekHelmet01_BaseColor.png", GL_TEXTURE0);

	mesh light = mesh::mesh("light.vert", "light.frag");
	light.makeSphere(48);

	camera.updateCameraVectors();
	//glm::mat4 view = MatrixHelper::buildTranslate(0.0f, 0.0f, -5.3f) * MatrixHelper::buildRotateX(-10.0f);
	glm::mat4 perspective = glm::perspective((float)glm::radians((float)HEIGHT*0.1), (float)WIDTH/(float)HEIGHT, 0.1f, 50.0f);
	glm::mat4 view = camera.getViewMatrix(); 

	brick_plane.setUniform1i(2, "decide");
	helmet.setUniform1i(1, "decide");

	std::cout << brick_plane.numTriangles*3 << std::endl;

	brick_plane.model = MatrixHelper::buildTranslate(0.0f, -2.0f, 0.0f) * MatrixHelper::buildScale(4.0f, 4.0f, 4.0f);
	//helmet.model = MatrixHelper::buildTranslate(0.0f, 1.0f, 0.0f);

	glm::mat4 theta = glm::mat4(1.0f);
	bool left = false;

	helmet.loadMaterialConstants(250.0f,
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.8f, 0.017155f, 0.043768f),
		glm::vec3(0.5f, 0.5f, 0.5f));

	float lastFrame = static_cast<float>(glfwGetTime());
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());

		camera.move_events(deltaTime);
		camera.updateCameraVectors();
		view = camera.getViewMatrix();
		perspective = glm::perspective((float)glm::radians((float)HEIGHT * 0.1), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		brick_plane.setUniform1i(2, "decide");
		glm::mat4 mvp = perspective * view * brick_plane.model ;
		glUniform3f(glGetUniformLocation(brick_plane.shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(brick_plane.shaderProgram, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(brick_plane.shaderProgram, "view"), 1, &camera.cameraPos[0]);
		brick_plane.setUniformMatrix4fv(&brick_plane.model, "model");
		brick_plane.setUniformMatrix4fv(&mvp, "mvp");
		brick_plane.DrawArray();


		helmet.setUniform1i(1, "decide");
		helmet.loadMaterialConstants(250.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.8f, 0.017155f, 0.043768f),
			glm::vec3(0.5f, 0.5f, 0.5f));
		helmet.model *= MatrixHelper::buildRotateY(1.0f);
		mvp = perspective * view * helmet.model;
		glUniform3f(glGetUniformLocation(helmet.shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(helmet.shaderProgram, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(helmet.shaderProgram, "view"), 1, &camera.cameraPos[0]);
		helmet.setUniformMatrix4fv(&helmet.model, "model");
		helmet.setUniformMatrix4fv(&mvp, "mvp");
		helmet.DrawArray();

		light.model = MatrixHelper::buildTranslate(lightPos.x, lightPos.y, lightPos.z) * MatrixHelper::buildScale(.2f, .2f, .2f);
		mvp = perspective * view * light.model;
		light.setUniformMatrix4fv(&mvp, "mvp");
		light.DrawArray();

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError(); 
	}

	brick_plane.cleanup();
	helmet.cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}