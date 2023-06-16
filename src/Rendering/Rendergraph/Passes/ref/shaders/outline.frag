#version 450

out vec4 FragColor;

in vec2 OutUV;
uniform sampler2D silhouette;

uniform vec3 color = vec3(1.0, 0.8353, 0.0);
uniform float WIDTH = 2;

void main()
{
    vec2 texSize  = textureSize(silhouette, 0).xy;
    vec2 pixelSize = vec2(1/texSize.x,1/texSize.y);
    bool isInside = false;
    int count = 0;
    float coverage = 0.0;
    float dist = 1e6;
    for (float y = -WIDTH;  y <= WIDTH;  ++y) {
        for (float x = -WIDTH;  x <= WIDTH;  ++x) {
            vec2 dUV = vec2(float(x) * pixelSize.x, float(y) * pixelSize.y);
            float mask = texture2D(silhouette, OutUV + dUV).r;
            coverage += mask;
            if (mask >= 0.5) {
                dist = min(dist, sqrt(float(x * x + y * y)));
            }
            if (x == 0 && y == 0) {
                isInside = (mask > 0.5);
            }
            count += 1;
        }
    }

    coverage /= float(count);
    float a;
    if (isInside) {
        a = min(1.0, (1.0 - coverage) / 0.75);
    } else {
        const float solid = 0.3 * float(WIDTH);
        const float fuzzy = float(WIDTH) - solid;
        a = 1.0 - min(1.0, max(0.0, dist - solid) / fuzzy);       
    }

    if(a < 0.1)
        discard;    

    FragColor = vec4(color,a);    
}