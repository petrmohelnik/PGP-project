#version 450

uniform sampler2D texDifSampler;
uniform vec3 lightPos;

in vec2 f_texCoord;

out vec4 glFragColor;

void main()
{
	float scale = (lightPos.z >= 0.0) ? 1.0 : -1.0;
	vec2 lightDir = (f_texCoord - lightPos.xy) * scale;
	vec2 tc = f_texCoord;
	vec2 dtc = lightDir /*/ 50.0*/ * 0.02 * 0.9;
	vec3 c = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < 50; i++)
	{
		tc -= dtc;
		c += texture(texDifSampler, tc).rgb * 0.02;
	}

	glFragColor = vec4(c, lightPos.z);

	// debug flek
	/*if(distance(f_texCoord, lightPos.xy) < 0.005)
		glFragColor = vec4(1.0, 0.0, 0.0, 1.0);*/
}