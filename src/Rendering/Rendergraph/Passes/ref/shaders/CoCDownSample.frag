
#version 450 core
layout (location = 0) out vec4 FragColor;

in vec2 OutUV;

uniform sampler2D main_Texture;

float Weigh (vec3 c) {
    return 1 / (1 + max(max(c.r, c.g), c.b));
}

void main()
{  
    vec2 texSize = textureSize(main_Texture, 0);
    vec2 texelSize = 1.0 / texSize;
    
    
    vec4 o = texelSize.xyxy * vec2(-0.5, 0.5).xxyy;
    float coc0 = texture(main_Texture, OutUV + o.xy).r;
    float coc1 = texture(main_Texture, OutUV + o.zy).r;
    float coc2 = texture(main_Texture, OutUV + o.xw).r;
    float coc3 = texture(main_Texture, OutUV + o.zw).r;
  
    float cocMin = min(min(min(coc0, coc1), coc2), coc3);
    float cocMax = max(max(max(coc0, coc1), coc2), coc3);
    float coc = cocMax >= -cocMin ? cocMax : cocMin;

   FragColor = vec4(texture(main_Texture, OutUV).rgb, coc);
}