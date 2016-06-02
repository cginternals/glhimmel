#version 330
#extension GL_ARB_shading_language_include : require

#include </data/shader/dither.glsl>
#inculde </data/shader/bruneton.glsl>

uniform vec4 cmn
        
uniform vec3 sun;
uniform vec3 sunr;

in vec4 v_ray;

const float ISun = 100.0;

//uniform vec3 c = vec3(cmn[1] + 0.2, 0.0, 0.0);
uniform float sunScale;
uniform float exposure;

uniform vec3 lheurebleueColor; 
uniform float lheurebleueIntensity;

//uniform sampler2D reflectanceSampler; // ground reflectance texture
uniform sampler2D irradianceSampler;    // precomputed skylight irradiance (E table)
uniform sampler3D inscatterSampler;     // precomputed inscattered light (S table)
     
// inscattered light along ray x+tv, when sun in direction s (=S[L]-T(x,x0)S[L]|x0)
vec3 inscatter(inout vec3 x, inout float t, vec3 v, vec3 s, out float r, out float mu, out vec3 attenuation) {
    vec3 result;
    r = length(x);
    mu = dot(x, v) / r;
    float d = -r * mu - sqrt(r * r * (mu * mu - 1.0) + cmn[2] * cmn[2]);
    if (d > 0.0) { // if x in space and ray intersects atmosphere
        // move x to nearest intersection of ray with top atmosphere boundary
        x += d * v;
        t -= d;
        mu = (r * mu + d) / cmn[2];
        r = cmn[2];
    }
    if (r <= cmn[2]) { // if ray intersects atmosphere
        float nu = dot(v, s);
        float muS = dot(x, s) / r;
        float phaseR = phaseFunctionR(nu);
        float phaseM = phaseFunctionM(nu);
        vec4 inscatter = max(texture4D(inscatterSampler, r, mu, muS, nu), 0.0);
        if (t > 0.0) {
            vec3 x0 = x + t * v;
            float r0 = length(x0);
            float rMu0 = dot(x0, v);
            float mu0 = rMu0 / r0;
            float muS0 = dot(x0, s) / r0;
//#ifdef FIX
            // avoids imprecision problems in transmittance computations based on textures
            // attenuation = analyticTransmittance(r, mu, t);
//#else
            attenuation = transmittance(r, mu, v, x0);
//#endif
            if (r0 > cmn[1] + 0.01) {
                // computes S[L]-T(x,x0)S[L]|x0
                inscatter = max(inscatter - attenuation.rgbr * texture4D(inscatterSampler, r0, mu0, muS0, nu), 0.0);
//#ifdef FIX
/*              // avoids imprecision problems near horizon by interpolating between two points above and below horizon
                const float EPS = 0.004;
                float muHoriz = -sqrt(1.0 - (cmn[1] / r) * (cmn[1] / r));
                if (abs(mu - muHoriz) < EPS) {
                    float a = ((mu - muHoriz) + EPS) / (2.0 * EPS);

                    mu = muHoriz - EPS;
                    r0 = sqrt(r * r + t * t + 2.0 * r * t * mu);
                    mu0 = (r * mu + t) / r0;
                    vec4 inScatter0 = texture4D(inscatterSampler, r, mu, muS, nu);
                    vec4 inScatter1 = texture4D(inscatterSampler, r0, mu0, muS0, nu);
                    vec4 inScatterA = max(inScatter0 - attenuation.rgbr * inScatter1, 0.0);

                    mu = muHoriz + EPS;
                    r0 = sqrt(r * r + t * t + 2.0 * r * t * mu);
                    mu0 = (r * mu + t) / r0;
                    inScatter0 = texture4D(inscatterSampler, r, mu, muS, nu);
                    inScatter1 = texture4D(inscatterSampler, r0, mu0, muS0, nu);
                    vec4 inScatterB = max(inScatter0 - attenuation.rgbr * inScatter1, 0.0);

                    inscatter = mix(inScatterA, inScatterB, a);
                }
//#endif
*/
            }
        }
//#ifdef FIX
        // avoids imprecision problems in Mie scattering when sun is below horizon
        inscatter.w *= smoothstep(0.00, 0.02, muS);
//#endif
        result = max(inscatter.rgb * phaseR + getMie(inscatter) * phaseM, 0.0);
    } else { // x in space and ray looking in space
        result = vec3(0.0);
    }
    return result * ISun;
}

// ground radiance at end of ray x+tv, when sun in direction s
// attenuated bewteen ground and viewer (=R[L0]+R[L*])

/*        
vec3 groundColor(vec3 x, float t, vec3 v, vec3 s, float r, float mu, vec3 attenuation)
{
    vec3 result;
    if (t > 0.0) { // if ray hits ground surface
        // ground reflectance at end of ray, x0
        vec3 x0 = x + t * v;
        float r0 = length(x0);
        vec3 n = x0 / r0;
        // vec2 coords = vec2(atan(n.y, n.x), acos(n.z)) * vec2(0.5, 1.0) / PI + vec2(0.5, 0.0);
        // vec4 reflectance = texture2D(reflectanceSampler, coords) * vec4(0.2, 0.2, 0.2, 1.0);
        vec4 reflectance = vec4(0.0, 0.0, 0.0, 1.0);
        if (r0 > cmn[1] + 0.01) {
            reflectance = vec4(0.4, 0.4, 0.4, 0.0);
        }

        // direct sun light (radiance) reaching x0
        float muS = dot(n, s);
        vec3 sunLight = transmittanceWithShadow(r0, muS);

        // precomputed sky light (irradiance) (=E[L*]) at x0
        vec3 groundSkyLight = irradiance(irradianceSampler, r0, muS);

        // light reflected at x0 (=(R[L0] + R[L*]) / T(x, x0))
        vec3 groundColor = reflectance.rgb * (max(muS, 0.0) * sunLight + groundSkyLight) * ISun / PI;

        // // water specular color due to sunLight
        // if (reflectance.w > 0.0) {
        //     vec3 h = normalize(s - v);
        //     float fresnel = 0.02 + 0.98 * pow(1.0 - dot(-v, h), 5.0);
        //     float waterBrdf = fresnel * pow(max(dot(h, n), 0.0), 150.0);
        //     groundColor += reflectance.w * max(waterBrdf, 0.0) * sunLight * ISun;
        // }
        //
        result = attenuation * groundColor; // = R[L0] + R[L*]
    } else { // ray looking at the sky
        result = vec3(0.0);
    }
    return result;
}
*/

// direct sun light for ray x+tv, when sun in direction s (=L0)
vec3 sunColor(vec3 x, float t, vec3 v, vec3 s, float r, float mu) {
    if (t > 0.0) {
        return vec3(0.0);
    } else {
        vec3 transmittance = r <= cmn[2] ? transmittanceWithShadow(r, mu) : vec3(1.0); // T(x, xo)
        float isun = step(cos(sunScale), dot(v, s)) * ISun; // Lsun
        return transmittance * isun; // Eq (9)
    }
}


void main() {
    vec3 x = vec3(0.0, 0.0, cmn[1] + cmn[0]);
    vec3 ray = normalize(m_ray.xyz);

    float r = length(x);
    float mu = dot(x, ray) / r;
    float t = -r * mu - sqrt(r * r * (mu * mu - 1.0) + cmn[1] * cmn[1]);
    //
    // vec3 g = x - vec3(0.0, 0.0, cmn[1] + 10.0);
    // float a = ray.x * ray.x + ray.y * ray.y - ray.z * ray.z;
    // float b = 2.0 * (g.x * ray.x + g.y * ray.y - g.z * ray.z);
    // float c = g.x * g.x + g.y * g.y - g.z * g.z;
    // float d = -(b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
    // bool cone = d > 0.0 && abs(x.z + d * ray.z - cmn[1]) <= 10.0;
    //
    // if (t > 0.0) {
    //     if (cone && d < t) {
    //         t = d;
    //     }
    // } else if (cone) {
    //     t = d;
    // }
    //
    vec3 attenuation;
    vec3 inscatterColor = inscatter(x, t, ray, sunr, r, mu, attenuation); // S[L]  - T(x, xs) S[l] | xs"
    // vec3 groundColor = groundColor(x, t, ray, sun, r, mu, attenuation);  // R[L0] + R[L*]
    vec3 sunColor = sunColor(x, t, ray, sunr, r, mu); // L0


    // l'heure bleue (blaue stunde des ozons)

    // gauss between -12° and +0° sun altitude (Civil & Nautical twilight) 
    // http://en.wikipedia.org/wiki/Twilight
    float hb = t > 0.0 ? 0.0 : exp(-pow(sunr.z, 2.0) * 166) + 0.03;     // the +0.03 is for a slight blueish tint at night
    vec3 bluehour = lheurebleueIntensity * lheurebleueColor * (dot(ray, sunr) + 1.5) * hb; // * mu (optional..)

    gl_FragColor = vec4(HDR(bluehour + sunColor /*+ groundColor*/ + inscatterColor), 1.0)    // Eq (16)
        + dither(3, int(cmn[3]));
}