#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D IconTexture;

void main()
{
    
vec4 alphaTex = texture(IconTexture, TexCoords);
if (alphaTex.r == 0 &&alphaTex.g == 0 && alphaTex.b == 0)
{
    discard;
}
    

 FragColor = vec4(alphaTex);
        
}
