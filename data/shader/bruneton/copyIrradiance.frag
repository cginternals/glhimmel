#version 330
#extension GL_ARB_shading_language_include : require
#include </data/shader/bruneton/include/constants.glsl>

uniform float k; // k = 0 for line 4, k = 1 for line 10
uniform sampler2D deltaESampler;
uniform sampler2D irradianceSampler;

out vec4 out_color;

void main() {
    vec2 uv = gl_FragCoord.xy / vec2(SKY_W, SKY_H);
    out_color  = k * texture(irradianceSampler, uv); // k = 0 for line 4, k = 1 for line 10
    out_color += texture(deltaESampler, uv);
}