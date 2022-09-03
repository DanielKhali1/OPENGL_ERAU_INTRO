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


/* 

we indicated that Euler angles can in
some cases lead to undesirable artifacts. The most common is called “gimbal
lock.” Describe gimbal lock, give an example, and explain why gimbal lock
can cause problems

	when 2 Euler axis align.. each axis of rotation must be rotated to realign the object to to target rotation. This can cause strange artifacts that 
	are not expected when it comes to animating.

*/


using namespace std;

void init(GLFWwindow* window) { }

void display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint createMainShaderProgram() {

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

	GLuint vao = NULL;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glm::mat4 rotationMatrix = MatrixHelper::buildRotateZ(90.0f);

	float deg = 0.0f;

	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, 600, 600);
		display(window, glfwGetTime());


		glUseProgram(shaderProgram);

		float deltaTime = glfwGetTime() - prevTime;
		prevTime = glfwGetTime();

		x += inc * glm::abs(deltaTime);
		if (x > 1.0f) inc = -1.0f;
		if (x < -1.0f) inc = 1.0f;

		glm::mat4 translationMatrix = MatrixHelper::buildTranslate(x, 0.0, 0.0);
		glm::mat4 rotation2Matrix = MatrixHelper::buildRotateY(deg);


		deg += 3.0f;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "rotateMatrix"), 1, GL_FALSE,  glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "rotation2Matrix"), 1, GL_FALSE, glm::value_ptr(rotation2Matrix));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));

		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//std::cout << glGetError() << std::endl;
		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}

	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}