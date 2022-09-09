#include "Shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <streambuf>
#include <iostream>


char* pullCstrFromFile(const char* source, std::string* str) {
	std::ifstream t(source);
	t.seekg(0, std::ios::end);
	(*str).reserve(t.tellg());
	t.seekg(0, std::ios::beg);
	(*str) = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	std::cout << str << "\n\n\n" << std::endl;
	return (char*)(*str).c_str();
}

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	std::string vert = "";
	std::string frag = "";

	const GLchar* vertProg = pullCstrFromFile(vertexShaderSource, &vert);
	const GLchar* fragProg = pullCstrFromFile(fragmentShaderSource, &frag);

	char infoLog[512];


	std::cout << "VERTEX SHADER" << std::endl;
	GLuint vertexShaderIndex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderIndex, 1, &vertProg, NULL);
	glCompileShader(vertexShaderIndex);
	checkStatus(vertexShaderIndex, infoLog, false);

	std::cout << "FRAGMENT SHADER" << std::endl;
	GLuint fragmentShaderIndex = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderIndex, 1, &fragProg, NULL);
	glCompileShader(fragmentShaderIndex);
	checkStatus(fragmentShaderIndex, infoLog, false);

	std::cout << "COMBINED PROGRAM SHADER" << std::endl;
	this->shaderProgram = glCreateProgram();
	glBindAttribLocation(this->shaderProgram, 0, "aPos");
	glAttachShader(this->shaderProgram, vertexShaderIndex);
	glAttachShader(this->shaderProgram, fragmentShaderIndex);
	glLinkProgram(this->shaderProgram);
	checkStatus(this->shaderProgram, infoLog, true);

	glDeleteProgram(vertexShaderIndex);
	glDeleteProgram(fragmentShaderIndex);
}

void Shader::checkStatus(unsigned int shaderIndex, char* infoLog, bool isLinking)
{
	int success = 1;
	glGetProgramiv(shaderIndex, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderIndex, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::COMPILATION\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "SUCCESS::PROGRAM::COMPILATION\n" << std::endl;
	}
	if (isLinking) {
		glGetProgramiv(shaderIndex, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderIndex, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINKING\n" << infoLog << std::endl;
		}
		else
		{
			std::cout << "SUCCESS::PROGRAM::LINKING\n" << std::endl;
		}
	}
	glGetError();

}
