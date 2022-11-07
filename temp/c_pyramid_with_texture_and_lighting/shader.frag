#version 430 core
in vec4 color;
in vec2 ftc; 

layout(binding=0) uniform sampler2D samp1;
layout(binding=1) uniform sampler2D samp2;
layout(binding=2) uniform sampler2D samp3;
layout(binding=3) uniform sampler2D samp4;

uniform int n[512];

in flat int instanceID;
out vec4 FragColor;
void main()
{

	int x = n[instanceID];
	if ( x == 0)
		FragColor = texture(samp1, ftc);
	else if (x == 1)
		FragColor = texture(samp2, ftc);
	else if (x == 2)
		FragColor = texture(samp3, ftc);
	else if (x == 3)
		FragColor = texture(samp4, ftc);

}