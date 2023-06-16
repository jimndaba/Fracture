
#version 450 core
layout (location = 0) out vec4 FragColor;

in vec2 OutUV;

uniform sampler2D main_Texture;
uniform sampler2D coc_Texture;
uniform float BokehRadius;

// From https://github.com/Unity-Technologies/PostProcessing/
				// blob/v2/PostProcessing/Shaders/Builtins/DiskKernels.hlsl
const int kernelSampleCount = 16;
const vec2 kernel[kernelSampleCount] = {
    vec2(0, 0),
    vec2(0.54545456, 0),
    vec2(0.16855472, 0.5187581),
    vec2(-0.44128203, 0.3206101),
    vec2(-0.44128197, -0.3206102),
    vec2(0.1685548, -0.5187581),
    vec2(1, 0),
    vec2(0.809017, 0.58778524),
    vec2(0.30901697, 0.95105654),
    vec2(-0.30901703, 0.9510565),
    vec2(-0.80901706, 0.5877852),
    vec2(-1, 0),
    vec2(-0.80901694, -0.58778536),
    vec2(-0.30901664, -0.9510566),
    vec2(0.30901712, -0.9510565),
    vec2(0.80901694, -0.5877853),
};

float Weigh (float coc, float radius) {               
                return clamp((coc - radius + 2) / 2, 0.0, 1.0);
            }

void main()
{  
    vec2 texelSize = 1.0 / vec2(textureSize(main_Texture, 0));
    float weight = 0;
    float coc = texture(coc_Texture, OutUV).r;
    vec3 bgColor = vec3(0);
	float bgWeight = 0;

    vec3 fgColor = vec3(0);
	float fgWeight = 0;

    for (int k = 0; k < kernelSampleCount; k++) 
    {
        vec2 o = kernel[k] * BokehRadius;
        float radius =  length(o);
        o *= texelSize;


        float a = texture(coc_Texture, OutUV + o).r;
        vec4 s = texture(main_Texture, OutUV + o);  
        
        float bgw = Weigh(max(0,min(a,coc)), radius);
        bgColor += s.rgb * bgw;
        bgWeight += bgw; 

        float fgw = Weigh(-a, radius);
        fgColor += s.rgb * fgw;
        fgWeight += fgw;
	}   
	bgColor *= 1 / (bgWeight + (bgWeight == 0 ? 1 : 0));	
    fgColor *= 1 / (fgWeight + (fgWeight== 0 ? 1 : 0));		
    float bgfg = min(1, fgWeight * 3.14159265359 / kernelSampleCount); 
    vec3 color = mix(bgColor, fgColor, bgfg);	
    FragColor = vec4(color,bgfg);
}