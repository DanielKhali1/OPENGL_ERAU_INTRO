#include "mesh.h"
#include "Shader.h"
#include <SOIL2/SOIL2.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>


mesh::mesh(GLuint shaderProgram) {
	this->currentShaderProg = shaderProgram;
	shaderPrograms.push_back(this->currentShaderProg);

	this->vaoID;
	glGenVertexArrays(1, &this->vaoID);
	glBindVertexArray(this->vaoID);

	model = glm::mat4(1.0f);
}

void mesh::addShader(GLuint shaderProgramIndex) {
	this->shaderPrograms.push_back(shaderProgramIndex);
}

void mesh::swapShader(int index) {
	this->currentShaderProg = this->shaderPrograms[index];
}

void mesh::loadMaterialConstants(float ns, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks)
{
	glUniform3fv(glGetUniformLocation(currentShaderProg, "Ka"), 1, &ka[0]);
	glUniform3fv(glGetUniformLocation(currentShaderProg, "Kd"), 1, &kd[0]);
	glUniform3fv(glGetUniformLocation(currentShaderProg, "Ks"), 1, &ks[0]);
}

void mesh::makeSphere(int prec) {

	int numVertices = (prec + 1) * (prec + 1);
	int numIndices = prec * prec * 6;
	std::vector<GLfloat> textureCoords;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> verticies;
	std::vector<glm::vec3> vert;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> tex;
	std::vector<GLuint> ind;

	for (int i = 0; i < numVertices; i++) { vert.push_back(glm::vec3()); } // std::vector::push_back()
	for (int i = 0; i < numVertices; i++) { tex.push_back(glm::vec2()); } // inserts new element at
	for (int i = 0; i < numVertices; i++) { norm.push_back(glm::vec3()); } // the end of a vector and
	for (int i = 0; i < numIndices; i++) { ind.push_back(0); }
	// calculate triangle vertices
	for (int i = 0; i <= prec; i++) {
		for (int j = 0; j <= prec; j++) {
			GLfloat y = (GLfloat)cos(glm::radians(180.0f - i * 180.0f / prec));
			GLfloat x = -(GLfloat)cos(glm::radians(j * 360.0f / prec)) * (GLfloat)abs(cos(asin(y)));
			GLfloat z = (GLfloat)sin(glm::radians(j * 360.0f / prec)) * (GLfloat)abs(cos(asin(y)));
			(vert)[i * (prec + 1) + j] = glm::vec3(x, y, z);
			(tex)[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
			norm[i * (prec + 1) + j] = glm::vec3(x, y, z);
		}
	}
	// calculate triangle indices
	for (int i = 0; i < prec; i++) {
		for (int j = 0; j < prec; j++) {
			(ind)[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
			(ind)[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
			(ind)[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
			(ind)[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
			(ind)[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
			(ind)[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
		}
	}

	for (int i = 0; i < numIndices; i++) {
		verticies.push_back((vert[ind[i]]).x);
		verticies.push_back((vert[ind[i]]).y);
		verticies.push_back((vert[ind[i]]).z);
		textureCoords.push_back((tex[ind[i]]).s);
		textureCoords.push_back((tex[ind[i]]).t);
		normals.push_back((norm[ind[i]]).x);
		normals.push_back((norm[ind[i]]).y);
		normals.push_back((norm[ind[i]]).z);
	}
	numTriangles = numIndices/3;

	loadVertexPositions(&verticies, "aPos");
	loadTextCoords(&textureCoords, "aTc");
	loadSurfaceNormals(&normals, "aNorm");
}

GLuint loadTexture1(const char* texImagePath) {
	GLuint textureID = SOIL_load_OGL_texture(texImagePath,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0) std::cout << "could not find texture file" << texImagePath << std::endl;
	return textureID;
}

void mesh::loadTexture(const char* texturePath, GLuint TextLoc) {
	glUseProgram(this->currentShaderProg);

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

		std::vector<GLfloat> textureCoords;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> verticies;

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
		std::cout << "finished loading obj" << std::endl;

		loadVertexPositions(&verticies, "aPos");
		loadTextCoords(&textureCoords, "aTc");
		loadSurfaceNormals(&normals, "aNorm");
	}

void mesh::loadVertexPositions(std::vector<GLfloat>* vertices, const char * vertexAttributeName) {

	GLuint vbo = NULL;
	glGenBuffers(1, &vbo);
	vboIDs.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), &(*vertices)[0], GL_STATIC_DRAW);

	glBindVertexArray(this->vaoID);

	GLuint posLoc = glGetAttribLocation(this->currentShaderProg, vertexAttributeName);
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


	GLuint textcoordLoc = glGetAttribLocation(this->currentShaderProg, vertexAttributeName);
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


	GLuint normalLoc = glGetAttribLocation(this->currentShaderProg, vertexAttributeName);
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


	GLuint colorLoc = glGetAttribLocation(this->currentShaderProg, vertexAttributeName);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(colorLoc);
}

void mesh::setUniformMatrix4fv(glm::mat4 * matrix, const char * shaderAttribName) {
	glUseProgram(this->currentShaderProg);
	glUniformMatrix4fv(glGetUniformLocation(this->currentShaderProg, shaderAttribName), 1, GL_FALSE, glm::value_ptr(*matrix));
}

void mesh::setUniform1i(int i, const char* shaderAttribName) {
	glUseProgram(this->currentShaderProg);
	glUniform1i(glGetUniformLocation(this->currentShaderProg, shaderAttribName), i);
}

void mesh::DrawArray(int size) {
	glUseProgram(this->currentShaderProg);
	glBindVertexArray(this->vaoID);

	glDrawArrays(GL_TRIANGLES, 0, size);
}

void mesh::DrawArray() {
	glUseProgram(this->currentShaderProg);
	glBindVertexArray(this->vaoID);

	glDrawArrays(GL_TRIANGLES, 0, numTriangles*3);
}

void mesh::DrawElements(std::vector<GLuint>* index_data) {
	glUseProgram(this->currentShaderProg);
	glBindVertexArray(this->vaoID);

	glDrawElements(GL_TRIANGLES, index_data->size() * sizeof(GLuint), GL_UNSIGNED_INT, &(*index_data)[0]);
}

void mesh::DrawElements() {
	glUseProgram(this->currentShaderProg);
	glBindVertexArray(this->vaoID);

	glDrawElements(GL_TRIANGLES, indicies.size() * sizeof(GLuint), GL_UNSIGNED_INT, &(indicies)[0]);
}

void mesh::setIndicies(std::vector<GLuint>* indicies) {
	this->indicies = (*indicies);
}

void mesh::cleanup() {

	glDeleteProgram(this->currentShaderProg);

}