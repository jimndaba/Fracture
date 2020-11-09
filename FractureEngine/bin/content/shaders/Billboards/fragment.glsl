#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D IconTexture;

void main()
{
        FragColor = texture(IconTexture, TexCoords);
}
