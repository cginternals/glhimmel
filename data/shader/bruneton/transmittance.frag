#extension GL_ARB_shading_language_include : require
#inculde </data/shader/bruneton/uniforms.glsl>
#include </data/shader/bruneton/constants.glsl>


void getTransmittanceRMu(out float r, out float muS) {
    r = gl_FragCoord.y / float(TRANSMITTANCE_H);
    muS = gl_FragCoord.x / float(TRANSMITTANCE_W);
    r = cmn[1] + (r * r) * (cmn[2] - cmn[1]);
    muS = -0.15 + tan(1.5 * muS) / tan(1.5) * (1.0 + 0.15);
}

float limit(float r, float mu) {
    float RL = cmn[2] + 1.0;
    float dout = -r * mu + sqrt(r * r * (mu * mu - 1.0) + RL * RL);
    float delta2 = r * r * (mu * mu - 1.0) + cmn[1] * cmn[1];
    if (delta2 >= 0.0) {
        float din = -r * mu - sqrt(delta2);
        if (din >= 0.0) {
            dout = min(dout, din);
        }
    }
    return dout;
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
    vec3 depth = betaR * opticalDepth(HR, r, muS) + betaMEx * opticalDepth(HM, r, muS);
    gl_FragColor = vec4(exp(-depth), 0.0);\n // Eq (
}   