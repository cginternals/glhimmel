uniform bool fakeSun;
uniform vec3 sun;
uniform vec4 sunCoeffs;
uniform float sunScale;

vec4 computeFakeSun(
    const vec3 eye
,   const vec3 sun
,   const vec4 coeffs
,   const float scale
,   const float alpha)
{
    float s = scale * 2.0 / length(normalize(sun) - eye);
    s *= alpha * 0.1 + 0.2;            // Reduce suns' size on low alpha.
    s *= clamp(eye.z + 0.1, 0.0, 0.1); // Disappear in lower hemisphere.

    s  = clamp(clamp(s, 0.0, 2.0) - (1.0 - alpha) * 2.0, 0.0, 2.0);
    return vec4(coeffs.rgb * s, coeffs.a);
}