#version 450 core
out vec4 FragColor;


in vec2 OutUV;
in mat4 InvVProj;

uniform int BlurSize = 2;
uniform sampler2D InSSAO; 

void main()
{   
    vec2 texelSize = 1.0 / vec2(textureSize(InSSAO, 0));
    float result = 0.0;
    for (int x = -BlurSize; x < BlurSize; ++x) 
    {
        for (int y = -BlurSize; y < BlurSize; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(InSSAO, OutUV + offset).r;
        }
    }
    int n = BlurSize*2;
    FragColor = vec4(result / ( n *  n));

}
