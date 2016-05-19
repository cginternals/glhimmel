#version 330
#extension GL_ARB_shading_language_include : require

layout(location = 0) out vec4 color;

in vec4 v_ray;
in vec4 v_razInvariant;
uniform mat4 razInverse;
uniform float srcAlpha;

uniform sampler2D back;
uniform sampler2D src;
uniform bool isHalf;

#include </data/shader/hband.glsl>
#include </data/shader/fakeSun.glsl>

void main(void)
{
    vec3 stu = normalize(v_ray.xyz);

    float m = 0.5 / (1.0 + stu.z);
    vec2 uv = vec2(stu.x, stu.y) * m + 0.5;

    vec4 fc = mix(
        texture2D(back, uv), texture2D(src, uv), srcAlpha);

    if(stu.z < 0.0) discard;

    if (fakeSun)
    {
        fc = computeFakeSun(
            normalize(v_razInvariant.xyz), sun, sunCoeffs, sunScale, fc.a);
    }

    if (hBand)
    {
        color = hband(
            stu.z, hbandScale, hbandWidth, hbandOffset, hbandColor, hbandBackground, fc);
    }
    else
    {
        color = fc;
    }

}
