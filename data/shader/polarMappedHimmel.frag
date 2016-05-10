#version 330

layout(location = 0) out vec4 color;

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

in vec4 v_razInvariant;
uniform mat4 razInverse;
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
    vec3 fix = normalize(eye.xyz);
    float s = scale * 2.0 / length(normalize(sun) - eye);
    s *= alpha * 0.1 + 0.2;            // Reduce suns' size on low alpha.
    s *= clamp(eye.z + 0.1, 0.0, 0.1); // Disappear in lower hemisphere.

    s  = clamp(clamp(s, 0.0, 2.0) - (1.0 - alpha) * 2.0, 0.0, 2.0);
    return vec4(coeffs.rgb * s, coeffs.a);
}

uniform float hbandScale;
uniform float hbandWidth;
uniform float hbandOffset;
uniform vec4 hbandColor;
uniform vec4 hbandBackground;

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

uniform bool isHalf;
uniform bool hBand;
uniform bool fakeSun;


in vec4 v_ray;

// From AbstractMappedHimmel

uniform float srcAlpha;
uniform sampler2D back;
uniform sampler2D src;


// Color Retrieval

const float c_2OverPi  = 0.6366197723675813430755350534901;
const float c_1Over2Pi = 0.1591549430918953357688837633725;
const float c_1OverPi  = 0.3183098861837906715377675267450;

void main(void)
{
    vec3 stu = normalize(v_ray.xyz);


    // TODO: c_1OverPi was not defined anywhere (typo?)
    float v = isHalf ? asin(+stu.z) * c_2OverPi : acos(-stu.z) * c_1OverPi;
    vec2 uv = vec2(atan(stu.x, stu.y) * c_1Over2Pi, v);

    vec4 fc = mix(texture2D(back, uv), texture2D(src, uv), srcAlpha);

    /*if (fakeSun)
    {
        fc += computeFakeSun(
            normalize(v_razInvariant.xyz), sun, sunCoeffs, sunScale, fc.a);
    }

    if (hBand)
    {
        color = hband(
            stu.z, hbandScale, hbandWidth, hbandOffset, hbandColor, hbandBackground, fc);
    }
    else
    {*/
        color = fc;
    //}

}
