#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

layout (std140, binding = 0) uniform FRAME_DATA
{
    mat4 proj_matrix;
    mat4 view_matrix;    
    mat4 lightSpace_Matrix;
    vec3 view_Position;
};

uniform mat4 model;


void main()
{
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;   
    FragPosLightSpace = lightSpace_Matrix * vec4(FragPos, 1.0);
    gl_Position =  proj_matrix *  view_matrix * vec4(FragPos, 1.0);
}