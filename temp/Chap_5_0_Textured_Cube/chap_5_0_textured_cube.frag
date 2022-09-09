#version 430 core
in vec4 color;
in vec2 ftc; 

layout(binding=0) uniform sampler2D samp;

out vec4 FragColor;
void main()
{
	FragColor = texture(samp, ftc);
}