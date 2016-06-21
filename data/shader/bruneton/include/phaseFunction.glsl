#include </data/shader/bruneton/include/uniforms.glsl>
#include </data/shader/bruneton/include/constants.glsl>

#ifndef PHASE_FUNCTION
#define PHASE_FUNCTION

float phaseFunctionR(float mu) {
    return (3.0 / (16.0 * PI)) * (1.0 + mu * mu);
}

float phaseFunctionM(float mu) {
    return 1.5 * 1.0 / (4.0 * PI) * (1.0 - u_mieG*u_mieG) * pow(1.0 + (u_mieG*u_mieG) - 2.0*u_mieG*mu, -3.0/2.0) * (1.0 + mu * mu) / (2.0 + u_mieG*u_mieG);
}

#endif // PHASE_FUNCTION
