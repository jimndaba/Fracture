#version 450 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 gNormal_Metalic;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gPosition;
layout (location = 4) out vec3 gEmissive;

const int MAX_LIGHTS = 10;

struct Light
{
    vec4 DirectionXYZ_IntensityW;
    vec4 PositionXYZ_RadiusW; //Radius for Point lights 
    vec4 SpecularXYZ_AttenuationW;
    vec4 AmbientRGB_InnerCOW;
    vec4 DiffuseRGB_OuterCOW;   
    bool enabled;//0_ OFF, 1 _ ON
    int type ; // 0 _ invalid ,1 _ Direc, 2 _ Point , 3 _ Spot
    vec2 _pad;
};
struct CascadeShadowData
{
    int CascadeCount;
    float CameraFarPlane;
    float Bias;
    float _pad;
};


layout(std140 , binding = 7) uniform sceneLights
{
   Light light[MAX_LIGHTS];
};

layout (std140, binding = 2) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

layout(std140 , binding = 3) uniform shadowData
{
   CascadeShadowData cData;
};

layout(std140 , binding = 4) uniform CascadePlaneDistances
{
   float cascadePlaneDistances[4];
};

in vec3 FragPos; 
in vec3 Normal;
in vec2 TexCoord;
in vec3 viewPos;
in mat4 view;

//PBR
uniform sampler2D aDiffuse; 
uniform sampler2D aNormal; 
uniform sampler2D aMetallic; 
uniform sampler2D aRoughness; 
uniform sampler2D aAO;
uniform sampler2D aEmissive;

//GLOBALS
uniform sampler2DArray aShadowMap;
uniform sampler2D GlobalAO;
uniform samplerCube IrradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   BrdfMap; 

uniform int _NormalFlag = 0;
uniform int _MetalFlag = 0;
uniform int _RoughFlag = 0;
uniform int _DiffuseFlag = 0;
uniform int _AOFlag = 0;
uniform int _EmmisiveFlag = 0;

vec3 CalcLight(Light light, vec3 normal, vec3 viewDir,vec3 F0);  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

vec3 getNormalFromMap();

float GetShadowBias(vec3 normal, vec3 lightDir);
float ShadowCalculation(vec3 fragPosWorldSpace,vec3 lightDir, vec3 normal);



vec3 pNormal = vec3(1.0);
float pMetalness = 1.0f;
float pRoughness = 0.4f;
float pAO = 0.4f;
vec3 albedo = vec3(1.0);
vec3 pEmissive = vec3(0.0);
const float PI = 3.14159265359;

uniform vec3 Diffuse = vec3(0.0, 0.6471, 0.6235);
uniform vec3 Emissive = vec3(0);
uniform float Metalness = 1.0f;
uniform float Roughness = 0.1f;
uniform float ao = 0.4f;
float globalAO = 1.0f;

void main()
{ 
    albedo = _DiffuseFlag == 1 ? texture(aDiffuse,TexCoord).rgb :  Diffuse;
    pNormal = _NormalFlag == 1 ? getNormalFromMap() : normalize(Normal).rgb;
    pMetalness = _MetalFlag == 1 ? texture(aMetallic,TexCoord).r : Metalness;
    pRoughness = _RoughFlag == 1 ? texture(aRoughness,TexCoord).r: Roughness;
    pEmissive = _EmmisiveFlag == 1? texture(aEmissive,TexCoord).rgb: Emissive;
    pAO = _AOFlag == 1 ? texture(aAO,TexCoord).r : ao;
   
    vec3 irradiance = texture(IrradianceMap,pNormal).rgb;


    vec2 texSize = textureSize(GlobalAO, 0).xy * 2.0f; 
    vec2 ssao_texCoord = (gl_FragCoord.xy / texSize);

    globalAO = texture(GlobalAO,ssao_texCoord).r;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 oLightout = vec3(0.0);
  
    vec3 R = reflect(-viewDir, pNormal);   
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  pRoughness * MAX_REFLECTION_LOD).rgb;   


    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, pMetalness);

    for(int i = 0 ;i < MAX_LIGHTS; i++)
    {
        if(!light[i].enabled)
        {
            continue;
        }       
        
        oLightout += CalcLight(light[i], pNormal,viewDir,F0);
    }   

    if(texture(aDiffuse,TexCoord).a <= 0.2)
    {
        //discard;
    }

    //vec3 F = fresnelSchlick(max(dot(pNormal,viewDir), 0.0), F0);
    vec3 F = fresnelSchlickRoughness(max(dot(pNormal,viewDir), 0.0), F0, pRoughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;	    
    kD *= 1.0 - pMetalness;  

    vec2 envBRDF  = texture(BrdfMap, vec2(max(dot(pNormal, viewDir), 0.0), pRoughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
    vec3 diffuse = irradiance * albedo;
    vec3 ambient = (kD * (diffuse + specular)) * ao * 0.4;// ;
    //ambient ;    
   
    vec3 color = vec3(0);    
    color += oLightout ;
    color += ambient;  
    color += pEmissive;
    color *= globalAO;

    FragColor = vec4(color, 1.0);
    gNormal_Metalic = vec4(pNormal ,pMetalness);
    gAlbedo = vec4(albedo,1.0);
    gPosition = FragPos;
    gEmissive = pEmissive;
} 

vec3 CalcLight(Light light, vec3 normal, vec3 viewDir,vec3 F0)
{
    ////////////////////////////////////////////////////////////////////////
    /* Properties
    vec4 DirectionXYZ_IntensityW;
    vec4 PositionXYZ_RadiusW; //Radius for Point lights 
    vec4 SpecularXYZ_AttenuationW;
    vec4 AmbientRGB_InnerCOW;
    vec4 Diffuse_OuterCOW;;    
    int enabled = 0;//0_ OFF, 1 _ ON
    int type = 0; // 0 _ invalid ,1 _ Direc, 2 _ Point , 3 _ Spot
    vec2 _pad;
    */
    ///////////////////////////////////////////////////////////////////////    
    switch(light.type)
    {
        //Directional Lights
        case 1:
        {                        
            vec3 lightDir = normalize(-light.DirectionXYZ_IntensityW.rgb);
            vec3 L = normalize(-light.DirectionXYZ_IntensityW.rgb);
            vec3 H = normalize(viewDir + L);
           
            // cook-torrance brdf
            float NDF = DistributionGGX(normal, H, pRoughness);        
            float G   = GeometrySmith(normal,viewDir , L, pRoughness);      
            vec3 F    = fresnelSchlick(max(dot(H,viewDir ), 0.0), F0);       
            
            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;
            kD *= 1.0 - pMetalness;	     
            
            vec3 numerator    = NDF * G * F;
            float denominator = 4.0 * max(dot(normal, viewDir ), 0.0) * max(dot(normal, L), 0.0) + 0.0001;
            vec3 specular     = (numerator / denominator) * light.SpecularXYZ_AttenuationW.rgb;  
             
             
            float shadow = 1;
            shadow = ShadowCalculation(FragPos,lightDir,normal);

            // add to outgoing radiance Lo
            float NdotL = max(dot(normal, L), 0.0);                
            return (kD * albedo  / PI + specular) * (1.0 - shadow) * NdotL*light.DiffuseRGB_OuterCOW.rgb* light.DirectionXYZ_IntensityW.a;//

            //return (r_diffuse + r_specular) * light.DirectionXYZ_IntensityW.a;//r_ambient + 
        }

         //Point Lights
        case 2:
        {
            // calculate per-light radiance
        vec3 L = normalize(light.PositionXYZ_RadiusW.xyz - FragPos);
        vec3 H = normalize(viewDir + L);
        //float distance = length(light.PositionXYZ_RadiusW.xyz - FragPos);
        //float attenuation = 1.0 / (distance * distance);
        float attenuation = pow(smoothstep(light.PositionXYZ_RadiusW.w, 0, length(light.PositionXYZ_RadiusW.xyz - FragPos)),light.SpecularXYZ_AttenuationW.w);
        vec3 radiance = light.DiffuseRGB_OuterCOW.xyz * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(normal, H, pRoughness);   
        float G   = GeometrySmith(normal,viewDir, L, pRoughness);    
        vec3 F    = fresnelSchlick(max(dot(H,viewDir), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = (numerator / denominator) * light.SpecularXYZ_AttenuationW.rgb;
        
         // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - pMetalness;	                
            
        // scale light by NdotL
        float NdotL = max(dot(normal, L), 0.0);        

        // add to outgoing radiance Lo
        return (kD * albedo / PI + specular) * radiance * NdotL * light.DirectionXYZ_IntensityW.a; 
        }

         //Spot Lights
        case 3:
        {

            break;
        }
    }

    return vec3(0.0);
};

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

vec3 getNormalFromMap()
{
    vec3 tangentNormal = normalize(Normal.rgb);

    if(_NormalFlag > 0.5)
    {
        tangentNormal = texture(aNormal, TexCoord).xyz * 2.0 - 1.0;
        vec3 Q1  = dFdx(FragPos);
        vec3 Q2  = dFdy(FragPos);
        vec2 st1 = dFdx(TexCoord);
        vec2 st2 = dFdy(TexCoord);

        vec3 N   = normalize(Normal.rgb);
        vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
        vec3 B  = -normalize(cross(N, T));
        mat3 TBN = mat3(T, B, N);
        tangentNormal = normalize(TBN * tangentNormal);
    }

    return tangentNormal;
}

float GetShadowBias(vec3 normal, vec3 lightDir)
{
	const float MINIMUM_SHADOW_BIAS = 0.0005f;
	float bias = max(MINIMUM_SHADOW_BIAS * (1.0 - dot(normal, lightDir)), cData.Bias);
	return bias;
}

float ShadowCalculation(vec3 fragPosWorldSpace,vec3 lightDir, vec3 normal)
{
    // select cascade layer
    vec4 fragPosViewSpace = view * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cData.CascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cData.CascadeCount;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth  > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float bias = GetShadowBias(normal, lightDir);
    if (layer == cData.CascadeCount)
    {
        bias *= 1 / (cData.CameraFarPlane * 0.5f);
    }
    else
    {
        bias *= 1 / (cascadePlaneDistances[layer] * 0.5f);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(aShadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(aShadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r; 
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
        
    return shadow;
};