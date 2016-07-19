#version 330
#extension GL_ARB_shading_language_include : require
#include </data/shader/bruneton/include/constants.glsl>

uniform sampler3D deltaSRSampler;
uniform sampler3D deltaSMSampler;

uniform int layer;

out vec4 out_color; 

void main() {
    vec3 uvw = vec3(gl_FragCoord.xy, float(layer) + 0.5) / vec3(ivec3(RES_MU_S * RES_NU, RES_MU, RES_R));
    vec4 ray = texture(deltaSRSampler, uvw);
    vec4 mie = texture(deltaSMSampler, uvw);
    out_color = vec4(ray.rgb, mie.r); // store only red component of single Mie scattering (cf. 'Angular precision')
}