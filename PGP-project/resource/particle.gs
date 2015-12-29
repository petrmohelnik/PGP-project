#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 mv;
uniform mat4 p;
uniform mat4 mvpDepth;
uniform mat4 mvpDepth2;

in float g_ttl[];

out vec2 f_texCoord;
out float f_ttl;
out vec4 f_depthPos;
out vec4 f_depth2Pos;

void main()
{
	vec4 pos = mv * gl_in[0].gl_Position;
	const vec4 scale = vec4(0.1);

	gl_Position = p * (pos + vec4(-1.0,-1.0,0.0,0.0) * scale);
	f_texCoord = vec2(0.0,0.0);
	f_ttl = g_ttl[0];
	f_depthPos = mvpDepth * vec4(gl_in[0].gl_Position.xyz, 1.0);
	f_depth2Pos = mvpDepth2 * vec4(gl_in[0].gl_Position.xyz, 1.0);
	EmitVertex();

	gl_Position = p * (pos + vec4(-1.0,1.0,0.0,0.0) * scale);
	f_texCoord = vec2(0.0,1.0);
	f_ttl = g_ttl[0];
	f_depthPos = mvpDepth * vec4(gl_in[0].gl_Position.xyz, 1.0);
	f_depth2Pos = mvpDepth2 * vec4(gl_in[0].gl_Position.xyz, 1.0);
	EmitVertex();

	gl_Position = p * (pos + vec4(1.0,-1.0,0.0,0.0) * scale);
	f_texCoord = vec2(1.0,0.0);
	f_ttl = g_ttl[0];
	f_depthPos = mvpDepth * vec4(gl_in[0].gl_Position.xyz, 1.0);
	f_depth2Pos = mvpDepth2 * vec4(gl_in[0].gl_Position.xyz, 1.0);
	EmitVertex();

	gl_Position = p * (pos + vec4(1.0,1.0,0.0,0.0) * scale);
	f_texCoord = vec2(1.0,1.0);
	f_ttl = g_ttl[0];
	f_depthPos = mvpDepth * vec4(gl_in[0].gl_Position.xyz, 1.0);
	f_depth2Pos = mvpDepth2 * vec4(gl_in[0].gl_Position.xyz, 1.0);
	EmitVertex();

	EndPrimitive();
}