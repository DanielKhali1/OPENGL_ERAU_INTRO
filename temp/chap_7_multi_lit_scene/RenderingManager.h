#pragma once
#include <vector>
#include "Shader.h"
#include "mesh.h"
#include "Camera.h"
class RenderingManager
{

private:
	std::vector <GLuint> shadowTex;
	std::vector<GLuint> frameBuffers;
	std::vector<Shader> shaders;
	std::vector<mesh> meshes;
	std::vector<glm::vec3> pointLightPositions;
	Camera camera;
	float WIDTH, HEIGHT;

	void applyMVP();
	void defaultFrameBufferPass();
	void shadowFrameBufferPass();
	void generateShadowBuffers(GLFWwindow* window);
public:
	void addMesh(GLuint shaderIndex);
	void addShader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void draw(GLFWwindow* window);
	RenderingManager(float width, float height);
};

