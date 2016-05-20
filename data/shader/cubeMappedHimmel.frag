#version 330
#extension GL_ARB_shading_language_include : require

layout(location = 0) out vec4 color;

in vec4 v_ray;
in vec4 v_razInvariant;
uniform mat4 razInverse;
uniform float srcAlpha;

uniform samplerCube back;
uniform samplerCube src;

#include </data/shader/hband.glsl>
#include </data/shader/fakeSun.glsl>

void main(void)
{
    vec3 stu = normalize(v_ray.xyz);

    vec4 fc = mix(
        texture(back, stu), texture(src, stu), srcAlpha);

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
