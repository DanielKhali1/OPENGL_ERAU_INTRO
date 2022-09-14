#version 430 core

layout (location = 1) in vec3 aPos;
uniform mat4 shadowMVP;

void main()
{
   gl_Position = shadowMVP * vec4(aPos, 1.0) ;
}