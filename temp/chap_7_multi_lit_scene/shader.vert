#version 430 core

layout (location = 1) in vec3 aPos;
layout (location = 2) in vec2 aTc;
layout (location = 3) in vec3 aNorm;

uniform mat4 mvp;
uniform mat4 model;

uniform mat4 shadowMVP2;
layout(binding=5) uniform sampler2DShadow shTex;

out vec4 shadow_coord;
out vec2 ftc;
out vec3 norm;
out vec3 fragPos;

void main()
{
   fragPos = vec3(model * vec4(aPos, 1.0));
   norm = mat3(transpose(inverse(model))) * aNorm;
   ftc = aTc;

   shadow_coord = shadowMVP2 * vec4(aPos, 1.0);

   gl_Position = mvp * vec4(aPos, 1.0) ;



}