#version 450 core

out vec2 OutUV;

void main()
{

OutUV = vec2((gl_VertexID << 1) & 2, gl_VertexID  & 2);
gl_Position = vec4(OutUV * 2.0f + -1.0f, 0.0f, 1.0f);

}