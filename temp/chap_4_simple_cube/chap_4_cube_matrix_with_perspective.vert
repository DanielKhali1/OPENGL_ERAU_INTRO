#version 430

layout (location = 1) in vec3 pos;
layout (location = 2) in vec4 color;

uniform mat4 mvp;

out vec4 fcolor;

void main()
{
   gl_Position = mvp * vec4(pos, 1.0) ;
   fcolor = color;
}