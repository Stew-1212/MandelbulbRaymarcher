//specify version
#version 450

/************/
/*INPUT DATA*/
/************/

//fullscreen triangle coords.
vec2 positions[3] = vec2[](
    vec2(-1.0, -1.0),
    vec2(-1.0, 3.0),
    vec2(3.0, -1.0)
);

/*************/
/*OUTPUT DATA*/
/*************/

/***************************/
/*FRAGMENT SHADER CONSTANTS*/
/***************************/

/******************/
/*HELPER FUNCTIONS*/
/******************/


/************/
/*BEGIN MAIN*/
/************/void main() {
    //create the fullscreen trianlge
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0); //use gl_VertexIndex b/c we are using Vulkan
}