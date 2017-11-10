#version 330
#extension GL_EXT_geometry_shader4 : enable

layout (points) in;
layout (triangle_Strip, max_vertices = 4) out;

uniform float q;
uniform mat4 modelViewProjection;

in float v_k[];
in vec3 v_color[];

out vec3 g_color;
out vec3 v_texCoord;


void main()
{
    if(v_k[0] == 0)
        return;

    float k = v_k[0];

    vec3 p = gl_in[0].gl_Position.xyz;

    vec3 u = cross(p, vec3(0, 0, 1));
    vec3 v = cross(u, p);

// used to have const psf function appearance
    v_texCoord.z = k / q;

    g_color = v_color[0];

    gl_Position = modelViewProjection * vec4(p - normalize(-u -v) * k, 1.0);
    v_texCoord.xy = vec2(-1.0, -1.0);
    EmitVertex();

    gl_Position = modelViewProjection * vec4(p - normalize(-u +v) * k, 1.0);
    v_texCoord.xy = vec2(-1.0,  1.0);
    EmitVertex();

    gl_Position = modelViewProjection  * vec4(p - normalize(+u -v) * k, 1.0);
    v_texCoord.xy = vec2( 1.0, -1.0);
    EmitVertex();

    gl_Position = modelViewProjection * vec4(p - normalize(+u +v) * k, 1.0);
    v_texCoord.xy = vec2( 1.0,  1.0);
    EmitVertex();
}