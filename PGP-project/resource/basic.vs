#version 430

uniform mat4 mvp;
uniform mat4 mvpDepth;
uniform mat4 mvpDepth2;
uniform mat3 m;

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_texCoord;

out vec3 f_pos;
out vec3 f_norm;
out vec2 f_texCoord;
out vec4 f_depthPos;
out vec4 f_depth2Pos;

void main()
{
	f_pos = m * v_pos;
	f_texCoord = v_texCoord;
	f_norm = v_norm;

	f_depthPos = mvpDepth * vec4(v_pos, 1.0);
	f_depth2Pos = mvpDepth2 * vec4(v_pos, 1.0);
	gl_Position = mvp * vec4(v_pos, 1.0);
}