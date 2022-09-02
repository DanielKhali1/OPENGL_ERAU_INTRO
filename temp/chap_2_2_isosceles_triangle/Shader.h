#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Shader
{
	public:
		GLuint shaderProgram;
		Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	private:
		void checkStatus(unsigned int shaderIndex, char* infoLog, bool isLinking);
};

