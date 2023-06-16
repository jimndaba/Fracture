#version 450 core
out vec4 FragColor;
  
in vec2  OutUV;

uniform sampler2D srcTexture;
uniform float redOffset   =  0.009;
uniform float greenOffset =  0.006;
uniform float blueOffset  = -0.006;
uniform bool ByPass;

void main()
{     
   vec4 result = texture(srcTexture, OutUV);

  if(ByPass)
  {
    FragColor = result; 
    return;
  }


 vec2 direction = ( OutUV - 0.5 ) * 10.0;

 

  result.r = texture(srcTexture, OutUV + (direction * vec2(redOffset  ))).r;
  result.g = texture(srcTexture, OutUV + (direction * vec2(greenOffset))).g;
  result.ba = texture(srcTexture, OutUV + (direction * vec2(blueOffset ))).ba;
  FragColor = result;  
}