#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

struct FrameData
{
    mat4 Projection;
    mat4 View;
    vec4 Position;
};

layout(std140 , binding = 0) uniform globalData
{
   FrameData frame;
};

out vec2 TexCoords;

uniform vec3 CameraRight;
uniform vec3 CameraUp;
uniform vec3 BillboardPos;
uniform vec2 BillboardSize;

void main()
{
	vec3 particleCenter_wordspace = BillboardPos;

	vec3 vertexPositionWorldSpace = particleCenter_wordspace
		+ CameraRight * aPos.x * BillboardSize.x
		+ CameraUp * -aPos.z * BillboardSize.y;
	
	TexCoords = aTex;

	gl_Position = frame.Projection * frame.View * vec4(vertexPositionWorldSpace, 1.0f);


}
