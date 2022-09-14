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
#include "Camera.h"

float WIDTH = 1000;
float HEIGHT = 700;

using namespace std;

glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 4.0);

float deltaTime;

Camera camera;

bool lockcamerain = true;

GLuint shadowBuffer;
GLuint shadowTex;


glm::mat4 lightVmatrix;
glm::mat4 lightPmatrix;
glm::mat4 b;

void init(GLFWwindow* window) { 
	b = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);

}

void display(GLFWwindow* window) {
	glClearColor(0.8, 0.8, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	WIDTH = static_cast<float> (width);
	HEIGHT = static_cast<float> (height);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE) { if (action == GLFW_PRESS) { lockcamerain = !lockcamerain; } }
	
	if (!lockcamerain) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	else {

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	camera.cameraKeyMovement(window, key, scancode, action, mods);
}

void mouse_callback(GLFWwindow* window, double xposz, double yposz) {

	float xpos = static_cast<float>(xposz);
	float ypos = static_cast<float>(yposz);
	if (lockcamerain) {

		if (camera.firstMouse)
		{
			camera.lastX = xpos;
			camera.lastY = ypos;
			camera.firstMouse = false;
		}
		float xoffset = xpos - camera.lastX;
		float yoffset = camera.lastY - ypos; // reversed since y-coordinates go from bottom to top

		camera.lastX = xpos;
		camera.lastY = ypos;

		camera.mouse_callback(xoffset, yoffset);

		glfwSetCursorPos(window, WIDTH / 2.0f, HEIGHT / 2.0f);
		camera.lastX = WIDTH / 2.0f;
		camera.lastY = HEIGHT / 2.0f;
	}
}
float lastFrame;

void passTwo(mesh* charm, mesh* squirt, mesh* bulb, mesh* plane, mesh* light) {

	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	
	camera.move_events(deltaTime);
	camera.updateCameraVectors();
	glm::mat4 perspective = glm::perspective((float)glm::radians((float)HEIGHT * 0.1), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	
	std::vector<mesh> meshes;
	meshes.push_back(*squirt);
	meshes.push_back(*charm);
	meshes.push_back(*bulb);
	meshes.push_back(*plane);
	glm::mat4 mvp;
	glm::mat4 shadowMvp;
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].swapShader(0);
		meshes[i].setUniform1i(i+1, "decide");
		mvp = perspective * camera.getViewMatrix() * meshes[i].model;
		glUniform3f(glGetUniformLocation(meshes[i].currentShaderProg, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(meshes[i].currentShaderProg, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(meshes[i].currentShaderProg, "view"), 1, &camera.cameraPos[0]);

		b = glm::mat4(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f);
		shadowMvp = b * lightPmatrix * lightVmatrix * meshes[i].model;
		meshes[i].setUniformMatrix4fv(&shadowMvp, "shadowMVP2");
		meshes[i].setUniformMatrix4fv(&meshes[i].model, "model");
		meshes[i].setUniformMatrix4fv(&mvp, "mvp");
		meshes[i].DrawArray();
	}


	light->model = MatrixHelper::buildTranslate(lightPos.x, lightPos.y, lightPos.z) * MatrixHelper::buildScale(.2f, .2f, .2f);
	mvp = perspective * camera.getViewMatrix() * light->model;
	light->setUniformMatrix4fv(&mvp, "mvp");
	light->DrawArray();
}

void passOne(mesh * charm, mesh * squirt, mesh * bulb, mesh * plane, mesh * light) {

		//need to use new shaderprogram
		charm->swapShader(1);
		bulb->swapShader(1);
		squirt->swapShader(1);
		plane->swapShader(1);

		charm->model *= MatrixHelper::buildRotateY(1.0f);


		lightPmatrix = glm::perspective(glm::radians(HEIGHT * 0.1f), WIDTH/HEIGHT, 0.1f, 100.0f);
		lightVmatrix = glm::lookAt(lightPos, glm::vec3(0.0, 0.0, 0.0)- lightPos, glm::vec3(0.0, 1.0, 0.0)); // vector from light to origin

		glm::mat4 mvp = lightPmatrix * lightVmatrix * squirt->model;
		squirt->setUniformMatrix4fv(&mvp, "shadowMVP");
		squirt->DrawArray();

		mvp = lightPmatrix * lightVmatrix * charm->model;
		charm->setUniformMatrix4fv(&mvp, "shadowMVP");
		charm->DrawArray();

		mvp = lightPmatrix * lightVmatrix * bulb->model;
		bulb->setUniformMatrix4fv(&mvp, "shadowMVP");
		bulb->DrawArray();

		mvp = lightPmatrix * lightVmatrix * plane->model;
		plane->setUniformMatrix4fv(&mvp, "shadowMVP");
		plane->DrawArray();
	}

void setupShadowBuffer(GLFWwindow* window) {
	int screenX = 1024;
	int screenY = 1024;
		

	glGenFramebuffers(1, &shadowBuffer);

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenX*2, screenY * 2, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D model loading", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);

	camera = Camera::Camera(window);
	glfwSetCursorPos(window, WIDTH / 2.0, HEIGHT / 2.0);


	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);


	setupShadowBuffer(window);

	mesh squirt = mesh::mesh("shader.vert", "shader.frag");
	squirt.loadOBJ("squirt.obj");
	squirt.loadTexture("squirt.png", GL_TEXTURE0);
	squirt.addShader("shadow.vert", "shadow.frag");

	mesh charm = mesh::mesh("shader.vert", "shader.frag");
	charm.loadOBJ("char.obj");
	charm.loadTexture("char.png", GL_TEXTURE1);
	charm.addShader("shadow.vert", "shadow.frag");


	mesh bulb = mesh::mesh("shader.vert", "shader.frag");
	bulb.loadOBJ("bulb.obj");
	bulb.loadTexture("bulb.png", GL_TEXTURE2);
	bulb.addShader("shadow.vert", "shadow.frag");


	mesh plane = mesh::mesh("shader.vert", "shader.frag");
	plane.loadOBJ("plane.obj");
	plane.loadTexture("grass_texture.jpg", GL_TEXTURE3);
	plane.addShader("shadow.vert", "shadow.frag");


	mesh light = mesh::mesh("light.vert", "light.frag");
	light.makeSphere(48);

	camera.updateCameraVectors();
	glm::mat4 perspective = glm::perspective((float)glm::radians((float)HEIGHT*0.1), (float)WIDTH/(float)HEIGHT, 0.1f, 50.0f);
	glm::mat4 view = camera.getViewMatrix(); 

	squirt.setUniform1i(1, "decide");
	charm.setUniform1i(2, "decide");
	bulb.setUniform1i(3, "decide");
	plane.setUniform1i(4, "decide");


	squirt.model = MatrixHelper::buildTranslate(0.0f, 0.0f, 0.0f);
	charm.model = MatrixHelper::buildTranslate(2.0f, 0.0f, 0.0f);
	bulb.model = MatrixHelper::buildTranslate(-2.0f, 0.0f, 0.0f);
	plane.model = MatrixHelper::buildTranslate(0.0f, -1.0f, 0.0f) * MatrixHelper::buildScale(4.0f, 4.0f, 4.0f);


	bool left = false;
	lastFrame = static_cast<float>(glfwGetTime());
	float theta = 0.0;
	while (!glfwWindowShouldClose(window)) {
		glViewport(0, 0, WIDTH, HEIGHT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		display(window);

		lightPos.x = 5 * glm::sin(glm::radians(theta));
		lightPos.z = 5 * glm::cos(glm::radians(theta));
		theta += 0.1f;

		glViewport(0, 0, WIDTH * 2, HEIGHT * 2);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE5);
	    glBindTexture(GL_TEXTURE_2D, shadowTex);


		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(3.0f, 4.0f);
		passOne(&charm, &squirt, &bulb, &plane, &light);

		glDisable(GL_POLYGON_OFFSET_FILL);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);
		display(window);
		passTwo(&charm, &squirt, &bulb, &plane, &light);

		std::cout << glGetError() << std::endl;
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		glGetError(); 
	}

	squirt.cleanup();
	charm.cleanup();
	bulb.cleanup();
	plane.cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}