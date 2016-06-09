#extension GL_ARB_shading_language_include : require
#include </data/shader/bruneton/include/uniforms.glsl>
#include </data/shader/bruneton/include/constants.glsl>

#ifndef IRRADIANCERMUS
#define IRRADIANCERMUS

void getIrradianceRMuS(out float r, out float muS) {
    r = cmn[1] + (gl_FragCoord.y - 0.5) / (float(SKY_H) - 1.0) * (cmn[2] - cmn[1]);
    muS = -0.2 + (gl_FragCoord.x - 0.5) / (float(SKY_W) - 1.0) * (1.0 + 0.2);
}

#endif // IRRADIANCERMUS