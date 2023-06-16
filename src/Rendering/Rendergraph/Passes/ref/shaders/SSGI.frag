#version 450 core
out vec4 FragColor;


in vec2 OutUV;
in mat4 InvVP;
in mat4 InvView;
in mat4 projection;


struct SSRParams
{
  float step; 
  float minRayStep;
  float maxSteps;
  int numBinarySearchSteps;
  int reflectionSpecularFalloffExponent;
  float strength;
  vec2 _pad;
};

#define Scale vec3(.8, .8, .8)
#define K 19.19

layout(std140 , binding = 12) uniform ssrParams
{
   SSRParams params;
};

uniform sampler2D InDepth; 
uniform sampler2D InColor;
uniform sampler2D InNormal;

int SAMPLES = 16;
float indirectamount  = 500.8;
bool noise = false;
float noiseamount = 2.0;

vec3 reconstructNormals(vec2 uv);
vec3 PositionFromDepth(vec2 uv, float depth);
vec3 lightSample(sampler2D color_tex,float depth,vec2 coord, mat4 ipm, vec2 lightcoord, vec3 normal, vec3 position, float n, vec2 texsize);

float Metallic;

vec2 mod_dither3(vec2 u) {
	float noiseX = mod(u.x + u.y + mod(208. + u.x * 3.58, 13. + mod(u.y * 22.9, 9.)),7.) * .143;
	float noiseY = mod(u.y + u.x + mod(203. + u.y * 3.18, 12. + mod(u.x * 27.4, 8.)),6.) * .139;
	return vec2(noiseX, noiseY)*2.0-1.0;
}


vec2 dither(vec2 coord, float seed, vec2 size) {
	float noiseX = ((fract(1.0-(coord.x+seed*1.0)*(size.x/2.0))*0.25)+(fract((coord.y+seed*2.0)*(size.y/2.0))*0.75))*2.0-1.0;
	float noiseY = ((fract(1.0-(coord.x+seed*3.0)*(size.x/2.0))*0.75)+(fract((coord.y+seed*4.0)*(size.y/2.0))*0.25))*2.0-1.0;
    return vec2(noiseX, noiseY);
}

float lenSq(vec3 vector){
    return pow(vector.x, 2.0) + pow(vector.y, 2.0) + pow(vector.z, 2.0);
}


void main()
{
  
float depth    = texture(InDepth, OutUV).x;
vec3 direct = texture(InColor, OutUV).rgb;
vec3 color = normalize(direct).rgb;
vec3 indirect = vec3(0.0, 0.0, 0.0);
float PI = 3.14159;
vec2 texSize  = textureSize(InColor, 0).xy;   

vec3 position = PositionFromDepth(OutUV,depth);
//vec3 normal    = reconstructNormals(OutUV);
vec3 normal = normalize(vec3(texture(InNormal, OutUV)));

//sampling in spiral    
float dlong = PI*(3.0-sqrt(5.0));
float dz = 1.0/float(SAMPLES);
float mlong = 0.0;
float z = 1.0 - dz/2.0;

for(int i = 0; i < SAMPLES; i++){
            
        float r = sqrt(1.0-z);
        
        float xpoint = (cos(mlong)*r)*0.5+0.5;
        float ypoint = (sin(mlong)*r)*0.5+0.5;
                
        z = z - dz;
        mlong = mlong + dlong;
    
        indirect += lightSample(InColor, depth, OutUV, InvVP, vec2(xpoint, ypoint), normal, position, float(i), texSize); 

 }

//FragColor = vec4(indirect,1.0);
FragColor += vec4(direct * (indirect/float(SAMPLES)* indirectamount),1.0); //vec4(direct+(indirect/float(SAMPLES) ),1.0);
}


vec3 PositionFromDepth(vec2 uv, float depth) 
{
float z = depth * 2.0 - 1.0;

vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, z, 1.0);
vec4 viewSpacePosition = InvVP * clipSpacePosition;

viewSpacePosition /= viewSpacePosition.w;

return viewSpacePosition.xyz;
}

vec3 reconstructNormals(vec2 uv)
{  
vec2 depth_dimensions = textureSize(InDepth,0);
vec2 uv0 = uv; // center
vec2 uv1 = uv + vec2(1, 0) / depth_dimensions; // right 
vec2 uv2 = uv + vec2(0, 1) / depth_dimensions; // top

float depth0 = texture(InDepth, uv0).r;
float depth1 = texture(InDepth, uv1).r;
float depth2 = texture(InDepth, uv2).r;

vec3 P0 = PositionFromDepth(uv0,depth0);
vec3 P1 = PositionFromDepth(uv1,depth1);
vec3 P2 = PositionFromDepth(uv2,depth2);

return normalize(cross(P1 - P0,P2 - P0));
}

vec3 lightSample(sampler2D color_tex,float depth,vec2 coord, mat4 ipm, vec2 lightcoord, vec3 normal, vec3 position, float n, vec2 texsize)
{
    vec2 random = vec2(1.0);
	if (noise){
    	random = (mod_dither3((coord*texsize)+vec2(n*82.294,n*127.721)))*0.01*noiseamount;
	}else{
		random = dither(coord, 1.0, texsize)*0.1*noiseamount;
	}

    lightcoord *= vec2(0.7);

    //light absolute data
    vec3 lightcolor = textureLod(color_tex, ((lightcoord)+random),0.0).rgb;
    vec3 lightnormal   = normalize(vec3(texture(InNormal, fract(lightcoord)+random)));// reconstructNormals(fract(lightcoord)+random);
    vec3 lightposition = PositionFromDepth(fract(lightcoord)+random,depth);


    //light variable data
    vec3 lightpath = lightposition - position;
    vec3 lightdir  = normalize(lightpath);
    
    //falloff calculations
    float cosemit  = clamp(dot(lightdir, -lightnormal), 0.0, 1.0); //emit only in one direction
    float coscatch = clamp(dot(lightdir, normal)*0.5+0.5,  0.0, 1.0); //recieve light from one direction
    float distfall = pow(lenSq(lightpath), 0.1) + 1.0;        //fall off with distance
    
    return (lightcolor * cosemit * coscatch / distfall)*(length(lightposition)/20.0);
}