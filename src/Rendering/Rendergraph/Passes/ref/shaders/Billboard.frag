#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D IconTexture;
uniform vec3 Color;

void main()
{
vec4 alphaTex = texture(IconTexture, TexCoords);
FragColor = vec4(alphaTex.rgb * Color,alphaTex.a);
        
}
