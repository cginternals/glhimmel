#include </data/shader/bruneton/include/uniforms.glsl>
#include </data/shader/bruneton/include/constants.glsl>

#ifndef IRRADIANCERMUS
#define IRRADIANCERMUS

void getIrradianceRMuS(out float r, out float muS) {
    r = u_apparentAngularRadius + (gl_FragCoord.y - 0.5) / (float(SKY_H) - 1.0) * (u_radiusUpToEndOfAtmosphere - u_apparentAngularRadius);
    muS = -0.2 + (gl_FragCoord.x - 0.5) / (float(SKY_W) - 1.0) * (1.0 + 0.2);
}

#endif // IRRADIANCERMUS
