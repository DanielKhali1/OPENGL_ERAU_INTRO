#include "RenderingManager.h"



RenderingManager::RenderingManager(float width, float height) {
	this->WIDTH = width;
	this->HEIGHT = height;
}

void RenderingManager::applyMVP()
{

}

void RenderingManager::defaultFrameBufferPass()
{

}

void RenderingManager::generateShadowBuffers()
{
	int screenX = 2048;
	int screenY = 2048;

	for (int i = 0; i < this->pointLightPositions.size(); i++) {

		frameBuffers.push_back(NULL);
		shadowTex.push_back(NULL);

		glGenFramebuffers(1, &frameBuffers[i]);

		glGenTextures(1, &shadowTex[i]);
		glBindTexture(GL_TEXTURE_2D, shadowTex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenX, screenY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RenderingManager::shadowFrameBufferPass()
{

}

void RenderingManager::addMesh(GLuint shaderIndex)
{
	mesh temp_mesh = mesh(shaderIndex);
	meshes.push_back(temp_mesh);
}

void RenderingManager::addShader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	Shader shaderObject(vertexShaderPath, fragmentShaderPath);
	this->shaders.push_back(shaderObject);
}

void display(GLFWwindow* window) {
	glClearColor(0.8, 0.8, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderingManager::draw(GLFWwindow* window)
{
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	display(window);

	lightPos.x = 5 * glm::sin(glm::radians(theta));
	lightPos.z = 5 * glm::cos(glm::radians(theta));
	theta += 0.1f;

	glViewport(0, 0, 2048, 2048);
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

