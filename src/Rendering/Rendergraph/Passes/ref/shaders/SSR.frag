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
uniform sampler2D InMetalic; 
uniform sampler2D InColor;

vec3 reconstructNormals(vec2 uv, mat4 InvVP);
vec3 PositionFromDepth(vec2 uv, float depth);

vec3 BinarySearch(inout vec3 dir, vec3 pos,  inout vec3 hitCoord, inout float dDepth); 
vec4 RayMarch(vec3 dir, vec3 pos, inout vec3 hitCoord, out float dDepth);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

vec3 hash(vec3 a);

float Metallic;

void main()
{
  
float depth    = textureLod(InDepth, OutUV,0).x;
Metallic = texture(InMetalic, OutUV).a;
if(Metallic < 0.01)
       discard;

vec3 position = PositionFromDepth(OutUV,depth);
vec3 normal = vec3(texture(InMetalic, OutUV)*InvView);
// Reflection vector
vec3 reflected = normalize(reflect(normalize(position),normalize(normal)));

vec3 albedo = texture(InColor, OutUV).rgb;




vec3 F0 = vec3(0.04); 
F0  = mix(F0, albedo, Metallic);
vec3 Fresnel = fresnelSchlick(max(dot(normalize(normal),normalize(position)), 0.0), F0);

vec3 hitPos = position;
float dDepth;

vec3 wp = vec3(vec4(position, 1.0) *InvView);
vec3 jitt = mix(vec3(0.0), vec3(hash(wp)), Metallic);
vec4 coords = RayMarch((vec3(jitt) + reflected * max(params.minRayStep, -position.z)),position , hitPos, dDepth);

vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - coords.xy));

float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);

float ReflectionMultiplier = pow(Metallic, params.reflectionSpecularFalloffExponent) * 
                screenEdgefactor * 
                -reflected.z;

// Get color
vec3 SSR = textureLod(InColor, coords.xy,0).rgb * clamp(ReflectionMultiplier, 0.0, 0.9) * Fresnel;
FragColor = vec4(SSR,Metallic) * params.strength;
}


vec3 PositionFromDepth(vec2 uv, float depth) 
{
float z = depth * 2.0 - 1.0;
vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, z, 1.0);
vec4 viewSpacePosition = InvVP * clipSpacePosition;
viewSpacePosition /= viewSpacePosition.w;
return viewSpacePosition.xyz;
}

vec3 reconstructNormals(vec2 uv, mat4 InvVP)
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

return normalize(cross(P1 - P0,P2 - P0)) ;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 hash(vec3 a)
{
    a = fract(a * Scale);
    a += dot(a, a.yxz + K);
    return fract((a.xxy + a.yxx)*a.zyx);   
}


vec4 RayMarch(vec3 dir, vec3 pos, inout vec3 hitCoord, out float dDepth)
{

    dir *= params.step;
 
 
    float depth;
    int steps;
    vec4 projectedCoord;

 
    for(int i = 0; i < params.maxSteps; i++)
    {
        hitCoord += dir;
 
        projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = PositionFromDepth(projectedCoord.xy,textureLod(InDepth, projectedCoord.xy,1).x).z;
        if(depth > 1000.0)
            continue;
 
        dDepth = hitCoord.z - depth;

        if((dir.z - dDepth) < 1.2)
        {
            if(dDepth <= 0.0)
            {   
                vec4 Result;
                Result = vec4(BinarySearch(dir, pos,hitCoord, dDepth), 1.0);

                return Result;
            }
        }
        
        steps++;
    }
 
    
    return vec4(projectedCoord.xy, depth, 0.0);
}

vec3 BinarySearch(inout vec3 dir, vec3 pos, inout vec3 hitCoord, inout float dDepth)
{
    float depth;

    vec4 projectedCoord;
 
    for(int i = 0; i < params.numBinarySearchSteps; i++)
    {

        projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = PositionFromDepth(projectedCoord.xy,textureLod(InDepth, projectedCoord.xy,1).x).z;

 
        dDepth = hitCoord.z - depth;

        dir *= 0.5;
        if(dDepth > 0.0)
            hitCoord += dir;
        else
            hitCoord -= dir;    
    }

        projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
    return vec3(projectedCoord.xy, depth);
}