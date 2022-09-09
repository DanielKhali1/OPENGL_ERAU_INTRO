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
#include<SOIL2/SOIL2.h>


#define WIDTH 470
#define HEIGHT 470

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

GLuint loadTexture(const char* texImagePath) {
	GLuint textureID = SOIL_load_OGL_texture(texImagePath,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0) std::cout << "could not find texture file" << texImagePath << std::endl;
	return textureID;
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

	int prec = 48;

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> colorData;

	int numVertices = (prec + 1) * (prec + 1);
	int numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); } 
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); } 
	// calculate triangle vertices
	for (int i = 0; i <= prec; i++) {
		for (int j = 0; j <= prec; j++) {
			float y = (GLfloat)cos(glm::radians(180.0f - i * 180.0f / prec));
			float x = -(GLfloat)cos(glm::radians(j * 360.0f / prec)) * (GLfloat)abs(cos(asin(y)));
			float z = (GLfloat)sin(glm::radians(j * 360.0f / prec)) * (GLfloat)abs(cos(asin(y)));
			vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
		}
	}
	// calculate triangle indices
	for (int i = 0; i < prec; i++) {
		for (int j = 0; j < prec; j++) {
			indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
			indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
			indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
			indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
			indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
			indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
		}
	}


	for (int i = 0; i < numIndices; i++) {
		colorData.push_back(0.0f);
		colorData.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		colorData.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		colorData.push_back(1.0f);
	}

	GLuint myTexture = loadTexture("image.jpg");

	GLuint shaderProgram = createMainShaderProgram();
	glEnable(GL_DEPTH_TEST);

	GLuint VBO[2];
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(GLfloat), &colorData[0], GL_STATIC_DRAW);

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

	//glm::mat4 view = MatrixHelper::buildTranslate(0.0f, 0.0f, -15.0f) * MatrixHelper::buildRotateX(-15.0f) * MatrixHelper::buildRotateY(20.0f);
	glm::mat4 view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::normalize(glm::vec3(0.0f, 2.0f, -3.0f) - glm::vec3(0.0f, 0.0f, 5.0f)), glm::vec3(0.0f, 3.0f, 0.0f));
	glm::mat4 perspective = glm::perspective(glm::radians(90.0f), (float)WIDTH/(float)HEIGHT, 0.01f, 100.0f);

	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());


		model1 *= MatrixHelper::buildRotateY(0.1f);
		glm::mat4 mvp = perspective * view * model1;
		glUseProgram(shaderProgram);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawElements(GL_TRIANGLES, indices.size() * (sizeof(GLuint)), GL_UNSIGNED_INT, &indices[0]);

		model2 *= MatrixHelper::buildTranslate(0.0f, 0.0f, 0.1f) * MatrixHelper::buildRotateY(1.0f);
		mvp = perspective * view * model2;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawElements(GL_TRIANGLES, indices.size() * (sizeof(GLuint)), GL_UNSIGNED_INT, &indices[0]);

		model3 *= MatrixHelper::buildTranslate(0.0f, -0.2f, 0.0) * MatrixHelper::buildRotateX(2.1f);
		mvp = perspective * view * model2 * model3;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawElements(GL_TRIANGLES, indices.size() * (sizeof(GLuint)), GL_UNSIGNED_INT, &indices[0]);

		model4 *= MatrixHelper::buildTranslate(0.0f, 0.0f, -0.1f) * MatrixHelper::buildRotateY(0.8f)  ;
		mvp = perspective * view *  model4;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawElements(GL_TRIANGLES, indices.size()*(sizeof(GLuint)), GL_UNSIGNED_INT, &indices[0]);
		//glDrawArrays(GL_TRIANGLES, 0, numIndices);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}

	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}