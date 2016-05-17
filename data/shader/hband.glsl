uniform bool hBand;
uniform float hbandScale;
uniform float hbandWidth;
uniform float hbandOffset;
uniform vec4 hbandColor;
uniform vec4 hbandBackground;

vec4 blend_normal (vec4 back, vec4 src, float srca)
{
    vec3 b = back.rgb;
    vec3 s = src.rgb;
    vec3 bs = s;
    float ba = back.a;
    float sa = clamp(srca, 0.0, 1.0);
    float ra = ba + sa - (ba * sa);
    vec3 r = (1.0 - sa / ra) * b + (sa / ra) * ((1.0 - ba) * s + ba * bs);
    return vec4(r, ra);
}

vec4 hband(
    const float z
,   const float scale
,   const float width
,   const float offset
,   const vec4 color
,   const vec4 background
,   vec4 fc /* fragment color */)
{
    fc = mix(fc, background, step(z, offset));
    float b = abs(z - offset) / scale;
    b = smoothstep(width, 1.0, b);
    return blend_normal(color, fc, b);
}

