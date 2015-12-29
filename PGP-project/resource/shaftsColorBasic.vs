#version 430

uniform mat4 mvp;

in vec3 v_pos;

void main()
{
	gl_Position = mvp * vec4(v_pos, 1.0);
}