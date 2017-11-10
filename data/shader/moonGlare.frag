#version 150 compatibility

uniform vec3 sun;

uniform float intensity;

uniform vec4 sunShine;
uniform vec3 earthShine;

uniform float phase;

const float radius = 0.98;

in vec2 v_texCoord;
out vec4 out_color;

void main(void)
{
    float x = v_texCoord.x;
    float y = v_texCoord.y;

    float zz = radius * radius - x * x - y * y;
    if(zz < 1.0 - radius)
        discard;

    float l = 1 - pow(length(vec2(x, y)) + 0.02, 0.25);
    // Day-Twilight-Night-Intensity Mapping (Butterworth-Filter)
    float b = 1.0 / sqrt(1 + pow(sun.z + 1.05, 32));

    out_color = vec4(l * (phase * sunShine.rgb) + l * earthShine , 1.0) * intensity * b;
}
