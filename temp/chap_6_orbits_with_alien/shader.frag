#version 430 core
in vec4 color;
in vec2 ftc; 

layout(binding=0) uniform sampler2D samp;
layout(binding=1) uniform sampler2D samp1;
layout(binding=2) uniform sampler2D samp2;
layout(binding=3) uniform sampler2D samp3;

uniform int decide;

out vec4 FragColor;
void main()
{
	if(decide == 1)	{ FragColor = mix( texture(samp, ftc), texture(samp1, ftc), 0.5); }
	else if(decide == 2)	FragColor = texture(samp2, ftc);
	else FragColor = texture(samp3, ftc) * 0.3 * vec4(1.0, 1.0, 1.0,  1.0);
}