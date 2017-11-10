#version 330

layout(location = 0) out vec4 fragcolor;

uniform vec4 cmn;

uniform vec3 sun;

// moon.xyz is expected to be normalized and moon.a the moons
// angular diameter in rad.
uniform vec4 moon;

uniform samplerCube moonCube;

uniform sampler1D eclCoeffs;
uniform vec4 eclParams;

uniform float q;
uniform mat4 R;

uniform vec4 sunShine;   // rgb as color and w as intensity.
uniform vec3 earthShine; // rgb as color with premultiplied intensity and scale.

const float radius = 0.98;
const float surface = 1.0;

in vec3 m_eye;
in mat3 m_tangent;
in vec2 m_texCoord;

const float PI               = 3.1415926535897932;
const float PI_05            = 1.5707963267948966;
const float TWO_OVER_THREEPI = 0.2122065907891938;


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


// Hapke-Lommel-Seeliger approximation of the moons reflectance function.

float brdf(
    float cos_r
,   float cos_i
,   float cos_p)
{
// i between incident  (+sun) and surface
// r between reflected (-eye) and surface
// p as i + r

    float g = 0.6; // surface densitiy parameter which determines the sharpness of the peak at the full Moon
    float t = 0.1; // small amount of forward scattering

// The following two params have various fixes applied
    float p = cos_p < 1.0 ? acos(cos_p) : 0.0;
    float p05 = abs(p + step(0, -p) * 0.000001) * 0.5;

    float tan_p = tan(p);

// Retrodirective. - Formular (Hapke66.3)
    float s = 1 - step(PI_05, p);
    float e = - exp(-g / tan_p);
    float B = s * (2 - tan_p / (2 * g) * (1 + e) * (3 + e))  + (1 - s);

// Scattering
//    float S = (sin(p) + (PI - p) * cos_p) / PI
//        + t * (1.0 - cos_p * 0.5) * (1.0 - cos_p * 0.5);

// improved (Hapke66.12) term
    float S = (1 - sin(p05) * tan(p05) * log(tan(PI_05 - p05 * 0.5)))
            + t * (1 - cos_p) * (1 - cos_p);

// BRDF
    float F = TWO_OVER_THREEPI * B * S / (1.0 + cos_r / cos_i);

    if(cos_i > 0)
        return 0.0;
    return (1.0 - step(0, cos_i)) * F;
}



vec3 eclipse(vec3 m, vec3 s, vec3 n, sampler1D e
,   float e0, float e1, float e2, float b)
{
    if(b <= 0)
        return vec3(1);

    float Df = length(cross(m - n * e0, s)) / e0;

    float t;
    if(Df < e1)
        t = Df / (2.0 * e1);
    else
        t = 0.5 + (Df - e1) / (2 * (e2 - e1));

    return texture(e, t).rgb * b;
}



void main(void)
{
    float x = m_texCoord.x;
    float y = m_texCoord.y;


    float zz = 1.0 - x * x - y * y;

    float w  = smoothstep(0.0, 2.828 * q / moon.a, zz); // fov and size indepentent antialiasing
    if(w <= 0.0)
        discard;

    vec3 eye = normalize(m_eye.xyz);

    if(belowHorizon(eye))
        discard;

    vec3 v = vec3(x, y, sqrt(zz));


// horizontal space
    vec3 hn = m_tangent * v;


// apply orientation for correct FrontFacing with optical librations in selenocentric space
    vec3 sn = normalize((vec4(v, 1.0) * R).xyz);


// fetch albedo and normals
    vec4 c  = texture(moonCube, sn);
    vec3 cn = vec3(c.r * 2 - 1, c.g * 2 - 1, c.b);


// convert normals to selenocentric space
    mat3 sm  = mat3(v.zxy, v.yzx, v);
    vec3 s_n = sm * cn;


// convert normals to horizontal space
    vec3 h_n = mix(hn, m_tangent * s_n, surface);


// brdf
    float cos_p = dot(-eye, sun);
    float cos_i = dot( sun, h_n);
    float cos_r = dot(-eye, h_n);

    float f = brdf(cos_r, cos_i, cos_p);


// accuire lunar eclipse coefficients
    vec3 e = eclipse(moon.xyz, sun, hn, eclCoeffs
       , eclParams[0], eclParams[1], eclParams[2], eclParams[3]);


    // Day-Twilight-Night-Intensity Mapping (Butterworth-Filter)
    float b = 0.5 / sqrt(1 + pow(sun.z + 1.05, 32)) + 0.33;


// diffuse
    vec3 diffuse = earthShine;
    diffuse += f * sunShine.w * b;

    diffuse *= c.a;
    diffuse  = max(vec3(0.0), diffuse);

    diffuse *= sunShine.rgb;
    diffuse *= e;

// TODO: make scattering coeff 4 as uniform...
    diffuse *= (1 - 4 * scatt(acos(eye.z)));

    fragcolor = w * vec4(diffuse, 1.0);
}
