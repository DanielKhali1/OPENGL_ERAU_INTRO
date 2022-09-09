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
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Simply just a cube", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);

	std::vector<GLfloat> vertex_position = { -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
									1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
									1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
									-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
									-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
									1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f }; // base – right back};


	GLfloat color_data[] = { 1.0f, 0.0f, 0.0f, 1.0f,
							 0.0f, 1.0f, 0.0f, 1.0f,
							 0.0f, 0.0f, 1.0f, 1.0f,
							 0.0f, 1.0f, 0.5f, 1.0f };

	std::vector<GLfloat> texCoords = {

		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
		// back colors
	};
	

	glEnable(GL_DEPTH_TEST);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);

	mesh pyramid = mesh::mesh("chap_5_0_textured_cube.vert", "chap_5_0_textured_cube.frag");
	pyramid.loadVertexPositions(&vertex_position, "aPos");
	pyramid.loadTextCoords(&texCoords, "tc");
	pyramid.loadTexture("image3.jpg");

	glGetError();

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = MatrixHelper::buildTranslate(0.0f, 0.0f, -6.0f) * MatrixHelper::buildRotateX(-20.0f) * MatrixHelper::buildRotateY(20.0f);
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
	
	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());

		view *= MatrixHelper::buildRotateX(1.0f);

		//draw here
		glm::mat4 mvp = perspective * view * model;
		pyramid.setUniformMatrix4fv(&mvp, "mvp");

		pyramid.DrawArray(18);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}

	pyramid.cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}