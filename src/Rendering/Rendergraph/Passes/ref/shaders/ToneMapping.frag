#version 450 core

out vec4 FragColor;

struct ToneNodeParams
{
    float Exposure;
    float Gamma;
    float Contrast;
		float Brightness;
    int ToneMapMode;
};

layout(std140 , binding = 10) uniform brightPassProps
{
   ToneNodeParams params;
};


uniform sampler2D InColorA;


in vec2 OutUV;

vec4 hdrTone2( vec4 hdrCol ){
  vec4 comp = ((hdrCol)*params.Exposure)/(vec4(1,1,1,1) + (hdrCol*params.Exposure));
  comp.a = 1.0;
  return comp;
}

vec3 uncharted2Tonemap(vec3 x) {
  x *=  params.Exposure;
	const float A = 0.15;
	const float B = 0.50;
	const float C = 0.10;
	const float D = 0.20;
	const float E = 0.02;
	const float F = 0.30;
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

vec3 tonemapUncharted2(vec3 color) {
  color *=  params.Exposure;
	const float W = 11.2;
	const float exposureBias = 2.0;
	vec3 curr = uncharted2Tonemap(exposureBias * color);
	vec3 whiteScale = 1.0 / uncharted2Tonemap(vec3(W));
	return curr * whiteScale;
}

// Based on Filmic Tonemapping Operators http://filmicgames.com/archives/75
vec3 tonemapFilmic(vec3 color) {
  color *= params.Exposure;
	vec3 x = max(vec3(0.0), color - 0.004) ;
	return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}

// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
vec3 acesFilm(vec3 x) {
    x *=  params.Exposure;
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d ) + e), 0.0, 1.0) ;
}

vec3 tonemapReinhard(vec3 color) {
	return vec3(1.0) - exp(-color * params.Exposure);
}

vec3 aces(vec3 x) {
  return x / (x + 0.155) * 1.019;
}

void main()
{     
    vec3 hdrColor = texture(InColorA, OutUV).rgb;      

    vec4 result = vec4(0);

    if(params.ToneMapMode == 0)
    {
      result = hdrTone2(vec4(hdrColor, 1.0));  
    }
    if(params.ToneMapMode == 1)
    {
      result = vec4(uncharted2Tonemap(hdrColor.rgb), 1.0);  
    }
    if(params.ToneMapMode == 2)
    {
      result = vec4(tonemapUncharted2(hdrColor.rgb), 1.0);   
    }
    if(params.ToneMapMode == 3)
    {
     result = vec4(tonemapFilmic(hdrColor.rgb), 1.0);    
    }
    if(params.ToneMapMode == 4)
    {
    result = vec4(acesFilm(hdrColor.rgb), 1.0) ; 
    //result = vec4(aces(hdrColor.rgb), 1.0)  ;
    }
    if(params.ToneMapMode == 5)
    {
     result = vec4(tonemapReinhard(hdrColor.rgb), 1.0);  
    }

    result.rgb = pow(result.rgb, vec3(1.0/params.Gamma));
    result.a = 1.0;

    vec3 corrected = (result.rgb - 0.5) * params.Contrast + 0.5;
    vec3 bright = corrected + vec3(params.Brightness,params.Brightness,params.Brightness);

    FragColor = vec4(bright,1.0);
}