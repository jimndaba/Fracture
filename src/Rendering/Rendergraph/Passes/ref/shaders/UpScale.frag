#version 450 core

// This shader performs upsampling on a texture,
// as taken from Call Of Duty method, presented at ACM Siggraph 2014.

// Remember to add bilinear minification filter for this texture!
// Remember to use a floating-point texture format (for HDR)!
// Remember to use edge clamping for this texture!
uniform sampler2D srcTexture;
uniform float filterRadius;

in vec2 OutUV;
layout (location = 0) out vec4 upsample;

void main()
{
    // The filter kernel is applied with a radius, specified in texture
    // coordinates, so that the radius will vary across mip resolutions.
    float x = filterRadius;
    float y = filterRadius;

    // Take 9 samples around current texel:
    // a - b - c
    // d - e - f
    // g - h - i
    // === ('e' is the current texel) ===
    vec4 a = texture(srcTexture, vec2(OutUV.x - x, OutUV.y + y));
    vec4 b = texture(srcTexture, vec2(OutUV.x,     OutUV.y + y));
    vec4 c = texture(srcTexture, vec2(OutUV.x + x, OutUV.y + y));

    vec4 d = texture(srcTexture, vec2(OutUV.x - x, OutUV.y));
    vec4 e = texture(srcTexture, vec2(OutUV.x,     OutUV.y));
    vec4 f = texture(srcTexture, vec2(OutUV.x + x, OutUV.y));

    vec4 g = texture(srcTexture, vec2(OutUV.x - x, OutUV.y - y));
    vec4 h = texture(srcTexture, vec2(OutUV.x,     OutUV.y - y));
    vec4 i = texture(srcTexture, vec2(OutUV.x + x, OutUV.y - y));

    // Apply weighted distribution, by using a 3x3 tent filter:
    //  1   | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    upsample = e*4.0;
    upsample += (b+d+f+h)*2.0;
    upsample += (a+c+g+i);
    upsample *= 1.0 / 16.0;
}