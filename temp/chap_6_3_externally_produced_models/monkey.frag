#version 430 core
in vec4 color;
in vec2 ftc; 

layout(binding=0) uniform sampler2D samp;
layout(binding=1) uniform sampler2D samp1;
layout(binding=2) uniform sampler2D samp2;

uniform int decide;

out vec4 FragColor;
void main()
{
	if(decide == 1)	FragColor = texture(samp, ftc);
	else if(decide == 2)	FragColor = texture(samp1, ftc);
	else FragColor = texture(samp2, ftc);
}