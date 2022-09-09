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


#define WIDTH 600
#define HEIGHT 600

using namespace std;

void init(GLFWwindow* window) { }

void display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}


int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D model loading", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);

	glEnable(GL_DEPTH_TEST);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);

	mesh bulb = mesh::mesh("monkey.vert", "monkey.frag");
	bulb.loadOBJ("bulb.obj");
	bulb.loadTexture("Bulbasaur_BaseColor_1001.png", GL_TEXTURE0);

	mesh charm = mesh::mesh("monkey.vert", "monkey.frag");
	charm.loadOBJ("char.obj");
	charm.loadTexture("Charmander_BaseColor_1001.png", GL_TEXTURE1);

	mesh squirt = mesh::mesh("monkey.vert", "monkey.frag");
	squirt.loadOBJ("squirt.obj");
	squirt.loadTexture("Squirtle_BaseColor_1001.png", GL_TEXTURE2);

	glGetError();

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = MatrixHelper::buildTranslate(0.0f, 0.0f, -5.3f) * MatrixHelper::buildRotateX(10.0f);
	glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);


	bulb.setUniform1i(1, "decide");
	charm.setUniform1i(2, "decide");
	squirt.setUniform1i(3, "decide");

	bulb.model = MatrixHelper::buildTranslate(0.0f, 0.0f, 0.0f);
	charm.model = MatrixHelper::buildTranslate(2.0f, 0.0f, 0.0f);
	squirt.model = MatrixHelper::buildTranslate(-2.0f, 0.0f, 0.0f);


	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());

		//draw here
		bulb.model *= MatrixHelper::buildRotateY(1.0f);
		glm::mat4 mvp = perspective * view * bulb.model;
		bulb.setUniformMatrix4fv(&mvp, "mvp");
		bulb.DrawArray();


		charm.model *= MatrixHelper::buildRotateY(1.0f);
		mvp = perspective * view * charm.model;
		charm.setUniformMatrix4fv(&mvp, "mvp");
		charm.DrawArray();

		squirt.model *= MatrixHelper::buildRotateY(1.0f);
		mvp = perspective * view * squirt.model;
		squirt.setUniformMatrix4fv(&mvp, "mvp");
		squirt.DrawArray();

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}

	bulb.cleanup();
	charm.cleanup();
	squirt.cleanup();


	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}