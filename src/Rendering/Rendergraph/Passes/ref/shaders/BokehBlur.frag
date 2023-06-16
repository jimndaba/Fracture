
#version 450 core
layout (location = 0) out vec4 FragColor;

in vec2 OutUV;

uniform sampler2D main_Texture;

// from http://www.java-gaming.org/index.php?topic=35123.0
vec4 cubic(float v){
    vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
    vec4 s = n * n * n;
    float x = s.x;
    float y = s.y - 4.0 * s.x;
    float z = s.z - 4.0 * s.y + 6.0 * s.x;
    float w = 6.0 - x - y - z;
    return vec4(x, y, z, w) * (1.0/6.0);
}


vec4 PostFilter (sampler2D _main_tex,vec2 texCoords  ){
    vec2 texSize = textureSize(_main_tex, 0);
    vec2 texelSize = 1.0 / texSize;

/*
    texCoords = texCoords * texSize - 0.5;

   
    vec2 fxy = fract(texCoords);
    texCoords -= fxy;

    vec4 xcubic = cubic(fxy.x);
    vec4 ycubic = cubic(fxy.y);

    vec4 c = texCoords.xxyy + vec2 (-0.5, +1.5).xyxy;
    
    vec4 s = vec4(xcubic.xz + xcubic.yw, ycubic.xz + ycubic.yw);
    vec4 offset = c + vec4 (xcubic.yw, ycubic.yw) / s;
    
    offset *= texelSize.xxyy;
    
    vec4 sample0 = texture(_main_tex, offset.xz);
    vec4 sample1 = texture(_main_tex, offset.yz);
    vec4 sample2 = texture(_main_tex, offset.xw);
    vec4 sample3 = texture(_main_tex, offset.yw);

    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);

    return mix(
       mix(sample3, sample2, sx), mix(sample1, sample0, sx)
    , sy);
*/


    vec4 o = texelSize.xyxy * vec2(-0.5, 0.5).xxyy;
    vec4 s =
        texture(_main_tex, texCoords + o.xy) +
        texture(_main_tex, texCoords + o.zy) +
        texture(_main_tex, texCoords + o.xw) +
        texture(_main_tex, texCoords + o.zw);
	return s * 0.25;
}


void main()
{  
   vec4 color = PostFilter(main_Texture,OutUV);
   FragColor = color;
}