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


#define WIDTH 1080
#define HEIGHT 720

using namespace std;

void init(GLFWwindow* window) { }

void display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint createMainShaderProgram() {

	int success;
	char infoLog[512];

	const char* vertexShaderSource = "chap_4_3_orbitting_cubes.vert";
	const char* fragmentShaderSource = "chap_4_3_orbitting_cubes.frag";
	Shader shaderObject(vertexShaderSource, fragmentShaderSource);

	return shaderObject.shaderProgram;
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Orbitting Cubes", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	
	glGetError();
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);

	GLuint indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat vertices[] = {
		// front
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0
	};

	GLfloat colorData[] = {
		// front colors
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
		// back colors
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0
	};

	GLuint shaderProgram = createMainShaderProgram();
	glEnable(GL_DEPTH_TEST);

	GLuint VBO[2];
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	GLuint positionLoc = glGetAttribLocation(shaderProgram, "aPos");
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(positionLoc);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	GLuint colorLoc = glGetAttribLocation(shaderProgram, "fcolor");
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(colorLoc);

	glGetError();
	
	glm::mat4 model1 = glm::mat4(1.0f);
	glm::mat4 model2 = MatrixHelper::buildScale(0.8f, 0.8f, 0.8f) * MatrixHelper::buildTranslate(7.0f, 0.0f, 0.0f);
	glm::mat4 model3 = MatrixHelper::buildScale(0.5f, 0.5f, 0.5f) * MatrixHelper::buildTranslate(0.0f, 0.0f, -4.3f);
	glm::mat4 model4 = MatrixHelper::buildTranslate(-7.0f, 1.0f, 0.0f);

	glm::mat4 view = MatrixHelper::buildTranslate(0.0f, 0.0f, -20.0f) * MatrixHelper::buildRotateX(-15.0f) * MatrixHelper::buildRotateY(20.0f);
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.01f, 100.0f);

	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());


		model1 *= MatrixHelper::buildRotateY(0.1f);
		glm::mat4 mvp = perspective * view * model1;
		glUseProgram(shaderProgram);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

		model2 *= MatrixHelper::buildTranslate(0.0f, 0.0f, 0.1f) * MatrixHelper::buildRotateY(1.0f);
		mvp = perspective * view * model2;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

		model3 *= MatrixHelper::buildTranslate(0.0f, -0.2f, 0.0) * MatrixHelper::buildRotateX(2.1f);
		mvp = perspective * view * model2 * model3;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

		model4 *= MatrixHelper::buildTranslate(0.0f, 0.0f, -0.1f) * MatrixHelper::buildRotateY(0.8f)  ;
		mvp = perspective * view *  model4;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}

	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}