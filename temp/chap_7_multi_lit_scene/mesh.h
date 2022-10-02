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
	GLuint currentShaderProg;
	std::vector<GLuint> shaderPrograms;
	std::vector<GLuint> vboIDs;

	std::vector<GLuint> indicies;


	float Ns;
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 Ke;
	float Ni;

	glm::mat4 model;

	int numTriangles;


	mesh(GLuint shaderProgram);

	void addShader(GLuint shaderProgramIndex);

	void swapShader(int index);

	void makeSphere(int prec);
	void loadTexture(const char * texturePath, GLuint TextLoc);

	void loadMaterialConstants(float ns, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks);

	void loadOBJ(const char * objFilePath);
	void loadVertexPositions(std::vector<GLfloat>* vertices, const char* vertexAttributeName);
	void loadTextCoords(std::vector<GLfloat>* vertices, const char* vertexAttributeName);
	void loadSurfaceNormals(std::vector<GLfloat>* vertices, const char* vertexAttributeName);
	void loadVertexColors(std::vector<GLfloat>* vertices, const char* vertexAttributeName);

	void setIndicies(std::vector<GLuint>* indicies);

	void setUniformMatrix4fv(glm::mat4* matrix, const char* shaderAttribName);
	void setUniform1i(int i, const char* shaderAttribName);


	void DrawElements(std::vector<GLuint>* index_data);
	void DrawElements();

	void DrawArray(int size);
	void DrawArray();


	void cleanup();
	
private:

};

