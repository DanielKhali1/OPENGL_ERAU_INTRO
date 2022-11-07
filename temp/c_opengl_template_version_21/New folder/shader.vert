#version 430 core

layout (location = 1) in vec3 aPos;
layout (location = 2) in vec3 aNorm;

uniform mat4 mvp;
uniform mat4 model;


out vec3 norm;
out vec3 fragPos;

void main()
{
   fragPos = vec3(model * vec4(aPos, 1.0));
   norm = mat3(transpose(inverse(model))) * aNorm;

   gl_Position = mvp * vec4(aPos, 1.0) ;
}