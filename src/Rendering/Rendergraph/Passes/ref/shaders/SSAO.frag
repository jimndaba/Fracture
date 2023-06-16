#version 450 core
out vec4 FragColor;


in vec2 OutUV;
in mat4 InvVProj;
in mat4 projection;
in mat4 InvView;

struct SSAOParams
{
  float base; 
  float radius;
  float falloff;
  float area;
  int NoSamples;
  float strength;
  vec2 _pad;
};


layout(std140 , binding = 5) uniform Kernel
{
   vec4 samples[64];
};


layout(std140 , binding = 6) uniform ssaoParams
{
   SSAOParams params;
};

uniform sampler2D InDepth; 
uniform sampler2D InNoise; 

uniform vec2 Resolution;

float CalcViewZ(vec2 Coords);
vec3 reconstructPosition(vec2 uv, float z);
vec3 reconstructNormals(vec2 uv, float z);

void main()
{
float depth    = textureLod(InDepth, OutUV,0).r;
vec3 fragPos   = reconstructPosition(OutUV,depth);
vec3 normal    = vec3(vec4(reconstructNormals(OutUV,depth),1.0)*InvView);
vec2 noiseScale = vec2(Resolution.x / 4.0f, Resolution.y / 4.0);
vec3 randomVec = texture(InNoise, OutUV * noiseScale).xyz;  

vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
vec3 bitangent = cross(normal, tangent);
mat3 TBN       = mat3(tangent, bitangent, normal);  

float occlusion = 0.0;

float radius_depth = params.radius/depth;
vec3 position = vec3(OutUV, depth);

for(int i = 0; i < params.NoSamples; ++i)
{
    vec3 ray = radius_depth * reflect(samples[i].xyz, randomVec);
    vec3 hemi_ray = (position + sign(dot(ray,normal)) * ray);

    float occ_depth = textureLod(InDepth, clamp(hemi_ray.xy,0.0,1.0),0).r;
    float difference = depth - occ_depth;

  
    occlusion += step(params.falloff, difference) * (1.0-smoothstep(params.falloff, params.area, difference));
}

occlusion *=  params.strength;
float ao =  1.0  - occlusion * (1.0  / params.NoSamples);
float Output = clamp(ao + params.base,0.0,1.0);
//occlusion = 1.0 - (occlusion / params.NoSamples) * params.strength;
FragColor = vec4(Output) ;

}

float CalcViewZ(vec2 Coords)
{
    float Depth = texture(InDepth, Coords).x;
    float ViewZ = projection[3][2] / (2 * Depth -1 - projection[2][2]);
    return ViewZ;
}


vec3 reconstructPosition(vec2 uv, float depth)
{  
float z = depth * 2.0 - 1.0;
vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0,z, 1.0);
vec4 viewSpacePosition = InvVProj * clipSpacePosition;
viewSpacePosition /= viewSpacePosition.w;
return viewSpacePosition.xyz;
}

vec3 reconstructNormals(vec2 uv, float depth)
{
vec2 depth_dimensions = textureSize(InDepth,0);
vec2 uv0 = uv; // center
vec2 uv1 = uv + vec2(1, 0) / depth_dimensions; // right 
vec2 uv2 = uv + vec2(0, 1) / depth_dimensions; // top

float depth0 = textureLod(InDepth, uv0,0).r;
float depth1 = textureLod(InDepth, uv1,0).r;
float depth2 = textureLod(InDepth, uv2,0).r;


vec3 P0 = reconstructPosition(uv0, depth0);
vec3 P1 = reconstructPosition(uv1, depth1);
vec3 P2 = reconstructPosition(uv2, depth2);
return normalize(cross(P1 - P0,P2 - P0)) ;
}