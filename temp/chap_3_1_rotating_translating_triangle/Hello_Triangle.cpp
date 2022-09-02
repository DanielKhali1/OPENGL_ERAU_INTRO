#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

#include <string>
#include <vector>


#define WIDTH 600
#define HEIGHT 600

using namespace std;

glm::mat4 view = glm::mat4(1.0f);
glm::vec3 position;


bool mv[6];

void objload(std::string filename) 
{
	std::ifstream ifs(filename.c_str(), std::ifstream::in);
}


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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_A) { if (action == GLFW_PRESS) { mv[0] = true; } else if (action == GLFW_RELEASE) { mv[0] = false; } }
	if (key == GLFW_KEY_D) { if (action == GLFW_PRESS) { mv[1] = true; } else if (action == GLFW_RELEASE) { mv[1] = false; } }
	if (key == GLFW_KEY_LEFT_CONTROL) { if (action == GLFW_PRESS) { mv[2] = true; } else if (action == GLFW_RELEASE) { mv[2] = false; } }
	if (key == GLFW_KEY_SPACE) { if (action == GLFW_PRESS) { mv[3] = true; } else if (action == GLFW_RELEASE) { mv[3] = false; } }
	if (key == GLFW_KEY_W) { if (action == GLFW_PRESS) { mv[4] = true; } else if (action == GLFW_RELEASE) { mv[4] = false; } }
	if (key == GLFW_KEY_S) { if (action == GLFW_PRESS) { mv[5] = true; } else if (action == GLFW_RELEASE) { mv[5] = false; } }

	view = glm::translate(glm::mat4(1.0f), position);
}

void move_events() {
	float sped = 0.03f;
	if (mv[0]) { position.x += sped; }
	if (mv[1]) { position.x -= sped; }
	if (mv[2]) { position.y += sped; }
	if (mv[3]) { position.y -= sped; }
	if (mv[5]) { position.z -= sped; }
	if (mv[4]) { position.z += sped; }

	view = glm::translate(glm::mat4(1.0f), position);
}

int main(void) {
	position = glm::vec3(0.0f, 0.0f, -5.0f);
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	GLFWwindow* window = glfwCreateWindow(600, 600, "KILL MYSELF", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	
	glGetError();
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);
	glfwSetKeyCallback(window, key_callback);

	GLfloat vertex_position[] = {0.0f, 0.5f, 0.0f,
							     0.0f, -0.5f, -0.5f,
							     0.5f, -0.5f, 0.5f,
								 -0.5, -0.5f, 0.5f};

	GLfloat color_data[] = { 1.0f, 0.0f, 0.0f, 1.0f,
							 0.0f, 1.0f, 0.0f, 1.0f,
							 0.0f, 0.0f, 1.0f, 1.0f,
							 0.0f, 1.0f, 0.5f, 1.0f};

	GLuint indices[] = { 2, 3, 0,
						 3, 0, 1,
						 2, 0, 1,
						 1, 3, 2};

	GLuint shaderProgram = createMainShaderProgram();
	glEnable(GL_DEPTH_TEST);

	GLuint VBO[2];
	glGenBuffers(2, VBO);
	// loading vertex data into a VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_position), vertex_position, GL_DYNAMIC_DRAW);

	// loading color data into a VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_DYNAMIC_DRAW);
	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// setting vertex vbo pointer and enabling the attrib array
	GLuint positionLoc = glGetAttribLocation(shaderProgram, "aPos");
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// setting color vbo pointer and enabling the attrib array
	GLuint colorLoc = glGetAttribLocation(shaderProgram, "fcolor");
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::translate(view, position);
	model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	glGetError();
	
	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, 600, 600);
		display(window, glfwGetTime());

		model = glm::rotate(model, glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mvp = projection  * view * model;
		GLuint mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, indices);


		glfwSwapBuffers(window);
		glfwPollEvents();
		move_events();
		glGetError();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}