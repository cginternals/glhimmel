#version 330
#extension GL_ARB_shading_language_include : require

layout(location = 0) out vec4 fragcolor;

uniform vec3 sun;

uniform vec4 color;
uniform float q;

uniform float deltaM;
uniform float scattering;

uniform samplerCube starmapCube;

in vec4 m_eye;
in vec4 m_ray;

uniform vec4 cmn;

float tAtm(vec3 ray)
{
    float r = cmn[1] + cmn[0];
    vec3 x = vec3(0.0, 0.0, r);

    vec3 v = normalize(ray);

    float mu = dot(x, v) / r;

    return r * mu - sqrt(r * r * (mu * mu - 1.0) + cmn[1] * cmn[1]);
}

bool belowHorizon(vec3 ray)
{
    if(ray.z > 0.0)
        return false;

    return tAtm(ray) < 0.0;
}


// coefficients are taken from calculateions by means of Bucholtz 94
// -> Scattering coefficient for air molecules (Rayleigh theory) for wavelengths (680, 550, 440) nm

const vec3 lambda = vec3(0.52, 1.22, 2.98);


float optical(const float theta)
{
    float sin_theta = sin(theta);

    float s = -sin(asin((cmn[1] + cmn[0]) / cmn[2] * sin_theta) - theta);
    s *= cmn[2];
    s /= sin_theta;
    s /= cmn[2] - cmn[0];

    return s;
}


// theta is the angle between ray and zenith ~ probably acos(ray.z)
vec3 scatt(const float theta)
{
    return lambda * optical(theta);
}


void main(void)
{
    vec3 eye = normalize(m_eye.xyz);

    if(belowHorizon(eye))
        discard;

    vec3 stu = normalize(m_ray.xyz);

    vec4 fc = texture(starmapCube, stu);
    fc *= 3e-2 / sqrt(q) * deltaM;

    float omega = acos(eye.z * 0.9998);

    // Day-Twilight-Night-Intensity Mapping (Butterworth-Filter)
    float b = 1.0 / sqrt(1 + pow(sun.z + 1.14, 32));

    fragcolor = vec4(b * (fc.rgb - scatt(omega)), 1.0);
}
