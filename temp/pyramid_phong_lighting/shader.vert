#version 430 core

layout (location = 1) in vec3 aPos;
layout (location = 2) in vec2 aTc;
layout (location = 3) in vec3 aNorm;

uniform mat4 mvp;
uniform mat4 model;

out vec2 ftc;
out vec4 color;
out vec3 norm;
out vec3 fragPos;

void main()
{
   gl_Position = mvp * vec4(aPos, 1.0) ;
   ftc = aTc;

   fragPos = vec3(model * vec4(aPos, 1.0));
   norm = mat3(transpose(inverse(model))) * aNorm;

}