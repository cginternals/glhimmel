#version 330
layout(location = 0) in vec2 vertex;

// moon.xyz is expected to be normalized and moon.a the moons
// angular diameter in rad.
uniform mat4 modelViewProjection;
uniform vec4 moonr;

uniform float scale;
const float SQRT2 = 1.41421356237;

out vec2 v_texCoord;

void main(void)
{
    vec3 m = moonr.xyz;

//  tangent space of the unitsphere at m.
    vec3 u = normalize(cross(vec3(0, 0, 1), m));
    vec3 v = normalize(cross(m, u));

    float mScale = tan(moonr.a) * SQRT2 * scale;

    vec3 eye = m - normalize(vertex.x * u + vertex.y * v) * mScale;

    v_texCoord = vertex;
    gl_Position = modelViewProjection * vec4(eye, 1.0);
}