#version 450

uniform sampler2D texDifSampler;
uniform vec3 ambientLight;

in vec2 f_texCoord;
in float f_ttl;

out vec4 glFragColor;

void main()
{
	vec4 dif = texture(texDifSampler, f_texCoord);

	glFragColor = vec4(ambientLight, dif.a * 0.03);
}