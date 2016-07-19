#version 330
#extension GL_ARB_shading_language_include : require
#include </data/shader/bruneton/include/constants.glsl>
#include </data/shader/bruneton/include/uniforms.glsl>
#include </data/shader/bruneton/include/muMuSNu.glsl>
#include </data/shader/bruneton/include/transmittance.glsl>
#include </data/shader/bruneton/include/texture4D.glsl>
#include </data/shader/bruneton/include/phaseFunction.glsl>
#include </data/shader/bruneton/include/irradiance.glsl>


uniform sampler2D deltaESampler;
uniform sampler3D deltaSRSampler;
uniform sampler3D deltaSMSampler;
uniform float first;

const float dphi = PI / float(INSCATTER_SPHERICAL_INTEGRAL_SAMPLES);
const float dtheta = PI / float(INSCATTER_SPHERICAL_INTEGRAL_SAMPLES);

out vec4 out_color; 

void inscatter(float r, float mu, float muS, float nu, out vec3 raymie) {
    r = clamp(r, u_apparentAngularRadius, u_radiusUpToEndOfAtmosphere);
    mu = clamp(mu, -1.0, 1.0);
    muS = clamp(muS, -1.0, 1.0);
    float var = sqrt(1.0 - mu * mu) * sqrt(1.0 - muS * muS);
    nu = clamp(nu, muS * mu - var, muS * mu + var);

    float cthetamin = -sqrt(1.0 - (u_apparentAngularRadius / r) * (u_apparentAngularRadius / r));

    vec3 v = vec3(sqrt(1.0 - mu * mu), 0.0, mu);
    float sx = v.x == 0.0 ? 0.0 : (nu - muS * mu) / v.x;
    vec3 s = vec3(sx, sqrt(max(0.0, 1.0 - sx * sx - muS * muS)), muS);

    raymie = vec3(0.0);

    // integral over 4.PI around x with two nested loops over w directions (theta,phi) -- Eq (7)
    for (int itheta = 0; itheta < INSCATTER_SPHERICAL_INTEGRAL_SAMPLES; ++itheta) {
        float theta = (float(itheta) + 0.5) * dtheta;
        float ctheta = cos(theta);

        float greflectance = 0.0;
        float dground = 0.0;
        vec3 gtransp = vec3(0.0);
        if (ctheta < cthetamin) { // if ground visible in direction w
            // compute transparency gtransp between x and ground
            greflectance = u_averageGroundReflectance / PI;
            dground = -r * ctheta - sqrt(r * r * (ctheta * ctheta - 1.0) + u_apparentAngularRadius * u_apparentAngularRadius);
            gtransp = transmittance(u_apparentAngularRadius, -(r * ctheta + dground) / u_apparentAngularRadius, dground);
        }

        for (int iphi = 0; iphi < 2 * INSCATTER_SPHERICAL_INTEGRAL_SAMPLES; ++iphi) {
            float phi = (float(iphi) + 0.5) * dphi;
            float dw = dtheta * dphi * sin(theta);
            vec3 w = vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), ctheta);

            float nu1 = dot(s, w);
            float nu2 = dot(v, w);
            float pr2 = phaseFunctionR(nu2);
            float pm2 = phaseFunctionM(nu2);

            // compute irradiance received at ground in direction w (if ground visible) =deltaE
            vec3 gnormal = (vec3(0.0, 0.0, r) + dground * w) / u_apparentAngularRadius;
            vec3 girradiance = irradiance(deltaESampler, u_apparentAngularRadius, dot(gnormal, s));

            vec3 raymie1; // light arriving at x from direction w

            // first term = light reflected from the ground and attenuated before reaching x, =T.alpha/PI.deltaE
            raymie1 = greflectance * girradiance * gtransp;

            // second term = inscattered light, =deltaS
            if (first == 1.0) {
                // first iteration is special because Rayleigh and Mie were stored separately,
                // without the phase functions factors; they must be reintroduced here
                float pr1 = phaseFunctionR(nu1);
                float pm1 = phaseFunctionM(nu1);
                vec3 ray1 = texture4D(deltaSRSampler, r, w.z, muS, nu1).rgb;
                vec3 mie1 = texture4D(deltaSMSampler, r, w.z, muS, nu1).rgb;
                raymie1 += ray1 * pr1 + mie1 * pm1;
            } else {
                raymie1 += texture4D(deltaSRSampler, r, w.z, muS, nu1).rgb;
            }

            // light coming from direction w and scattered in direction v
            // = light arriving at x from direction w (raymie1) * SUM(scattering coefficient * phaseFunction)
            // see Eq (7)
            raymie += raymie1 * (u_betaR * exp(-(r - u_apparentAngularRadius) / u_HR) * pr2 + u_betaMSca * exp(-(r - u_apparentAngularRadius) / u_HM) * pm2) * dw;
        }
    }

    // output raymie = J[T.alpha / PI.deltaE + deltaS] (line 7 in algorithm 4.1)
}

void main() {
    vec3 raymie;
    float mu, muS, nu;
    getMuMuSNu(u_r, u_dhdH, mu, muS, nu);
    inscatter(u_r, mu, muS, nu, raymie);
    out_color.rgb = raymie;
}