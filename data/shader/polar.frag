
uniform sampler2D back;
uniform sampler2D src;
uniform bool isHalf;

const float c_2OverPi  = 0.6366197723675813430755350534901;
const float c_1Over2Pi = 0.1591549430918953357688837633725;
const float c_1OverPi  = 0.3183098861837906715377675267450;

vec4 textureColor(vec3 stu)
{
    // TODO: c_1OverPi was not defined anywhere (typo?)
    float v = isHalf ? (asin(+stu.z) * c_2OverPi) : (acos(-stu.z) * c_1OverPi);
    vec2 uv = vec2(atan(stu.x, stu.y) * c_1Over2Pi, v);

    return mix(texture2D(back, uv), texture2D(src, uv), srcAlpha);
}