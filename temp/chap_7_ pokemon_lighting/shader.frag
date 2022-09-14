#version 430 core
in vec4 color;
in vec2 ftc; 
in vec3 norm;
in vec3 fragPos;

in vec4 shadow_coord;


layout(binding=0) uniform sampler2D samp1;
layout(binding=1) uniform sampler2D samp2;
layout(binding=2) uniform sampler2D samp3;
layout(binding=3) uniform sampler2D samp4;
layout(binding=4) uniform sampler2D samp5;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 view;
layout(binding=5) uniform sampler2DShadow shTex;



uniform int decide;

out vec4 FragColor;
void main()
{

	float notInShadow = textureProj(shTex, shadow_coord);

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

	// if in shadow
	if(notInShadow != 1.0) 
	{
		
		diffuse *= 0;
		specular *= 0;
	}

	if(decide == 1)	FragColor =  vec4((ambient + diffuse + specular), 1.0) * texture(samp1, ftc) ;
	else if(decide == 2) FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(samp2, ftc);
	else if(decide == 3) FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(samp3, ftc);
	else if(decide == 4) FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(samp4, ftc);
	else FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(samp5, ftc);

}