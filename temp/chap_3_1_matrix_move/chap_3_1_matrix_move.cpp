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

	const char* vertexShaderSource = "chap_3_1_matrix_move.vert";
	const char* fragmentShaderSource = "chap_3_1_matrix_move.frag";
	Shader shaderObject(vertexShaderSource, fragmentShaderSource);

	return shaderObject.shaderProgram;
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	GLFWwindow* window = glfwCreateWindow(600, 600, "chap_3_1_matrix_move", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	
	glGetError();
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);


	GLuint shaderProgram = createMainShaderProgram();
	glEnable(GL_DEPTH_TEST);


	glGetError();
	float x = 0.0f;
	float inc = 1.0f;

	float prevTime = 0.0f;

	glm::mat4 rotationMatrix = MatrixHelper::buildRotateY(90.0f);

	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, 600, 600);
		display(window, glfwGetTime());


		glUseProgram(shaderProgram);

	/*	float deltaTime = glfwGetTime() - prevTime;
		prevTime = glfwGetTime();

		x += inc * glm::abs(deltaTime);
		if (x > 1.0f) inc = -1.0f;
		if (x < -1.0f) inc = 1.0f;

		glProgramUniform4fv(shaderProgram, glGetUniformLocation(shaderProgram, "rotateMatrix"), 1, glm::value_ptr(rotationMatrix));
		glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "offset"), x);
		*/
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}

	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}