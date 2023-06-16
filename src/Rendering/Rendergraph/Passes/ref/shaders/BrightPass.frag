
#version 450 core
layout (location = 0) out vec4 FragColor;

in vec2 OutUV;


uniform sampler2D srcTexture;
uniform float brightPassThreshold;

vec3 Prefilter (vec3 c) {
			float brightness = max(c.r, max(c.g, c.b));
			float contribution = max(0, brightness - brightPassThreshold);
			contribution /= max(brightness, 0.00001);
			return c * contribution;
		}

void main()
{
    vec3 c = Prefilter(texture2D(srcTexture, OutUV).rgb);    
    //float gamma = 2.2;
    //vec3 mapped = c / (c + vec3(1.0));
    //FragColor =vec4(pow(mapped, vec3(1.0/gamma)),1.0) ;
    FragColor =vec4(c,1.0);
}