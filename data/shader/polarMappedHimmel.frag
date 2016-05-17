#version 330

layout(location = 0) out vec4 color;

in vec4 v_ray;
in vec4 v_razInvariant;
uniform mat4 razInverse;
uniform float srcAlpha;

uniform sampler2D back;
uniform sampler2D src;
uniform bool isHalf;

#include "/hband.glsl"
#include "/fakeSun.glsl"

void main(void)
{
    vec3 stu = normalize(v_ray.xyz);

    const float c_2OverPi  = 0.6366197723675813430755350534901;
    const float c_1Over2Pi = 0.1591549430918953357688837633725;
    const float c_1OverPi  = 0.3183098861837906715377675267450;

    // TODO: c_1OverPi was not defined anywhere (typo?)
    float v = isHalf ? (asin(+stu.z) * c_2OverPi) : (acos(-stu.z) * c_1OverPi);
    vec2 uv = vec2(atan(stu.x, stu.y) * c_1Over2Pi, v);

    vec4 fc = mix(texture2D(back, uv), texture2D(src, uv), srcAlpha);

    if (fakeSun)
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
    {
        color = fc;
    }

}
