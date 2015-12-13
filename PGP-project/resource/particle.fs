#version 450

uniform sampler2D texDifSampler;

in vec2 f_texCoord;
in float f_ttl;

out vec4 glFragColor;

void main()
{
	glFragColor = texture(texDifSampler, f_texCoord);
}