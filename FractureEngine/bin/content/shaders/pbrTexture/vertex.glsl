#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;


out VS_OUT {    
    vec3 T;
    vec3 B;
    vec3 N;
} vs_out;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal; 
    vs_out.T = normalize(mat3(model) * aTangent);
    vs_out.B = normalize(mat3(model) * aBitangent);
    vs_out.N = normalize(mat3(model) * aNormal);

    gl_Position =  projection * view * vec4(FragPos, 1.0);
}