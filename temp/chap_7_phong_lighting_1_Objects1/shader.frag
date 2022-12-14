#version 430 core
in vec4 color;
in vec2 ftc; 
in vec3 norm;
in vec3 fragPos;

layout(binding=0) uniform sampler2D samp;
layout(binding=1) uniform sampler2D samp3;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 view;

uniform int decide;

out vec4 FragColor;
void main()
{
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(  lightPos  - fragPos  ); 
	float diff = max (dot(normal , lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
    vec3 viewDir = normalize(view - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;

	if(decide == 1)	FragColor =  vec4((ambient + diffuse + specular), 1.0) * texture(samp, ftc) ;
	else FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(samp3, ftc);
}