#version 330
#extension GL_ARB_shading_language_include : require
#include </data/shader/bruneton/include/uniforms.glsl>
#include </data/shader/bruneton/include/constants.glsl>
#include </data/shader/bruneton/include/muMuSNu.glsl>
#include </data/shader/bruneton/include/phaseFunction.glsl>

uniform int layer;

uniform sampler3D deltaSRSampler;
uniform sampler3D inscatterSampler;

out vec4 out_color; 

void main() {
    float mu, muS, nu;
    getMuMuSNu(u_r, u_dhdH, mu, muS, nu);
    vec3 uvw = vec3(gl_FragCoord.xy, float(layer) + 0.5) / vec3(ivec3(RES_MU_S * RES_NU, RES_MU, RES_R));
    out_color  = texture(inscatterSampler, uvw);
    out_color += vec4(texture(deltaSRSampler, uvw).rgb / phaseFunctionR(nu), 0.0);
}