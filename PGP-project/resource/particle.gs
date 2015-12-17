#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 mv;
uniform mat4 p;

in float g_ttl[];

out vec2 f_texCoord;
out float f_ttl;

void main()
{
	vec4 pos = mv * gl_in[0].gl_Position;

	gl_Position = p * (pos + vec4(-1.0,-1.0,0.0,0.0) * vec4(0.04)) ;
	f_texCoord = vec2(0.0,0.0);
	f_ttl = g_ttl[0];
	EmitVertex();

	gl_Position = p * (pos + vec4(-1.0,1.0,0.0,0.0) * vec4(0.04));
	f_texCoord = vec2(0.0,1.0);
	f_ttl = g_ttl[0];
	EmitVertex();

	gl_Position = p * (pos + vec4(1.0,-1.0,0.0,0.0) * vec4(0.04));
	f_texCoord = vec2(1.0,0.0);
	f_ttl = g_ttl[0];
	EmitVertex();

	gl_Position = p * (pos + vec4(1.0,1.0,0.0,0.0) * vec4(0.04));
	f_texCoord = vec2(1.0,1.0);
	f_ttl = g_ttl[0];
	EmitVertex();

	EndPrimitive();
}