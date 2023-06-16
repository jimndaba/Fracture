#version 450 core
layout (location = 0) in vec3 aPos;

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

uniform mat4 model;

void main()
{    
    gl_Position = frame.Projection * frame.View *  model * vec4(aPos, 1.0);
}