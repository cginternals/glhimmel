#version 330

uniform float q;
uniform float scale;
uniform float glareIntensity;

uniform vec3 sun;

in vec3 g_color;
in vec3 v_texCoord;

layout(location = 0) out vec4 fragcolor;

void main(void)
{
    float x = v_texCoord.x;
    float y = v_texCoord.y;

    float zz = (1 - x * x - y * y);

    if(zz < 0)
        discard;

    float k =  v_texCoord.z;

    float l = length(vec2(x, y));

    float t = 1 - smoothstep(0.0, 1.0, l * k / scale);
    float g = 1 - pow(l, glareIntensity / 64.0);

    fragcolor = vec4(max(t, g) * g_color, 1.0);
}
