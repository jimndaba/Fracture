#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

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
uniform int _TilingFlag = 0;
uniform float Tiling = 1.0;

out vec3 Normal;
out vec3 FragPos;  
out vec3 viewPos;
out vec2 TexCoord;
out mat4 view;

void main()
{
    view = frame.View;
   
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;
    
    TexCoord = _TilingFlag == 1 ? vec2(aTexCoord.x / 2.0, aTexCoord.y / 2.0) * Tiling : aTexCoord; 

    FragPos = vec3(model * vec4(aPos, 1.0));
    viewPos = frame.Position.xyz;
    gl_Position = frame.Projection * frame.View *  model * vec4(aPos, 1.0);
}