#version 430 core
in vec2 ftc;
layout(binding=0) uniform sampler2D samp1;
layout(binding=1) uniform sampler2D samp2;
layout(binding=2) uniform sampler2D samp3;
layout(binding=3) uniform sampler2D samp4;
layout(binding=4) uniform sampler2D samp5;



uniform int texture_dec;


out vec4 FragColor;
void main()
{
    if(texture_dec == 1) {
        FragColor = texture(samp1, ftc);
    }
    else if (texture_dec == 2) {
            FragColor = texture(samp2, ftc);
    } else if (texture_dec == 3) {
            FragColor = texture(samp3, ftc);
    } else if (texture_dec == 4) {
            FragColor = texture(samp4, ftc);
    } else {
            FragColor = texture(samp5, ftc);

    }
}