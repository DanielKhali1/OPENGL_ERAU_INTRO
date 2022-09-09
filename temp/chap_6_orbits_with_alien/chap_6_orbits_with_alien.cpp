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

	mesh ufo = mesh::mesh("shader.vert", "shader.frag");
	ufo.loadOBJ("ufo.obj");
	ufo.loadTexture("ufo_diffuse.png", GL_TEXTURE0);
	ufo.loadTexture("ufo_diffuse_glow.png", GL_TEXTURE1);
	glGetError();

	mesh planet1 = mesh::mesh("shader.vert", "shader.frag");
	planet1.makeSphere(48);
	planet1.loadTexture("earth.jpg", GL_TEXTURE2);

	mesh plane = mesh::mesh("shader.vert", "shader.frag");
	plane.loadOBJ("plane.obj");
	plane.loadTexture("background.jpg", GL_TEXTURE3);

	glm::mat4 view = MatrixHelper::buildTranslate(0.0f, 0.0f, -3.3f) * MatrixHelper::buildRotateX(-10.0f);
	glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);

	std::vector<mesh> ufos;

	ufo.setUniform1i(1, "decide");
	planet1.setUniform1i(2, "decide");
	plane.setUniform1i(3, "decide");
	std::cout << planet1.numTriangles*3 << std::endl;

	ufo.model = MatrixHelper::buildScale(0.2f, 0.2f, 0.2f) * MatrixHelper::buildTranslate(-8.0f, 0.0f, 0.0f);
	planet1.model = MatrixHelper::buildTranslate(0.0f, 0.0f, 0.0f);
	plane.model = MatrixHelper::buildScale(20.0f, 15.0f, 10.f) * MatrixHelper::buildTranslate(0.0f, -0.35f, -2.0f) * MatrixHelper::buildRotateX(90.0f) ;
	float theta = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());

		glm::mat4 mvp = perspective * view * plane.model;
		plane.setUniformMatrix4fv(&mvp, "mvp");
		plane.DrawArray();

		//draw here
		ufo.model *= MatrixHelper::buildRotateY(-1.0f) * MatrixHelper::buildTranslate( 0.0f, glm::cos(theta) * 0.05f, 0.12f);
		theta += 0.01f;
		mvp = perspective * view * ufo.model;
		ufo.setUniformMatrix4fv(&mvp, "mvp");
		ufo.DrawArray();

		planet1.model *= MatrixHelper::buildRotateY(1.0f);
		mvp = perspective * view * planet1.model;
		planet1.setUniformMatrix4fv(&mvp, "mvp");
		planet1.DrawArray();

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();
	}

	plane.cleanup();
	ufo.cleanup();
	planet1.cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}