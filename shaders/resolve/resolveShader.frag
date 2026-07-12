//specify version
#version 450

/************/
/*INPUT DATA*/
/************/

//hdr image sampler
layout(binding = 0) uniform sampler2D hdrImage;

//might need a resolution push constant so we sample the hdrImage correctly

/*************/
/*OUTPUT DATA*/
/*************/

layout(location = 0) out vec4 outColor;

/***************************/
/*FRAGMENT SHADER CONSTANTS*/
/***************************/

/******************/
/*HELPER FUNCTIONS*/
/******************/

//tone mapping function ACES
vec3 ACES(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

/************/
/*BEGIN MAIN*/
/************/
void main() {
    //calculate uv from pixel coordinates
    vec2 uv = gl_FragCoord.xy / vec2(textureSize(hdrImage, 0));
    
    //sample HDR color
    vec3 hdr = texture(hdrImage, uv).rgb;

    //simple Reinhard tone mapping
    vec3 mapped = ACES(hdr);

    //gamma correction ->do not need this since our final format is SRGB
    //mapped = pow(mapped, vec3(1.0 / 2.2));

    //final output color to our swapChain
    outColor = vec4(mapped, 1.0);
}


