uniform bool fakeSun;
uniform vec3 sunCoords;
uniform vec4 sunCoeffs;
uniform float sunScale;

vec4 computeFakeSun(
    const vec3 eye
,   const float alpha)
{
    float s = sunScale * 2.0 / length(normalize(sunCoords) - eye);
    s *= alpha * 0.1 + 0.2;            // Reduce suns' size on low alpha.
    s *= clamp(eye.z + 0.1, 0.0, 0.1); // Disappear in lower hemisphere.

    s  = clamp(clamp(s, 0.0, 2.0) - (1.0 - alpha) * 2.0, 0.0, 2.0);
    return vec4(sunCoeffs.rgb * s, sunCoeffs.a);
}