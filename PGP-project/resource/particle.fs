#version 450

uniform sampler2D texDifSampler;
uniform vec3 ambientLight;
uniform sampler2DShadow texDepthSampler;

in vec2 f_texCoord;
in float f_ttl;
in vec4 f_depthPos;

out vec4 glFragColor;

void main()
{
	float depthVis = textureProj(texDepthSampler, f_depthPos).r;// texture(texDepthSampler, f_depthPos.xyz / f_depthPos.w).r;
	vec3 ambientReflection = ambientLight;
	vec4 dif = texture(texDifSampler, f_texCoord);

	glFragColor = vec4((depthVis + ambientReflection) * dif.rgb, dif.a * 0.1);
}