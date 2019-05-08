#version 330

layout(location = 1) in vec4 vertex;
layout(location = 0) in vec4 vertexColor;

uniform vec3 sun;

uniform mat4 R; // rgb and alpha for mix
uniform vec4 color; // rgb and alpha for mix

uniform float scattering;
uniform float scintillations;

uniform float glareScale;

uniform float q;
uniform float apparentMagnitude;

uniform sampler1D noise1;

uniform vec4 cmn;

const float PI = 3.1415926535897932384626433832795;
const float _35OVER13PI = 0.85698815511020565414014334123662;


out float v_k;
out vec3 v_color;


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
    vec4 v = vertex * R;
    gl_Position = v;

    v_k = 0;

    if(belowHorizon(v.xyz)) // discard stars below horizon
        return;

    float m = vertexColor.w;
    float m_a = apparentMagnitude;

    float delta_m = pow(2.512, m_a - m);

    float i_t = delta_m * _35OVER13PI;

    i_t *= 4e-7 / (q * q);  // resolution correlated
    i_t = min(1.167, i_t);    // volume of smoothstep (V_T)

    // Day-Twilight-Night-Intensity Mapping (Butterworth-Filter)
    float b = 1.0 / sqrt(1 + pow(sun.z + 1.14, 32));
    i_t *= b;

    if(i_t < 0.01)
        return;

    float i_g = pow(2.512, m_a - (m + 0.167)) - 1;
    vec3 v_t = vec3(i_t);

    float theta = acos(v.z);

    float r = mod(int(cmn[3]) ^ int(vertex.w), 251);
    float sci = 0.02 / texture(noise1, r / 256.0).r;

    vec3 E_ext = scatt(theta);
    E_ext *= (scattering + scintillations * sci);

    v_t -= E_ext;

    v_color = mix(vertexColor.rgb, color.rgb, color.w);
    v_color *= v_t;

    v_color = max(vec3(0.0), v_color);
    v_k = max(q, sqrt(i_g) * 2e-2 * glareScale);
}