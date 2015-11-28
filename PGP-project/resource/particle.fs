#version 450

uniform sampler2D texDifSampler;

in vec2 f_texCoord;
in float f_ttl;

out vec4 gl_FragColor;

void main()
{
	gl_FragColor = texture(texDifSampler, f_texCoord);
}