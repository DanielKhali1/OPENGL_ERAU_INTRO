#version 430 core
in vec4 color;
in vec2 ftc; 
in vec3 norm;
in vec3 norm2;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform int decide;

out vec4 FragColor;
void main()
{
	vec3 normal = normalize(norm);
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;
	

	vec3 lightDir = normalize(fragPos   - lightPos  ); 
	float diff = max (dot(lightDir , normalize(norm2)), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.2;
    vec3 viewDir = normalize(vec3(0.0, 0.0, 0.0) - fragPos);
    vec3 reflectDir = reflect(lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular);
	vec4 result_color =  vec4(result, 1.0);

	if(decide == 1)	FragColor = vec4(0.8, 0.8, 0.8, 0.8) * result_color ;
	else 	FragColor = vec4(0.8, 0.8, 0.8, 0.8)* result_color ;
}