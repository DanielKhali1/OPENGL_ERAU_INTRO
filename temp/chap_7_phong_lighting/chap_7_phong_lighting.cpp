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

bool mv[6];

glm::vec3 lightPos = glm::vec3(2.0f, 0.0f, 0.0);

void init(GLFWwindow* window) { }

void display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_A) { if (action == GLFW_PRESS) { mv[0] = true; } else if (action == GLFW_RELEASE) { mv[0] = false; } }
	if (key == GLFW_KEY_D) { if (action == GLFW_PRESS) { mv[1] = true; } else if (action == GLFW_RELEASE) { mv[1] = false; } }
	if (key == GLFW_KEY_LEFT_CONTROL) { if (action == GLFW_PRESS) { mv[2] = true; } else if (action == GLFW_RELEASE) { mv[2] = false; } }
	if (key == GLFW_KEY_SPACE) { if (action == GLFW_PRESS) { mv[3] = true; } else if (action == GLFW_RELEASE) { mv[3] = false; } }
	if (key == GLFW_KEY_W) { if (action == GLFW_PRESS) { mv[4] = true; } else if (action == GLFW_RELEASE) { mv[4] = false; } }
	if (key == GLFW_KEY_S) { if (action == GLFW_PRESS) { mv[5] = true; } else if (action == GLFW_RELEASE) { mv[5] = false; } }
	std::cout << glGetError() << "\n";
	//view = glm::translate(glm::mat4(1.0f), position);
}

void move_events() {
	float sped = 0.03f;
	if (mv[0]) { lightPos.x -= sped; }
	if (mv[1]) { lightPos.x += sped; }
	if (mv[2]) { lightPos.y -= sped; }
	if (mv[3]) { lightPos.y += sped; }
	if (mv[5]) { lightPos.z += sped; }
	if (mv[4]) { lightPos.z -= sped; }
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D model loading", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);

	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(window, key_callback);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);

	GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);
	

	//mesh planet1 = mesh::mesh("shader.vert", "shader.frag");
	//planet1.loadOBJ("Grrek_Helmet_01.obj");
	//planet1.loadTexture("Grrek_Helmet_low_map_GreekHelmet01_BaseColor.png", GL_TEXTURE0);

	mesh planet1 = mesh::mesh("shader.vert", "shader.frag");
	planet1.loadOBJ("nathan.obj");
	planet1.loadTexture("nathan.jpg", GL_TEXTURE0);

	mesh light = mesh::mesh("light.vert", "light.frag");
	light.makeSphere(48);

	glm::mat4 view = MatrixHelper::buildTranslate(0.0f, 0.0f, -5.3f) * MatrixHelper::buildRotateX(-10.0f);
	glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 10.0f);

	planet1.setUniform1i(1, "decide");
	std::cout << planet1.numTriangles*3 << std::endl;

	planet1.model = MatrixHelper::buildScale(2.0f, 2.0f, 2.0f); MatrixHelper::buildTranslate(0.0f, 0.0f, 5.0f);

	light.model = MatrixHelper::buildTranslate(lightPos.x, lightPos.y, lightPos.z) * MatrixHelper::buildScale(.2f, .2f, .2f)  ;
	glm::mat4 theta = glm::mat4(1.0f);
	bool left = false;


	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());
		move_events();

		planet1.setUniform1i(1, "decide");
		glm::mat4 mvp = perspective * view * planet1.model ;
		glUniform3f(glGetUniformLocation(planet1.shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(planet1.shaderProgram, "lightPos"), 1, &lightPos[0]);
		planet1.setUniformMatrix4fv(&planet1.model, "model");
		planet1.setUniformMatrix4fv(&mvp, "mvp");
		planet1.DrawArray();

		light.model = MatrixHelper::buildTranslate(lightPos.x, lightPos.y, lightPos.z) * MatrixHelper::buildScale(.2f, .2f, .2f);
		mvp = perspective * view * light.model;
		light.setUniformMatrix4fv(&mvp, "mvp");
		light.DrawArray();

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError(); 
	}

	planet1.cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}