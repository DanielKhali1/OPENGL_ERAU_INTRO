#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include <fstream>

#include <string>
#include <vector>


#define WIDTH 600
#define HEIGHT 600

using namespace std;

void init(GLFWwindow* window) { }

void display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint createMainShaderProgram() {

	int success;
	char infoLog[512];

	const char* vertexShaderSource = "hello_triangle.vert";
	const char* fragmentShaderSource = "hello_triangle.frag";
	Shader shaderObject(vertexShaderSource, fragmentShaderSource);

	return shaderObject.shaderProgram;
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	GLFWwindow* window = glfwCreateWindow(600, 600, "INSERT_PROJECT_NAME_HERE", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	
	glGetError();
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);


	GLuint shaderProgram = createMainShaderProgram();
	glEnable(GL_DEPTH_TEST);


	glGetError();
	
	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, 600, 600);
		display(window, glfwGetTime());


		glUseProgram(shaderProgram);
		//draw here


		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}

	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}