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


#define WIDTH 1280
#define HEIGHT 720
glm::mat4 view = glm::mat4(1.0f);
int b = 0;

void init(GLFWwindow* window) { }

void display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint createMainShaderProgram() {

	int success;
	char infoLog[512];

	const char* vertexShaderSource = "chap_4_simple_cube.vert";
	const char* fragmentShaderSource = "chap_4_simple_cube.frag";
	Shader shaderObject(vertexShaderSource, fragmentShaderSource);

	return shaderObject.shaderProgram;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		b++;
		if (b > 3) {
			b = 0;
		}

		if (b == 0) {
			glm::mat4 model = glm::mat4(1.0f);
			view = glm::translate(view, glm::vec3(20.0f, -0.0f, -5.0f));
			view = glm::rotate(view, glm::radians(130.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::rotate(view, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			std::cout << b << std::endl;
		} else if (b == 1) {
			view = glm::mat4(1.0f);
			view = glm::translate(view, glm::vec3(-40.0f, 0.0f, -10.0f));
			view = glm::rotate(view, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			std::cout << b << std::endl;
		} else if (b == 2) {
			view = glm::mat4(1.0f);
			view = glm::rotate(view, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::rotate(view, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			view = glm::translate(view, glm::vec3(-10.0f, 10.0f, 5.0f));
			std::cout << b << std::endl;
		} else if (b == 3) {
			view = glm::mat4(1.0f);
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
			view = glm::rotate(view, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			std::cout << b << std::endl;
		}
	}
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Chapter 4 simple cube", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_DYNAMIC_DRAW);

	GLuint vao = NULL;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint positionLoc = glGetAttribLocation(shaderProgram, "pos");
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint colorLoc = glGetAttribLocation(shaderProgram, "color");
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glm::mat4 model = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(20.0f, -0.0f, -5.0f));
	view = glm::rotate(view, glm::radians(130.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	
	while (!glfwWindowShouldClose(window)) {
		//view = glm::rotate(view, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.0f, -0.1f));

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());
		
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);

		glm::mat4 mvp = projection * view * model;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 20; j++) {
				for (int k = 0; k < 10; k++) {

					model =  glm::translate(model, glm::vec3(4.0f * (j+1), -4.0f * (k + 1), 4.0f * (i + 1)));
					model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
					mvp = projection * view * model;
					
					glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
					model = glm::mat4(1.0f);
				}
			}
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}