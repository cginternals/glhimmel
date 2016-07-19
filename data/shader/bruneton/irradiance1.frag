#version 330
#extension GL_ARB_shading_language_include : require
#include </data/shader/bruneton/include/uniforms.glsl>
#include </data/shader/bruneton/include/constants.glsl>
#include </data/shader/bruneton/include/irradianceRMuS.glsl>
#include </data/shader/bruneton/include/transmittance.glsl>

out vec4 out_color;

void main() {
    float r, muS;
    getIrradianceRMuS(r, muS);
    out_color = vec4(transmittance(r, muS) * max(muS, 0.0), 0.0);
}