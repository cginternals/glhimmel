#version 150 compatibility
uniform sampler2D preNoise;

uniform float altitude;
uniform vec2 scale;
uniform vec3 color;

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

    return texture2D(preNoise, uv * scale).r;
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
    float n = T(o + t * eye);

    gl_FragColor = vec4(color, n);
}