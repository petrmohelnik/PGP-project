#version 430

in vec3 v_pos;

out vec2 f_texCoord;

void main()
{
	f_texCoord = (v_pos.xy + 1.0) * 0.5;
	gl_Position = vec4(v_pos, 1.0);
}