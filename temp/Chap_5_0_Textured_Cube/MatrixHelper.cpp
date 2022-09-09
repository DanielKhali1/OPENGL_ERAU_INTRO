#include "MatrixHelper.h"


glm::mat4 MatrixHelper::buildTranslate(float x, float y, float z)
{
    return glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     x,    y,    z,    1.0f);
}

glm::mat4 MatrixHelper::buildRotateY(float degrees)
{
    return glm::mat4(glm::cos(glm::radians(degrees)), 0.0f, glm::sin(glm::radians(degrees))  , 0.0f,
                     0.0f                               , 1.0f, 0.0f                                 , 0.0f,
                     -glm::sin(glm::radians(degrees)), 0.0f, glm::cos(glm::radians(degrees)) , 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 MatrixHelper::buildRotateX(float degrees)
{
    return glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, glm::cos(glm::radians(degrees)), -glm::sin(glm::radians(degrees)), 0.0f,
                     0.0f, glm::sin(glm::radians(degrees)),  glm::cos(glm::radians(degrees)), 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 MatrixHelper::buildRotateZ(float degrees)
{
    return glm::mat4(glm::cos(glm::radians(degrees)), -glm::sin(glm::radians(degrees)), 0.0f, 0.0f,
                     glm::sin(glm::radians(degrees)), glm::cos(glm::radians(degrees)), 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 MatrixHelper::buildScale(float x, float y, float z)
{
    return glm::mat4(  x , 0.0f, 0.0f, 0.0f,
                     0.0f,  y  , 0.0f, 0.0f,
                     0.0f, 0.0f,   z , 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);;
}
