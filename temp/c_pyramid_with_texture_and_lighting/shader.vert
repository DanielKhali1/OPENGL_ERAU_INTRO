#version 430 core

layout (location = 1) in vec3 aPos;
layout (location = 2) in vec2 tc;

uniform mat4 mvp;

out vec2 ftc;
out vec4 color;
out flat int instanceID;

uniform vec3 offsets[512];

void main()
{
	instanceID = gl_InstanceID;
   vec3 offset = offsets[gl_InstanceID];
   gl_Position = mvp * vec4(aPos+ offset, 1.0) ;
   ftc = tc;
}