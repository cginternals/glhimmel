#version 330
#extension GL_ARB_shading_language_include : require
#include </data/shader/bruneton/include/constants.glsl>
#include </data/shader/bruneton/include/uniforms.glsl>
#include </data/shader/bruneton/include/limit.glsl>
  

void getTransmittanceRMu(out float r, out float muS) {
    r = gl_FragCoord.y / float(TRANSMITTANCE_H);
    muS = gl_FragCoord.x / float(TRANSMITTANCE_W);
    r = u_apparentAngularRadius + (r * r) * (u_radiusUpToEndOfAtmosphere - u_apparentAngularRadius);
    muS = -0.15 + tan(1.5 * muS) / tan(1.5) * (1.0 + 0.15);
}

float opticalDepth(float H, float r, float mu) {
    float result = 0.0;
    float dx = limit(r, mu) / float(TRANSMITTANCE_INTEGRAL_SAMPLES);
    float xi = 0.0;
    float yi = exp(-(r - u_apparentAngularRadius) / H);
    for (int i = 1; i <= TRANSMITTANCE_INTEGRAL_SAMPLES; ++i) {
        float xj = float(i) * dx;
        float yj = exp(-(sqrt(r * r + xj * xj + 2.0 * xj * r * mu) - u_apparentAngularRadius) / H);
        result += (yi + yj) / 2.0 * dx;
        xi = xj;
        yi = yj;
    }
    return mu < -sqrt(1.0 - (u_apparentAngularRadius / r) * (u_apparentAngularRadius / r)) ? 1e9 : result;
}

void main() {
    float r, muS;
    getTransmittanceRMu(r, muS);
    vec3 depth = u_betaR * opticalDepth(u_HR, r, muS) + u_betaMEx * opticalDepth(HM, r, muS);
    gl_FragColor = vec4(exp(-depth), 0.0); // Eq (5)
}   