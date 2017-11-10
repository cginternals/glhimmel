#version 150 compatibility
uniform sampler3D noise0;
uniform sampler3D noise1;
uniform sampler3D noise2;
uniform sampler3D noise3;

uniform float noiseSize;
uniform float time;

uniform float coverage;
uniform float sharpness;

uniform float change;
uniform vec2  wind;


void main()
{
   vec2 uv = gl_FragCoord.xy / noiseSize;
   float n = 0;

   float t = time * 3600.0;

   vec2  m = t * wind;
   t *= change;

   n += 1.00000 * texture3D(noise0, vec3(uv     + m * 0.18, t * 0.01)).r;
   n += 0.50000 * texture3D(noise1, vec3(uv     + m * 0.16, t * 0.02)).r;
   n += 0.25000 * texture3D(noise2, vec3(uv     + m * 0.14, t * 0.04)).r;
   n += 0.12500 * texture3D(noise3, vec3(uv     + m * 0.12, t * 0.08)).r;
   n += 0.06750 * texture3D(noise3, vec3(uv * 2 + m * 0.10, t * 0.16)).r;
//   n += 0.06125 * texture3D(noise3, vec3(uv * 4 + m * 0.08, t * 0.32)).r;
//           n += 0.06125 * texture3D(noise2, vec3(uv * 8 + m * 0.06, t * 0.64)).r;
   n *= 0.76;

   n = n - 1 + coverage;
   n /= coverage;
   n = max(0.0, n);
   n = pow(n, 1.0 - sharpness);

   gl_FragColor = vec4(n);
}