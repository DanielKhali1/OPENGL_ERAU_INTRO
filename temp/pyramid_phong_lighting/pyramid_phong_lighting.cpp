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


#define WIDTH 1280
#define HEIGHT 720

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

void calculateSurfaceNormals(std::vector<GLfloat>* normals, std::vector <GLfloat>* vertex_data) {

	for (int i = 0; i*3 < vertex_data->size(); i += 3) {
		std::cout << vertex_data->size() << " " << i*3 + 2 << "\n";
		glm::vec3 p1 = glm::vec3((*vertex_data)[i * 3], (*vertex_data)[i * 3 + 1], (*vertex_data)[i * 3 + 2]);
		glm::vec3 p2 = glm::vec3((*vertex_data)[(i + 1) * 3], (*vertex_data)[(i + 1) * 3 + 1], (*vertex_data)[(i + 1) * 3 + 2]);
		glm::vec3 p3 = glm::vec3((*vertex_data)[(i + 2) * 3], (*vertex_data)[(i + 2) * 3 + 1], (*vertex_data)[(i + 2) * 3 + 2]);


		glm::vec3 u = p2 - p1;
		glm::vec3 v = p3 - p1;

		std::cout << "u: " << u.x << " " << u.y << "  " << u.z << std::endl;
		std::cout << "v: " << v.x << " " << v.y << "  " << v.z << std::endl;


		for (int i = 0; i < 3; i++) {

		glm::vec3 normal = glm::normalize(glm::cross(v, u));
		normals->push_back(normal.x);
		normals->push_back(normal.y);
		normals->push_back(normal.z);
		std::cout << normal.x << " " << normal.y << "  " << normal.z << std::endl;
		}

	}
}

std::vector<GLfloat> vertex_position = { -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
								1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
								1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
								-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
								-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
								1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f }; // base – right back};


std::vector<GLfloat> texCoords = {

	0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	// back colors
};

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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);

	mesh pyramid = mesh::mesh("shader.vert", "shader.frag");
	pyramid.loadVertexPositions(&vertex_position, "aPos");
	std::vector<GLfloat> normal;
	calculateSurfaceNormals(&normal, &vertex_position);
	pyramid.loadSurfaceNormals(&normal, "aNorm");



	mesh light = mesh::mesh("light.vert", "light.frag");
	light.makeSphere(48);

	glm::mat4 view = MatrixHelper::buildTranslate(0.0f, 0.0f, -7) * MatrixHelper::buildRotateX(-20.0f) * MatrixHelper::buildRotateY(-10.0f);
	glm::mat4 perspective = glm::perspective(glm::radians(60.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 20.0f);

	pyramid.setUniform1i(1, "decide");
	std::cout << pyramid.numTriangles*3 << std::endl;

	pyramid.model = MatrixHelper::buildScale(2.0f, 2.0f, 2.0f); MatrixHelper::buildTranslate(0.0f, 0.0f, 5.0f);

	light.model = MatrixHelper::buildTranslate(lightPos.x, lightPos.y, lightPos.z) * MatrixHelper::buildScale(.2f, .2f, .2f)  ;
	bool left = false;

	float theta = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());
		move_events();

		pyramid.setUniform1i(2, "decide");
		//pyramid.model *= MatrixHelper::buildRotateY(1.0f);
		view *= MatrixHelper::buildTranslate(-0.001, 0.0, 0.0) * MatrixHelper::buildRotateY(0.5f);
		glm::mat4 mvp = perspective * view * pyramid.model ;
		glUniform3f(glGetUniformLocation(pyramid.shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(pyramid.shaderProgram, "lightPos"), 1, &lightPos[0]);
		pyramid.setUniformMatrix4fv(&pyramid.model, "model");
		pyramid.setUniformMatrix4fv(&mvp, "mvp");
		pyramid.DrawArray(vertex_position.size() /3);

		lightPos.x = 5* glm::sin(glm::radians(theta));
		lightPos.y = 1 * glm::sin(glm::radians(theta*1.2f));
		lightPos.z = 5* glm::cos(glm::radians(theta));
		theta += 1.0f;


		light.model = MatrixHelper::buildTranslate(lightPos.x, lightPos.y, lightPos.z) * MatrixHelper::buildScale(.2f, .2f, .2f);
		mvp = perspective * view * light.model;
		light.setUniformMatrix4fv(&mvp, "mvp");
		light.DrawArray();

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError(); 
	}

	pyramid.cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}