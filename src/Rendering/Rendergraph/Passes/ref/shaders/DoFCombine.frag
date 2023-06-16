
#version 450 core
layout (location = 0) out vec4 FragColor;

in vec2 OutUV;

uniform sampler2D main_Texture;
uniform sampler2D bokeh_Texture;
uniform sampler2D coc_Texture;

void main()
{  
    vec3 main_text = texture(main_Texture, OutUV).rgb;
    vec3 bokeh = texture(bokeh_Texture, OutUV).rgb;
    float coc = texture(coc_Texture, OutUV).r;

	float dofStrength = smoothstep(0.1, 1, abs(coc));
	vec3 color = mix(main_text, bokeh, dofStrength + texture(bokeh_Texture, OutUV).a - dofStrength * texture(bokeh_Texture, OutUV).a);
    
    FragColor = vec4(color,coc);
}