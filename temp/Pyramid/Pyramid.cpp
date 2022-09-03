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


#define WIDTH 1280
#define HEIGHT 720

using namespace std;

glm::mat4 view = glm::mat4(1.0f);
glm::vec3 position;

glm::vec3 lightPos(-1, 0, 0.1f);

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

GLuint createLampShaderProgram() {

	int success;
	char infoLog[512];

	const char* vertexShaderSource = "lamp.vert";
	const char* fragmentShaderSource = "lamp.frag";
	Shader shaderObject(vertexShaderSource, fragmentShaderSource);

	return shaderObject.shaderProgram;
}

GLuint createMainShaderProgram() {

	int success;
	char infoLog[512];

	const char* vertexShaderSource = "Pyramid.vert";
	const char* fragmentShaderSource = "Pyramid.frag";
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

	//view = glm::translate(glm::mat4(1.0f), position);
}

void move_events() {
	float sped = 0.03f;
	if (mv[0]) { lightPos.x -= sped; }
	if (mv[1]) { lightPos.x += sped; }
	if (mv[2]) { lightPos.y += sped; }
	if (mv[3]) { lightPos.y -= sped; }
	if (mv[5]) { lightPos.z -= sped; }
	if (mv[4]) { lightPos.z += sped; }
}

void calculateSurfaceNormals(std::vector<GLfloat> * normals, GLfloat* vertex_data, int vert_size,  GLuint * index_data, int index_size  ) {
	
	for (int i = 0; i < vert_size; i += 3) {
		glm::vec3 p1 = glm::vec3(vertex_data[index_data[i] * 3],   vertex_data[index_data[i] * 3 + 1],     vertex_data[index_data[i] * 3 + 2]    );
		glm::vec3 p2 = glm::vec3(vertex_data[index_data[i+1] * 3], vertex_data[index_data[i + 1] * 3 + 1], vertex_data[index_data[i + 1] * 3 + 2]);
		glm::vec3 p3 = glm::vec3(vertex_data[index_data[i+2] * 3], vertex_data[index_data[i + 2] * 3 + 1], vertex_data[index_data[i + 2] * 3 + 2]);


		std::cout << "p1: " << vertex_data[index_data[i] * 3] << " " << vertex_data[index_data[i] * 3 + 1] << " " << vertex_data[index_data[i] * 3 + 2] << std::endl;
		std::cout << "p2: " << vertex_data[index_data[i + 1] * 3] << " " << vertex_data[index_data[i + 1] * 3 + 1] << " " << vertex_data[index_data[i + 1] * 3 + 2] << std::endl;
		std::cout << "p3: " << vertex_data[index_data[i + 2] * 3] << " " << vertex_data[index_data[i + 2] * 3 + 1] << " " << vertex_data[index_data[i + 2] * 3 + 2] << std::endl;

		glm::vec3 u = p2 - p1;
		glm::vec3 v = p3 - p1;

		std::cout << "u: " << u.x << " " << u.y << "  " << u.z << std::endl;
		std::cout << "v: " << v.x << " " << v.y << "  " << v.z << std::endl;


		glm::vec3 normal = glm::cross(u, v);
		normals->push_back(normal.x);
		normals->push_back(normal.y);
		normals->push_back(normal.z);
		std::cout << normal.x << " " << normal.y << "  " << normal.z << std::endl;

	}
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	/*float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	lightPos = glm::vec3(xpos*2.0f , ypos * 2.0f, 1.0f);*/
}


int main(void) {
	position = glm::vec3(0.0f, 0.0f, -5.0f);
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "KILL MYSELF", NULL, NULL);
	glfwSetWindowPos(window, 20, 20);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	GLfloat vertex_position[] = {0.0f, 0.5f, 0.0f,
							     0.0f, -0.5f, -0.70711f,
							     0.5f, -0.5f, 0.5f,
								 -0.5, -0.5f, 0.5f};

	GLfloat color_data[] = { 1.0f, 0.0f, 0.0f, 1.0f,
							 0.0f, 1.0f, 0.0f, 1.0f,
							 0.0f, 0.0f, 1.0f, 1.0f,
							 0.0f, 1.0f, 0.5f, 1.0f};

	GLuint indices[] = { 0, 3, 2,
						 3, 0, 1, 
						 1, 0, 2, // dont change this
						 2, 3, 1};

	std::vector<GLfloat> normals;
	calculateSurfaceNormals(&normals, vertex_position, 12, indices, 12);

	GLuint shaderProgram = createMainShaderProgram();
	GLuint lampShaderProgram = createLampShaderProgram();


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);

	GLuint VBO[3];
	glGenBuffers(3, VBO);
	// loading vertex data into a VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_position), vertex_position, GL_DYNAMIC_DRAW);

	// loading color data into a VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_DYNAMIC_DRAW);


	GLuint VAO[2];
	glGenVertexArrays(2, &VAO[0]);
	glBindVertexArray(VAO[0]);
	
	// setting vertex vbo pointer and enabling the attrib array
	GLuint positionLoc = glGetAttribLocation(shaderProgram, "aPos");
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	// setting color vbo pointer and enabling the attrib array
	GLuint colorLoc = glGetAttribLocation(shaderProgram, "fcolor");
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint normalLoc = glGetAttribLocation(shaderProgram, "aNorm");
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glBindVertexArray(VAO[1]);
	positionLoc = glGetAttribLocation(lampShaderProgram, "aPos");
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model1 = glm::mat4(1.0f);

	view =  glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	float big_rot = 1;
	model1 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	model1 = glm::rotate(model1, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());
		model1 = glm::rotate(model1, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

		//lightPos.x = 1*glm::cos(glm::radians(big_rot));
		//lightPos.z = 1*glm::sin(glm::radians(big_rot));

		//big_rot += .2f;
		glBindVertexArray(VAO[0]);
		glUseProgram(shaderProgram);

		//model1 = glm::rotate(model1, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::rotate(view, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 mvp = projection * view * model1;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model1));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
		glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, indices);


		glBindVertexArray(VAO[1]);
		glUseProgram(lampShaderProgram); 
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::translate(model, lightPos);
		//model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

		mvp = projection * view * model;
		glUniformMatrix4fv(glGetUniformLocation(lampShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, indices);

		glfwSwapBuffers(window);
		glfwPollEvents();
		move_events();
		glGetError();

	}

	glDeleteVertexArrays(1, &VAO[0]);
	glDeleteBuffers(3, VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}