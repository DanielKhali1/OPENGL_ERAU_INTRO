#version 430 core

layout (location = 0) in vec4 fcolor;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec3 aNorm;

uniform mat4 mvp;
uniform mat4 model;

out vec3 FragPos;
out vec3 fNormal;
out vec4 color;

void main()
{
   FragPos = vec3(model * vec4(aPos, 1.0));
   color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
   fNormal = aNorm; 

   gl_Position = mvp * vec4(aPos, 1.0) ;
}