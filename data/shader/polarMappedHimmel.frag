#version 330

vec4 blend_normal (vec4 back, vec4 src, float srca)\n"         \
{\n"                                                             \
    vec3 b = back.rgb;\n"                                        \
    vec3 s = src.rgb;\n"                                         \
    \n"                                                              \
    vec3 bs = s;\n"                                   \
    \n"                                                              \
    float ba = back.a;\n"                                        \
    float sa = clamp(srca, 0.0, 1.0);\n"                         \
    \n"                                                              \
    float ra = ba + sa - (ba * sa);\n"                           \
    \n"                                                              \
    vec3 r = (1.0 - sa / ra) * b + (sa / ra) * ((1.0 - ba) * s + ba * bs);\n" \
    \n"                                                              \
    return vec4(r, ra);\n"                                       \
}

in vec4 m_razInvariant;
uniform mat4 razInverse;
uniform vec3 sun;
uniform vec4 sunCoeffs;
uniform float sunScale;

vec4 fakeSun(
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

uniform vec3 hbandParams;
uniform vec4 hbandBackground;
uniform vec4 hbandColor;

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
    
PRAGMA_ONCE(main, 

ENABLE_IF(half, getMappingMode() == MM_Half)

ENABLE_IF(hBand, m_hBand)
ENABLE_IF(fakeSun, m_fakeSun)


in vec4 m_ray;


// From AbstractMappedHimmel

uniform float srcAlpha;
uniform sampler2D back;
uniform sampler2D src;


// Color Retrieval

const float c_2OverPi  = 0.6366197723675813430755350534901;
const float c_1Over2Pi = 0.1591549430918953357688837633725;

void main(void)
{
    vec3 stu = normalize(m_ray.xyz);

IF_ELSE_ENABLED(half,
    vec2 uv = vec2(atan(stu.x, stu.y) * c_1Over2Pi, asin(+stu.z) * c_2OverPi);"
,
    vec2 uv = vec2(atan(stu.x, stu.y) * c_1Over2Pi, acos(-stu.z) * c_1OverPi);")

    vec4 fc = mix(texture2D(back, uv), texture2D(src, uv), srcAlpha);


IF_ENABLED(fakeSun,

    fc += fakeSun(
        normalize(m_razInvariant.xyz)
    ,   sun
    ,   sunCoeffs
    ,   sunScale
    ,   fc.a);")

IF_ELSE_ENABLED(hBand,

    gl_FragColor = hband(
        stu.z
    ,   hbandParams[0]
    ,   hbandParams[1]
    ,   hbandParams[2]
    ,   hbandColor
    ,   hbandBackground
    ,   fc);"
,
    gl_FragColor = fc;")

}