#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (std140, binding = 0) uniform FRAME_DATA
{
    mat4 proj_matrix;
    mat4 view_matrix;    
    mat4 lightSpace_Matrix;
    vec3 view_Position;
};

out VS_OUT {
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vs_out;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;


//uniform mat4 projection;
//uniform mat4 view;
uniform mat4 model;
uniform vec3 viewPos;
//uniform mat4 lightSpaceMatrix;


void main()
{
    

    TexCoords = aTexCoords;
    FragPos = vec3(model* vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal; 

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0))); // Gram-Schmidt Process  
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N,T);
    mat3 TBN = transpose(mat3(T, B, N));
    FragPosLightSpace = lightSpace_Matrix * vec4(FragPos, 1.0);
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentFragPos = TBN * FragPos;   

    gl_Position =  proj_matrix * view_matrix * vec4(FragPos, 1.0);
}