#version 330

layout(location = 0) in vec2 vertex;

uniform mat4 razInverse;
uniform mat4 modelView;
uniform mat4 inverseProjection;

out vec4 v_ray;
out vec4 v_razInvariant;

void main()
{
    gl_Position = vec4(vertex, 0.0, 1.0);
    v_ray = inverseProjection * vec4(vertex, 0.0, 1.0) * modelView;
    v_razInvariant = v_ray * razInverse;
}
