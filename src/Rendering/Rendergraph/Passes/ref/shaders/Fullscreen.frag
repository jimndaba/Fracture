#version 450

out vec4 FragColor;

in vec2 OutUV;
uniform sampler2D aDiffuse;

void main()
{
        vec3 color = vec3(0.0);
        color += texture(aDiffuse,OutUV).rgb;
        
        FragColor = vec4(color,1);
}