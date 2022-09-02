#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <fstream>

#include <string>
#include <vector>


#define WIDTH 200
#define HEIGHT 200

using namespace std;


bool mv[6];

void objload(std::string filename) 
{
	std::ifstream ifs(filename.c_str(), std::ifstream::in);
}


void init(GLFWwindow* window) { }

void display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

GLuint createMainShaderProgram() {

	int success;
	char infoLog[512];

	const char* vertexShaderSource = "chap_2_1_pixel_pulse.vert";
	const char* fragmentShaderSource = "chap_2_1_pixel_pulse.frag";
	Shader shaderObject(vertexShaderSource, fragmentShaderSource);

	return shaderObject.shaderProgram;
}



int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "chap_2_1_pixel_pulse", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	
	glGetError();
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);


	GLuint shaderProgram = createMainShaderProgram();


	GLuint vao = NULL;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glUseProgram(shaderProgram);
	float size = 0.0f;
	float max_size = 20.0f;
	bool up = true;
	while (!glfwWindowShouldClose(window)) {
		if (up)
		{
			size += 1.0f;
			if (size >= max_size)
				up = false;
		}
		else
		{
			size -= 1.0f;
			if (size <= 0)
				up = true;
		}
		glPointSize(size);
		
		
		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());

		glDrawArrays(GL_POINTS, 0, 1);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}

	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}