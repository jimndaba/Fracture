#version 450 core

out vec4 FragColor;

struct ToneNodeParams
{
    float Exposure;
    float Gamma;
    int ToneMapMode;
};

layout(std140 , binding = 8) uniform brightPassProps
{
   ToneNodeParams params;
};


uniform sampler2D InColorA;
uniform sampler2D InColorB;
uniform int Mode;

in vec2 OutUV;

vec3 blendAdd(vec3 base, vec3 blend) {
	return min(base+blend,vec3(1.0));
}

vec3 blendAdd(vec3 base, vec3 blend, float opacity) {
	return (blendAdd(base, blend) * opacity + base * (1.0 - opacity));
}


void main()
{     
    vec4 colora = texture(InColorA, OutUV);      
    vec4 colorb = texture(InColorB, OutUV);   

    vec4 result = colora;
  
    if(Mode == 0 )
    {
        result.rgb =   colorb.rgb ;//blendAdd(colora.rgb,colorb.rgb);       
    }
      if(Mode == 1)
    {
        result *= colorb;
    }
    FragColor =  result;
}