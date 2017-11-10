#version 330

layout(location = 0) in vec2 vertex;

uniform mat4 modelView;
uniform mat4 inverseProjection;

out vec4 m_eye;
out vec4 m_ray;

uniform mat4 R;

void main(void)
{
    gl_Position = vec4(vertex, 0, 1);
    m_eye = inverseProjection * vec4(vertex, 0, 1) * modelView;
    m_ray = R * m_eye;
}