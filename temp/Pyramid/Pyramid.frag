#version 430 core

out vec4 FragColor;

in vec3 fNormal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec4 color;

void main()
{
	vec3 norm = fNormal;
	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize( lightPos - FragPos   );  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * vec3(color.x, color.y, color.z);
	FragColor = vec4(result, 1.0);
}