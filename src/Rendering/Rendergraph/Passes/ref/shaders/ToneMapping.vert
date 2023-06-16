#version 450 core

out vec2 OutUV;
out mat4 InvVProj;
out mat4 projection;

struct FrameData
{
    mat4 Projection;
    mat4 View;
    vec4 CameraPosition;
    float DeltaTime;
    vec3 _pad;
};

layout(std140 , binding = 0) uniform globalData
{
   FrameData frame;
};


void main()
{
projection = frame.Projection;
InvVProj = inverse(frame.Projection);
OutUV = vec2((gl_VertexID << 1) & 2, gl_VertexID  & 2);
gl_Position = vec4(OutUV * 2.0f + -1.0f, 0.0f, 1.0f);

}