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

    float yy = (1.0 - stu.y);
    yy *= yy;
    float m = 0.5 / sqrt(stu.x * stu.x + stu.z * stu.z + yy);
    vec2 uv = vec2(-stu.x, stu.z) * m + 0.5;

    vec4 fc = mix(
        texture(back, uv), texture(src, uv), srcAlpha);

    if (fakeSun)
    {
        fc += computeFakeSun(
            normalize(v_razInvariant.xyz), fc.a);
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
