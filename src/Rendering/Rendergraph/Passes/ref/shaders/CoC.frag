
#version 450 core
layout (location = 0) out float FragColor;

in vec2 OutUV;


uniform sampler2D depth_Texture;
uniform float FocalLength;
uniform float FocalRange;
uniform float near;
uniform float far;
uniform float BokehRadius;


float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}



void main()
{  
    float depth = LinearizeDepth(texture2D(depth_Texture, OutUV).r);

    float coc = (depth - FocalLength) / FocalRange;
   	coc = clamp(coc, -1, 1) * BokehRadius;
   
    FragColor = coc;

}