#version 450 core
out vec4 FragColor;
  
in vec2 OutUV;

uniform sampler2D srcTexture;
uniform float Inner   =  0.045;
uniform float Outer =  0.038;
uniform float Roundness =  0.7;
uniform float Smoothness =  0.1;
uniform bool Bypass;

float sdSquare(vec2 point, float width) {
	vec2 d = abs(point) - width;
	return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float vignette(vec2 uv, vec2 size, float roundness, float smoothness) {
	// Center UVs
	uv -= 0.5;

	// Shift UVs based on the larger of width or height
	float minWidth = min(size.x, size.y);
	uv.x = sign(uv.x) * clamp(abs(uv.x) - abs(minWidth - size.x), 0.0, 1.0);
	uv.y = sign(uv.y) * clamp(abs(uv.y) - abs(minWidth - size.y), 0.0, 1.0);

	// Signed distance calculation
	float boxSize = minWidth * (1.0 - roundness);
	float dist = sdSquare(uv, boxSize) - (minWidth * roundness);

	return 1.0 - smoothstep(0.0, smoothness, dist);
}

void main()
{     
    vec4 result = texture(srcTexture, OutUV);
   
    if(Bypass)
    {
        FragColor = result; 
        return;
    } 
    float vignetteValue = vignette(OutUV, vec2(Outer, Inner), Roundness, Smoothness);
    FragColor = texture2D(srcTexture,OutUV) * vignetteValue;
}