#version 450

uniform sampler2D texDifSampler;

out vec4 glFragColor;

void main()
{
	glFragColor = vec4(texture(texDifSampler, gl_FragCoord.xy).rgb, 1.0);
}