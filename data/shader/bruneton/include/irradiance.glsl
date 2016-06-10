#ifndef IRRADIANCE
#define IRRADIANCE

vec2 getIrradianceUV(float r, float muS) {
    float uR = (r - u_apparentAngularRadius) / (u_radiusUpToEndOfAtmosphere - u_apparentAngularRadius);
    float uMuS = (muS + 0.2) / (1.0 + 0.2);
    return vec2(uMuS, uR);
}

vec3 irradiance(sampler2D sampler, float r, float muS) {
    vec2 uv = getIrradianceUV(r, muS);
    return texture2D(sampler, uv).rgb;
}

#endif // IRRADIANCE