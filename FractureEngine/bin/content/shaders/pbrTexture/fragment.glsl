#version 330 core
out vec4 FragColor;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  sampler2D reflection;
  float shininess;
};

struct SunLight {
    bool enabled;
    vec3 direction;
    float intensity;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    bool enabled;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

struct SpotLight {
    bool enabled;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// material parameters
uniform vec3  u_albedo;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ao;

//material flags
uniform float albedoFlag;
uniform float normalFlag;
uniform float metallicFlag;
uniform float roughnessFlag;
uniform float aoFlag;
uniform float TransparencyFlag;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

//Shadows
uniform sampler2D shadowMap;
in vec4 FragPosLightSpace;

// lights
#define NR_SUN_LIGHTS 1
uniform SunLight sunLights[NR_SUN_LIGHTS];

#define NR_POINT_LIGHTS 10
uniform PointLight pointLights[NR_POINT_LIGHTS];

#define NR_SPOT_LIGHTS 10
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform vec3 viewPos;
uniform float intensity;

const float PI = 3.14159265359;
const float Epsilon = 0.00001;

// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = normalize(Normal);

    if(normalFlag > 0.5)
    {
        tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;
        vec3 Q1  = dFdx(FragPos);
        vec3 Q2  = dFdy(FragPos);
        vec2 st1 = dFdx(TexCoords);
        vec2 st2 = dFdy(TexCoords);

        vec3 N   = normalize(Normal);
        vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
        vec3 B  = -normalize(cross(N, T));
        mat3 TBN = mat3(T, B, N);
        tangentNormal = normalize(TBN * tangentNormal);
    }

    return tangentNormal;
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
   
    return nom / denom;
}  
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
} 
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
float ShadowFade = 0.8;

float GetShadowBias(SunLight light)
{
	const float MINIMUM_SHADOW_BIAS = 0.005;
	float bias = max(MINIMUM_SHADOW_BIAS * (1.0 - dot(Normal, light.direction)), MINIMUM_SHADOW_BIAS);
	return bias;
}

float ShadowCalculation(vec4 fragPosLightSpace,SunLight light)
{
// perform perspective divide
    const float NUM_SAMPLES = 8.0;
    const float SAMPLES_START = (NUM_SAMPLES- 1)/2.0;
    const float NUM_SAMPLES_SQ = NUM_SAMPLES*NUM_SAMPLES;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;   

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;   
    float shadow = 0.0;   

    float bias = GetShadowBias(light);
  
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0); //1.0 / 
    for(float x = -SAMPLES_START; x <= SAMPLES_START;x +=1.0)
    {
        for(float y = -SAMPLES_START; y <= SAMPLES_START;y +=1.0)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - GetShadowBias(light) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    
    if(projCoords.z > 1.0)
        shadow = 0.0;

	return  shadow/NUM_SAMPLES_SQ;

}


vec3 CalcDirLight(SunLight light,vec3 F0, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 alb,vec3 F0, vec3 normal, vec3 fragPos, vec3 viewDir,vec3 m_ambient);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcIBL(vec3 F0, vec3 normal, vec3 viewDir, vec3 ref);

vec3 albedo = vec3(0.0);
float metallic = 0.0;
float roughness = 0.0;
float ao = 0.0;
vec3 ambient =  vec3(0.0);


void main()
{		
    // material properties     
    vec3 tex = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));    
    float alpha = 1.0;  

    albedo = albedoFlag > 0.5 ?  tex : u_albedo; 
    metallic = metallicFlag > 0.5 ? texture(metallicMap, TexCoords).r : u_metallic; 
    roughness = roughnessFlag > 0.5 ? texture(roughnessMap, TexCoords).r : u_roughness; 
    ao = aoFlag > 0.5 ?  texture(aoMap, TexCoords).r :u_ao;    

    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewPos - FragPos);
    vec3 R = reflect(-V, N); 

     
    // reflectance equation
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    
    vec3 Lo = vec3(0.0);
    ambient =  vec3(0.0);

    for(int i = 0; i < NR_SUN_LIGHTS; i++)
    {
      if (!sunLights[i].enabled)
      {
        continue;
      }
     
      Lo += CalcDirLight(sunLights[i],F0, N,V);
    }

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
      if (!pointLights[i].enabled)
      {
        continue;
      }
      
      Lo += CalcPointLight(pointLights[i],albedo, F0, N, FragPos, V,ambient);      
    }

    vec3 color = vec3(0);
    color += CalcIBL(F0,N,V,R);
    color += Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 


    if(TransparencyFlag > 0.5)
    {      
        vec4 alphaTex =  texture(albedoMap, TexCoords);
        alpha = alphaTex.a;
    }
    vec4 final = vec4(color,alpha);

    if(final.a < 0.5)
        discard;

    FragColor = final;
}

vec3 CalcIBL(vec3 F0, vec3 normal, vec3 viewDir,vec3 ref)
{
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(normal, viewDir), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    vec3 irradiance = texture(irradianceMap, normal).rgb ;
    vec3 diffuse = irradiance * albedo ;
 
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, ref,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(normal, viewDir), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);



    return ambient = (kD * diffuse + specular) * ao * intensity ;
}

vec3 CalcDirLight(SunLight light,vec3 F0, vec3 normal, vec3 viewDir)
{
    vec3 radiance = (light.diffuse * light.intensity) * 0.5;
    vec3 l = normalize(-light.direction);
    vec3 H = normalize(viewDir+ l);
    float NoL = clamp(dot(normal, l), 0.0, 1.0);
    //ambient += light.ambient;



    vec3 F  = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);  
	float G = GeometrySmith(normal, viewDir, l, roughness); 

    float NDF = DistributionGGX(normal, H, roughness);   
    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(normal, viewDir), 0.0) * max(dot(normal, l), 0.0) + 0.001; // 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator * light.specular;

    vec3 kS = F;
	vec3 kd = vec3(1.0) - kS;
    kd *= 1.0 - metallic;	
	vec3 diffuseBRDF = kd * albedo;

	// Cook-Torrance
    float NdotL = max(dot(normal, l), 0.0);  
    float shadow = ShadowCalculation(FragPosLightSpace,light);    
    vec3 result = (diffuseBRDF / PI + specular) * radiance * NoL;
    result *= ((1.0 - shadow)  * (NoL * 1.0)) ; //(shadow * (NoL * 1.0));
    return result;  
}

vec3 CalcPointLight(PointLight light,vec3 alb, vec3 F0,vec3 normal, vec3 fragPos, vec3 viewDir,vec3 m_ambient)
{   
    
 
     // calculate per-light radiance
        vec3 L = normalize(light.position - fragPos);
        vec3 H = normalize(viewDir+ L);
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        vec3 radiance = (light.diffuse* light.intensity ) * attenuation;
       

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(normal, H, roughness);   
        float G   = GeometrySmith(normal, viewDir, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);        
        
        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(normal, viewDir), 0.0) * max(dot(normal, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator * light.specular;
        
        vec3 mambient  = light.ambient * albedo;
     
        specular *= attenuation;
        mambient *= attenuation;
         // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	 
        
    
        // scale light by NdotL
        float NdotL = max(dot(normal, L), 0.0);        
    return  (kD * albedo / PI + specular) * radiance * NdotL + mambient; 

}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //ambient += light.ambient;
   return vec3(0.0);
}