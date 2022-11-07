#include "mesh.h"
#include "Shader.h"
#include <SOIL2/SOIL2.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>


mesh::mesh(const char* vertexShaderPath, const char* fragmentShaderPath) {
	Shader shaderObject(vertexShaderPath, fragmentShaderPath);
	this->shaderProgram = shaderObject.shaderProgram;

	this->vaoID;
	glGenVertexArrays(1, &this->vaoID);
	glBindVertexArray(this->vaoID);

	model = glm::mat4(1.0f);
}


GLuint loadTexture1(const char* texImagePath) {
	GLuint textureID = SOIL_load_OGL_texture(texImagePath,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0) std::cout << "could not find texture file" << texImagePath << std::endl;
	return textureID;
}

void mesh::loadTexture(const char* texturePath, GLuint TextLoc) {
	glUseProgram(this->shaderProgram);

	glActiveTexture(TextLoc);
	GLuint bricktext = loadTexture1(texturePath);
	glBindTexture(GL_TEXTURE_2D, bricktext);
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

void mesh::loadOBJ(const char* objFilePath) {
		float x, y, z;
		std::string content;
		std::ifstream fileStream(objFilePath, std::ios::in);
		std::string line = "";

		std::vector<GLfloat> vertVals;
		std::vector<GLfloat> normVals;
		std::vector<GLfloat> stVals;
		std::vector<GLfloat> triangleVerts;

		std::cout << "loading obj" << std::endl;

		while (!fileStream.eof()) {
			getline(fileStream, line);
			if (line.compare(0, 2, "v ") == 0) { // vertex position ("v" case)
				std::stringstream ss(line.erase(0, 1));
				ss >> x; ss >> y; ss >> z; // extract the vertex position values
				vertVals.push_back(x);
				vertVals.push_back(y);
				vertVals.push_back(z);
			}
				if (line.compare(0, 2, "vt") == 0) { // texture coordinates ("vt" case)
					std::stringstream ss(line.erase(0, 2));
					ss >> x; ss >> y; // extract texture coordinate values
					stVals.push_back(x);
					stVals.push_back(y);
				}
			if (line.compare(0, 2, "vn") == 0) { // vertex normals ("vn" case)
				std::stringstream ss(line.erase(0, 2));
				ss >> x; ss >> y; ss >> z; // extract the normal vector values
				normVals.push_back(x);
				normVals.push_back(y);
				normVals.push_back(z);
			}
			if (line.compare(0, 2, "f ") == 0) { // triangle faces ("f" case)
				std::string oneCorner, v, t, n;
				std::stringstream ss(line.erase(0, 2));
				for (int i = 0; i < 3; i++) {
					getline(ss, oneCorner, ' '); // extract triangle face references
					std::stringstream oneCornerSS(oneCorner);
					getline(oneCornerSS, v, '/');
					getline(oneCornerSS, t, '/');
					getline(oneCornerSS, n, '/');
					int vertRef = (stoi(v) - 1) * 3; // "stoi" converts string to integer
					int tcRef = (stoi(t) - 1) * 2;
					int normRef = (stoi(n) - 1) * 3;
					verticies.push_back(vertVals[vertRef]); // build vector of vertices
					verticies.push_back(vertVals[vertRef + 1]);
					verticies.push_back(vertVals[vertRef + 2]);
					textureCoords.push_back(stVals[tcRef]); // build vector of texture coords
					textureCoords.push_back(stVals[tcRef + 1]);
					normals.push_back(normVals[normRef]); //... and normals
					normals.push_back(normVals[normRef + 1]);
					normals.push_back(normVals[normRef + 2]);
				}
				numTriangles += 1;
			}
		}

		for (int i = 0; i < verticies.size(); i+=3) {
			std::cout << verticies[i] << ", " << verticies[i + 1] << ", " << verticies[i + 2] << ",\n";
		}

		std::cout << "finished loading obj" << std::endl;
		loadVertexPositions(&this->verticies, "aPos");
		loadTextCoords(&this->textureCoords, "aTc");
		loadSurfaceNormals(&this->normals, "aNorm");
	}

void mesh::loadVertexPositions(std::vector<GLfloat>* vertices, const char * vertexAttributeName) {

	GLuint vbo = NULL;
	glGenBuffers(1, &vbo);
	vboIDs.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), &(*vertices)[0], GL_STATIC_DRAW);

	glBindVertexArray(this->vaoID);

	GLuint posLoc = glGetAttribLocation(this->shaderProgram, vertexAttributeName);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(posLoc);
}

void mesh::loadTextCoords(std::vector<GLfloat>* vertices, const char* vertexAttributeName) {
	GLuint vbo = NULL;
	glGenBuffers(1, &vbo);
	vboIDs.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), &(*vertices)[0], GL_STATIC_DRAW);

	glBindVertexArray(this->vaoID);


	GLuint textcoordLoc = glGetAttribLocation(this->shaderProgram, vertexAttributeName);
	glVertexAttribPointer(textcoordLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(textcoordLoc);
}

void mesh::loadSurfaceNormals(std::vector<GLfloat>* vertices, const char* vertexAttributeName) {
	GLuint vbo = NULL;
	glGenBuffers(1, &vbo);
	vboIDs.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), &(*vertices)[0], GL_STATIC_DRAW);

	glBindVertexArray(this->vaoID);


	GLuint normalLoc = glGetAttribLocation(this->shaderProgram, vertexAttributeName);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(normalLoc);
}

void mesh::loadVertexColors(std::vector<GLfloat>* vertices, const char* vertexAttributeName) {
	GLuint vbo = NULL;
	glGenBuffers(1, &vbo);
	vboIDs.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), &(*vertices)[0], GL_STATIC_DRAW);

	glBindVertexArray(this->vaoID);


	GLuint colorLoc = glGetAttribLocation(this->shaderProgram, vertexAttributeName);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(colorLoc);
}

void mesh::setUniformMatrix4fv(glm::mat4 * matrix, const char * shaderAttribName) {
	glUseProgram(this->shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, shaderAttribName), 1, GL_FALSE, glm::value_ptr(*matrix));
}

void mesh::setUniform1i(int i, const char* shaderAttribName) {
	glUseProgram(this->shaderProgram);
	glUniform1i(glGetUniformLocation(this->shaderProgram, shaderAttribName), i);
}

void mesh::DrawArray(int size) {
	glUseProgram(this->shaderProgram);
	glBindVertexArray(this->vaoID);

	glDrawArrays(GL_TRIANGLES, 0, size);
}

void mesh::DrawArray() {
	glUseProgram(this->shaderProgram);
	glBindVertexArray(this->vaoID);

	glDrawArrays(GL_TRIANGLES, 0, numTriangles*3);
}

void mesh::DrawElements(std::vector<GLuint>* index_data) {
	glUseProgram(this->shaderProgram);
	glBindVertexArray(this->vaoID);

	glDrawElements(GL_TRIANGLES, index_data->size() * sizeof(GLuint), GL_UNSIGNED_INT, &(*index_data)[0]);
}

void mesh::DrawElements() {
	glUseProgram(this->shaderProgram);
	glBindVertexArray(this->vaoID);

	glDrawElements(GL_TRIANGLES, indicies.size() * sizeof(GLuint), GL_UNSIGNED_INT, &(indicies)[0]);
}

void mesh::setIndicies(std::vector<GLuint>* indicies) {
	this->indicies = (*indicies);
}

void mesh::cleanup() {

	glDeleteProgram(this->shaderProgram);

}