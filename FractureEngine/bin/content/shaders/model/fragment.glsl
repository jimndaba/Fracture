#version 330 core
out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D normal;
  sampler2D reflection;
  float shininess;
  bool castShadows;
};

struct SunLight {
    bool enabled;
    vec3 direction;
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
in vec3 Position;
in vec4 FragPosLightSpace;

uniform SunLight sunLight;
uniform Material material;
uniform vec3 viewPos;
uniform samplerCube skybox;
uniform sampler2D shadowMap;
uniform int NoOfLights;

#define NR_SUN_LIGHTS 1
uniform SunLight sunLights[NR_SUN_LIGHTS];

#define NR_POINT_LIGHTS 10
uniform PointLight pointLights[NR_POINT_LIGHTS];

#define NR_SPOT_LIGHTS 10
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec3 CalcDirLight(SunLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);

    vec3 viewDir = normalize(viewPos- FragPos);

    vec3 result;

    for(int i = 0; i < NR_SUN_LIGHTS; i++)
    {
      if (!sunLights[i].enabled)
      {
        continue;
      }
      result += CalcDirLight(sunLights[i],norm,viewDir);//
    }

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
      if (!pointLights[i].enabled)
      {
        continue;
      }
      result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
      if (!spotLights[i].enabled)
      {
        continue;
      }
      result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    }
  
    FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(SunLight light, vec3 normal, vec3 viewDir)
{
      vec3 lightDir = normalize(-light.direction);
      // diffuse shading
      float diff = max(dot(normal, lightDir), 0.0);
      // specular shading
      vec3 reflectDir = reflect(-lightDir, normal);
      vec3 halfwayDir = normalize(lightDir + viewDir);
      float spec = pow(max(dot(normal , halfwayDir), 0.0), material.shininess);


      //vec3 ambient  = 0.5f * color;
      vec3 ambient  = light.ambient * texture(material.diffuse, TexCoords).rgb;
      vec3 diffuse  = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
      vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
      return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient  * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient  * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
