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


#define WIDTH 1280
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

	const char* vertexShaderSource = "chap_5_orbitting_planets_textured1.vert";
	const char* fragmentShaderSource = "chap_5_orbitting_planets_textured1.frag";
	Shader shaderObject(vertexShaderSource, fragmentShaderSource);

	return shaderObject.shaderProgram;
}

GLuint loadTexture(const char* texImagePath) {
	GLuint textureID = SOIL_load_OGL_texture(texImagePath,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0) std::cout << "could not find texture file" << texImagePath << std::endl;
	return textureID;
}

void makeSphere(int prec, std::vector<glm::vec3> * vertices, std::vector<glm::vec2>* texcoords, std::vector<GLuint>* indices) {

	int numVertices = (prec + 1) * (prec + 1);
	int numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) { vertices->push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { texcoords->push_back(glm::vec2()); }
	for (int i = 0; i < numIndices; i++) { indices->push_back(0); }
	// calculate triangle vertices
	for (int i = 0; i <= prec; i++) {
		for (int j = 0; j <= prec; j++) {
			GLfloat y = (GLfloat)cos(glm::radians(180.0f - i * 180.0f / prec));
			GLfloat x = -(GLfloat)cos(glm::radians(j * 360.0f / prec)) * (GLfloat)abs(cos(asin(y)));
			GLfloat z = (GLfloat)sin(glm::radians(j * 360.0f / prec)) * (GLfloat)abs(cos(asin(y)));
			(*vertices)[i * (prec + 1) + j] = glm::vec3(x, y, z);
			(*texcoords)[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
		}
	}
	// calculate triangle indices
	for (int i = 0; i < prec; i++) {
		for (int j = 0; j < prec; j++) {
			(*indices)[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
			(*indices)[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
			(*indices)[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
			(*indices)[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
			(*indices)[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
			(*indices)[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
		}
	}
}


void texParams() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}
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

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texcoords;
	std::vector<GLuint> indices;
	int prec = 48;
	makeSphere(prec, &vertices, &texcoords, &indices);

	int numIndices = prec * prec * 6;


	std::vector<float> pvalues; // vertex positions
	std::vector<float> tvalues; // texture coordinate

	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vertices[indices[i]]).x);
		pvalues.push_back((vertices[indices[i]]).y);
		pvalues.push_back((vertices[indices[i]]).z);
		tvalues.push_back((texcoords[indices[i]]).s);
		tvalues.push_back((texcoords[indices[i]]).t);
	}

	std::cout << indices.size()  << std::endl;


	GLuint shaderProgram = createMainShaderProgram();
	glEnable(GL_DEPTH_TEST);

	GLuint VBO[2];
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	GLuint posLoc = glGetAttribLocation(shaderProgram, "aPos");
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(posLoc); 

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	GLuint textureLoc = glGetAttribLocation(shaderProgram, "tc");
	glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(textureLoc);


	//testure 1
	GLuint textures[4];
	glActiveTexture(GL_TEXTURE0);
	textures[0] = loadTexture("sun.jpg");
	texParams();
	glActiveTexture(GL_TEXTURE1);
	textures[1] = loadTexture("earth.jpg");
	texParams();
	glActiveTexture(GL_TEXTURE2);
	textures[2] = loadTexture("moon.jpg");
	texParams();
	glActiveTexture(GL_TEXTURE3);
	textures[3] = loadTexture("jupiter.jpg");
	texParams();
	glActiveTexture(GL_TEXTURE4);
	textures[3] = loadTexture("background.jpg");
	texParams();



	glm::mat4 model1 = glm::mat4(1.0f);
	glm::mat4 model2 = MatrixHelper::buildScale(0.8f, 0.8f, 0.8f) * MatrixHelper::buildTranslate(7.0f, 0.0f, 0.0f);
	glm::mat4 model3 = MatrixHelper::buildScale(0.5f, 0.5f, 0.5f) * MatrixHelper::buildTranslate(0.0f, 0.0f, -4.3f);
	glm::mat4 model4 = MatrixHelper::buildTranslate(-7.0f, 1.0f, 0.0f);

	glm::mat4 view = MatrixHelper::buildTranslate(0.0f, 0.0f, -8.0f) * MatrixHelper::buildRotateX(-15.0f) * MatrixHelper::buildRotateY(20.0f);
	glm::mat4 perspective = glm::perspective(glm::radians(90.0f), (float)WIDTH/(float)HEIGHT, 0.01f, 100.0f);

	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, WIDTH, HEIGHT);
		display(window, glfwGetTime());


		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		model1 *= MatrixHelper::buildRotateY(0.1f);
		glm::mat4 mvp = perspective * view * MatrixHelper::buildScale(1.5f, 1.5f, 1.5f) * model1;

		glUniform1i(glGetUniformLocation(shaderProgram, "texture_dec"), 1);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawArrays(GL_TRIANGLES,0, indices.size());


		glUniform1i(glGetUniformLocation(shaderProgram, "texture_dec"), 2);

		model2 *= MatrixHelper::buildTranslate(0.0f, 0.0f, 0.1f) * MatrixHelper::buildRotateY(1.0f);
		mvp = perspective * view * model2;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawArrays(GL_TRIANGLES, 0, indices.size());


		glUniform1i(glGetUniformLocation(shaderProgram, "texture_dec"), 3);
		model3 *= MatrixHelper::buildTranslate(0.0f, -0.2f, 0.0) * MatrixHelper::buildRotateX(2.1f);
		mvp = perspective * view * model2 * model3;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));
		glDrawArrays(GL_TRIANGLES, 0, indices.size());

		glUniform1i(glGetUniformLocation(shaderProgram, "texture_dec"), 4);
		model4 *= MatrixHelper::buildTranslate(0.0f, 0.0f, -0.1f) * MatrixHelper::buildRotateY(1.0f)  ;
		mvp = perspective * view *  model4;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1.0f, GL_FALSE, glm::value_ptr(mvp));


		glDrawArrays(GL_TRIANGLES, 0, indices.size());
		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError();

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(4, textures);
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(2, VBO);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}