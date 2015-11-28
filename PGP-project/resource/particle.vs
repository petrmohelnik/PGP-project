#version 450

in vec4 v_pos;

out float g_ttl;

void main()
{
	g_ttl = v_pos.w;
	gl_Position = vec4(v_pos.xyz, 1.0);
}