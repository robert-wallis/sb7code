#version 410 core

layout (location = 0) in vec4 offset;
layout (location = 1) in vec4 color;

out VS_OUT {
	vec4 color;
} vs_out;

void main()
{
    const vec4 verts[] = vec4[] (
        vec4( 0.62, -0.62, 0.5, 1.0),
        vec4(-0.62, -0.62, 0.5, 1.0),
        vec4( 0.62,  0.62, 0.5, 1.0)
    );
    gl_Position = verts[gl_VertexID] + offset;
	vs_out.color = color;
}