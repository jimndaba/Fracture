#version 450 core

out vec4 FragColor;

in vec2 OutUV;
uniform sampler2D srcTexture;
uniform sampler2D bloomTexture;
uniform sampler2D DirtMask;

uniform float BloomStrength;
uniform float dirtMaskIntensity = 0.4f;

void main()
{
    vec4 hdrColor = texture(srcTexture, OutUV);
    vec4 bloomColor = texture(bloomTexture, OutUV);

    vec4 drt = texture(DirtMask, vec2(OutUV.x, 1.0 - OutUV.y)) * dirtMaskIntensity;

    bloomColor += bloomColor * drt;

    bloomColor *= BloomStrength;

    vec4 color = hdrColor + bloomColor;

    //vec4 color =  mix(hdrColor,bloomColor + bloomColor*drt , vec4(vec3(BloomStrength),1.0)); // linear interpolation

    FragColor = color ;
}