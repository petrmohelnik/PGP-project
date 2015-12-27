#version 450

uniform vec3 ambientLight;

out vec4 glFragColor;

void main()
{
	glFragColor = vec4(ambientLight, 1.0);
}