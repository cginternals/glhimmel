#version 150 compatibility

uniform sampler2D clouds;

uniform float altitude;
uniform float offset;
uniform float thickness;

uniform vec2 scale;

uniform vec3 tcolor;
uniform vec3 bcolor;

const float sSteps = 128;
const float sRange =  8;

const int dSteps = 32;
const float dRange = 1.9;

uniform vec3 sunr;

uniform vec4 cmn;

in vec4 v_ray;
in vec4 v_texCoord;


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

bool layerIntersection(
    const vec3 d
,   const vec3 o
,   const float altitude
,   out float t)
{
    float r = cmn[1] + altitude;

    // for now, ignore if altitude is above cloud layer
    if(o.z > r)
        return false;

    float a = dot(d, d);
    float b = 2 * dot(d, o);
    float c = dot(o, o) - r * r;

    float B = b * b - 4 * a * c;
    B = sqrt(B);

    float q;

    if(b < 0)
        q = (-b - B) * 0.5;
    else
        q = (-b + B) * 0.5;

    float t0 = q / a;
    float t1 = c / q;

    if(t0 > t1)
    {
        q  = t0;
        t0 = t1;
        t1 = q ;
    }

    if(t1 < 0)
        return false;

    t = t0 < 0 ? t1 : t0;
    return true;
}


float T(vec3 stu)
{
    float m = 2.0 * (1.0 + stu.z);
    vec2 uv = vec2(stu.x / m + 0.5, stu.y / m + 0.5);

    return texture2D(clouds, uv * scale).r;
}

float density(
    in vec3 stu0
,   in vec3 sun
,   in float aa0)
{
    vec3 stu1 = stu0 + sun * dRange * thickness;

    float iSteps = 1.0 / (dSteps - 1);
    float iRange = dRange * thickness * iSteps;

    vec3 Dstu = (stu1 - stu0) * iSteps;
    vec3 stu  = stu0;

    float d = 0.0;
    float a1 = thickness + offset;

    for(int i = 0; i < dSteps; ++i)
    {
        float t = T(stu);

        float a = aa0 + i * iRange;

        if(a > t * offset
        && a < t * a1)
            d += iSteps;

        stu += Dstu;
    }

    return d;
}

float scatter(
    in vec3 eye
,   in vec3 sun
,   out float d)
{
    vec3 o0 = vec3(0, 0, cmn[1] + cmn[0]);

    // check if intersects with lower cloud sphere
    float t0, t1;

    float a1 = thickness + offset;

    if(!layerIntersection(eye, o0, altitude + offset, t0))
        return 0.f;

    if(!layerIntersection(eye, o0, altitude + a1, t1))
        return 0.f;

    // start t0 and end t1 through atm

    d = 0.0;

    vec3 stu0 = o0 + t0 * eye;
    vec3 stu1 = o0 + t1 * eye;

    float iSteps = 1.0 / (sSteps - 1);

    vec3 Dstu = (stu1 - stu0) / (sSteps - 1);
    vec3 stu  = stu0;

    float s = 0.0;
    float Da = thickness * iSteps;

    for(int i = 0; i < sSteps; ++i)
    {
        float t = T(stu);

        float a = offset + i * Da;

        if(a > t * offset
        && a < t * a1)
        {
            s += density(stu, sun, a);
            ++d;
        }
        if(d >= sRange)
            break;

        stu += Dstu;
    }
    s /= sRange;
    d /= sRange;

    return s;
}

void main()
{
    vec3 eye = normalize(v_ray.xyz);

    if(belowHorizon(eye))
        discard;

    vec3 sun = normalize(sunr);

    float t;
    vec3 o = vec3(0, 0, cmn[1] + cmn[0]);
    layerIntersection(eye, o, altitude, t);

    float d;
    float s = scatter(eye, sun, d);
    float f = 1.0 - s;

    d *= (1.0 - pow(t, 0.8) * 0.012);

    gl_FragColor = vec4(mix(tcolor, bcolor, s) * f, d);
}