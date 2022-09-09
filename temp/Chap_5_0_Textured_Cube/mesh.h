#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
class mesh
{

public:
	GLuint vaoID;
	GLuint shaderProgram;
	std::vector<GLuint> vboIDs;

	std::vector<GLuint> indicies;
	std::vector<GLfloat> textureCoords;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> verticies;


	mesh(const char * vertexShaderPath, const char * fragmentShaderPath);

	void loadTexture(const char * texturePath);

	void loadOBJ(const char * objFilePath);
	void loadVertexPositions(std::vector<GLfloat>* vertices, const char* vertexAttributeName);
	void loadTextCoords(std::vector<GLfloat>* vertices, const char* vertexAttributeName);
	void loadSurfaceNormals(std::vector<GLfloat>* vertices, const char* vertexAttributeName);
	void loadVertexColors(std::vector<GLfloat>* vertices, const char* vertexAttributeName);

	void setIndicies(std::vector<GLuint>* indicies);

	void setUniformMatrix4fv(glm::mat4* matrix, const char* shaderAttribName);

	void DrawElements(std::vector<GLuint>* index_data);
	void DrawElements();

	void DrawArray(int size);

	void cleanup();
	
private:

};

