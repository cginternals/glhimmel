#version 330
#extension GL_ARB_shading_language_include : require
#include </data/shader/bruneton/include/uniforms.glsl>
#include </data/shader/bruneton/include/constants.glsl>
#include </data/shader/bruneton/include/transmittance.glsl>
#include </data/shader/bruneton/include/muMuSNu.glsl>
#include </data/shader/bruneton/include/limit.glsl>

out vec4 out_color1;
out vec4 out_color2;

void integrand(float r, float mu, float muS, float nu, float t, out vec3 ray, out vec3 mie) {
    ray = vec3(0.0);
    mie = vec3(0.0);
    float ri = sqrt(r * r + t * t + 2.0 * r * mu * t);
    float muSi = (nu * t + muS * r) / ri;
    ri = max(u_apparentAngularRadius, ri);
    if (muSi >= -sqrt(1.0 - u_apparentAngularRadius * u_apparentAngularRadius / (ri * ri))) {
        vec3 ti = transmittance(r, mu, t) * transmittance(ri, muSi);
        ray = exp(-(ri - u_apparentAngularRadius) / u_HR) * ti;
        mie = exp(-(ri - u_apparentAngularRadius) / u_HM) * ti;
    }
}

void inscatter(float r, float mu, float muS, float nu, out vec3 ray, out vec3 mie) {
    ray = vec3(0.0);
    mie = vec3(0.0);
    float dx = limit(r, mu) / float(INSCATTER_INTEGRAL_SAMPLES);
    float xi = 0.0;
    vec3 rayi;
    vec3 miei;
    integrand(r, mu, muS, nu, 0.0, rayi, miei);
    for (int i = 1; i <= INSCATTER_INTEGRAL_SAMPLES; ++i) {
        float xj = float(i) * dx;
        vec3 rayj;
        vec3 miej;
        integrand(r, mu, muS, nu, xj, rayj, miej);
        ray += (rayi + rayj) / 2.0 * dx;
        mie += (miei + miej) / 2.0 * dx;
        xi = xj;
        rayi = rayj;
        miei = miej;
    }
    ray *= u_betaR;
    mie *= u_betaMSca;
}

void main() {
    vec3 ray;
    vec3 mie;
    float mu, muS, nu;
    getMuMuSNu(u_r, u_dhdH, mu, muS, nu);
    inscatter(u_r, mu, muS, nu, ray, mie);
    // store separately Rayleigh and Mie contributions, WITHOUT the phase function factor
    // (cf 'Angular precision')
    out_color1.rgb = ray;
    out_color2.rgb = mie;
}