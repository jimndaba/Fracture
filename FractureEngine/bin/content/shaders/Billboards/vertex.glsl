
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

layout (std140, binding = 0) uniform FRAME_DATA
{
    mat4 proj_matrix;
    mat4 view_matrix;    
    mat4 lightSpace_Matrix;
    vec3 view_Position;
};

out vec2 TexCoords;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform vec3 BillboardPos;
uniform vec2 BillboardSize;

uniform mat4 model;



void main()
{
	vec3 billboardcenter = BillboardPos;

	vec3 vertexPositionWorldSpace = CameraRight_worldspace  + CameraUp_worldspace;
	vec3 FragPos = vec3(model * vec4(aPos, 1.0)) ;




	TexCoords = aTexCoords;
	gl_Position =  proj_matrix * view_matrix * vec4(FragPos, 1.0f);

}
