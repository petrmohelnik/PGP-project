#version 430

uniform sampler2D texDifSampler;

in vec2 f_texCoord;
out vec4 glFragColor;

void main()
{
	glFragColor = texture(texDifSampler, f_texCoord);
}