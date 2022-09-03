#pragma once
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>>

class MatrixHelper
{
public:
	static glm::mat4 buildTranslate(float x, float y, float z);
	static glm::mat4 buildRotateY(float degrees);
	static glm::mat4 buildRotateX(float degrees);
	static glm::mat4 buildRotateZ(float degrees);
	static glm::mat4 buildScale(float x, float y, float z);

};

