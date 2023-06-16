#version 450 core


struct FrameData
{
    mat4 Projection;
    mat4 View;
    vec4 Position;
};

struct Line
{
   vec4 Point;
   vec4 Color;
};

layout(std140, binding = 0 ) uniform globalData
{
   FrameData frame;
};

layout(std140, binding = 9 ) uniform debuglines
{
   Line lines[1000];
};

out vec4 color;

void main()
{
   color = lines[gl_VertexID].Color;
   gl_Position =  frame.Projection * frame.View * lines[gl_VertexID].Point;
};