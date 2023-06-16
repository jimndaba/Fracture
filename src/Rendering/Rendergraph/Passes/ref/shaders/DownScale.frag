
#version 450 core

// This shader performs downsampling on a texture,
// as taken from Call Of Duty method, presented at ACM Siggraph 2014.
// This particular method was customly designed to eliminate
// "pulsating artifacts and temporal stability issues".

// Remember to add bilinear minification filter for this texture!
// Remember to use a floating-point texture format (for HDR)!
// Remember to use edge clamping for this texture!
uniform sampler2D srcTexture;
uniform sampler2D emissionTexture;

uniform float EmissionStrength;
uniform vec2 srcResolution;
uniform int mipLevel;

in vec2 OutUV;
layout (location = 0) out vec4 FragColor;

vec3 PowVec3(vec3 v, float p)
{
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

const float invGamma = 1.0 / 2.2;
vec3 ToSRGB(vec3 v) { return PowVec3(v, invGamma); }

float RGBToLuminance(vec3 col)
{
    return dot(col, vec3(0.2126f, 0.7152f, 0.0722f));
}

float KarisAverage(vec3 col)
{
    // Formula is 1 / (1 + luma)
    float luma = RGBToLuminance(ToSRGB(col)) * 0.25f;
    return 1.0f / (1.0f + luma);
}

vec4 DownScale(sampler2D src, float x, float y)
{
    vec4 color = vec4(0);
    vec4 a = texture(src, vec2(OutUV.x - 2*x, OutUV.y + 2*y));
    vec4 b = texture(src, vec2(OutUV.x,       OutUV.y + 2*y));
    vec4 c = texture(src, vec2(OutUV.x + 2*x, OutUV.y + 2*y));

    vec4 d = texture(src, vec2(OutUV.x - 2*x, OutUV.y));
    vec4 e = texture(src, vec2(OutUV.x,       OutUV.y));
    vec4 f = texture(src, vec2(OutUV.x + 2*x, OutUV.y));

    vec4 g = texture(src, vec2(OutUV.x - 2*x, OutUV.y - 2*y));
    vec4 h = texture(src, vec2(OutUV.x,       OutUV.y - 2*y));
    vec4 i = texture(src, vec2(OutUV.x + 2*x, OutUV.y - 2*y));

    vec4 j = texture(src, vec2(OutUV.x - x, OutUV.y + y));
    vec4 k = texture(src, vec2(OutUV.x + x, OutUV.y + y));
    vec4 l = texture(src, vec2(OutUV.x - x, OutUV.y - y));
    vec4 m = texture(src, vec2(OutUV.x + x, OutUV.y - y));

    // Apply weighted distribution:
    // 0.5 + 0.125 + 0.125 + 0.125 + 0.125 = 1
    // a,b,d,e * 0.125
    // b,c,e,f * 0.125
    // d,e,g,h * 0.125
    // e,f,h,i * 0.125
    // j,k,l,m * 0.5
    // This shows 5 square areas that are being sampled. But some of them overlap,
    // so to have an energy preserving downsample we need to make some adjustments.
    // The weights are the distributed, so that the sum of j,k,l,m (e.g.)
    // contribute 0.5 to the final color output. The code below is written
    // to effectively yield this sum. We get:
    // 0.125*5 + 0.03125*4 + 0.0625*4 = 1

    vec4 groups[5];
    switch (mipLevel)
    {
    case 0:
        // We are writing to mip 0, so we need to apply Karis average to each block
        // of 4 samples to prevent fireflies (very bright subpixels, leads to pulsating
        // artifacts).
        groups[0] = (a+b+d+e) * (0.125f/4.0f);
        groups[1] = (b+c+e+f) * (0.125f/4.0f);
        groups[2] = (d+e+g+h) * (0.125f/4.0f);
        groups[3] = (e+f+h+i) * (0.125f/4.0f);
        groups[4] = (j+k+l+m) * (0.5f/4.0f);
        groups[0] *= KarisAverage(groups[0].xyz);
        groups[1] *= KarisAverage(groups[1].xyz);
        groups[2] *= KarisAverage(groups[2].xyz);
        groups[3] *= KarisAverage(groups[3].xyz);
        groups[4] *= KarisAverage(groups[4].xyz);
        color = groups[0]+groups[1]+groups[2]+groups[3]+groups[4];
        color = max(color, 0.0001f);
        break;
    default:
        color = e*0.125;
        color += (a+c+g+i)*0.03125;
        color += (b+d+f+h)*0.0625;
        color += (j+k+l+m)*0.125;
        break;
    }  
    return color;
}

void main()
{
    vec2 srcTexelSize = 1.0 / srcResolution;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    vec4 bloom = DownScale(srcTexture,x,y);   
    vec4 emission = DownScale(emissionTexture,x,y) * EmissionStrength;
    FragColor = bloom + emission;
   
}