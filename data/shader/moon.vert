#version 330

layout(location = 0) in vec2 vertex;

// moonr.xyz is expected to be normalized and moonr.a the moons
// angular diameter in rad.
uniform mat4 modelViewProjection;
uniform vec4 moonr;

out mat3 m_tangent;
out vec3 m_eye;
out vec2 m_texCoord;

void main(void)
{
    vec3 m = moonr.xyz;

    vec3 u = normalize(cross(vec3(0, 0, 1), m));
    vec3 v = normalize(cross(m, u));
    m_tangent = mat3(u, v, m);

    m_eye = m - (vertex.x * u + vertex.y * v) * moonr.a;

    m_texCoord = vertex;
    gl_Position = modelViewProjection * vec4(m_eye, 1.0);
}