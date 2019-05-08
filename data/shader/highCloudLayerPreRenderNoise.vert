#version 330
layout(location = 0) in vec2 vertex;

out vec4 v_ray;
out vec4 v_texCoord;

uniform mat4 modelView;
uniform mat4 inverseProjection;

void main(void)
{
    gl_Position = vec4(vertex, 0.0, 1.0);

    v_ray = inverseProjection * vec4(vertex, 0.0, 1.0) * modelView;
    v_texCoord = vec4(vertex, 0.0, 1.0) * 0.5 + 0.5;
}
