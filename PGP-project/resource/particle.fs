#version 450

uniform sampler2D texDifSampler;
uniform vec3 ambientLight;
uniform sampler2DShadow texDepthSampler;
uniform sampler2D texDepth2Sampler;
uniform sampler2D texDepth3Sampler;
uniform sampler2D texDepth4Sampler;

in vec2 f_texCoord;
in float f_ttl;
in vec4 f_depthPos;
in vec4 f_depth2Pos;

out vec4 glFragColor;
out vec4 glFragDepth;

void main()
{
	float depthVis = textureProj(texDepthSampler, f_depthPos).r;// texture(texDepthSampler, f_depthPos.xyz / f_depthPos.w).r;
	float depth2Vis = clamp(1.0 - clamp(((f_depth2Pos.z / f_depth2Pos.w) - textureProj(texDepth2Sampler, f_depth2Pos.xyw).r) * 20000.0, 0.0, 1.0) + // rozdíl aktuální hloubky s hloubkou ve SM partiklu
		1.0 - textureProj(texDepth3Sampler, f_depth2Pos).r * 10.0, 0.0, 1.0); // pøiètení k akumulaci

	vec3 ambientReflection = ambientLight;
	vec4 dif = texture(texDifSampler, f_texCoord);

	glFragColor = vec4((depthVis * depth2Vis + ambientReflection) * dif.rgb, dif.a * 0.03);
  glFragDepth = vec4(1.0 - gl_FragCoord.z, 1.0 - gl_FragCoord.z, 1.0 - gl_FragCoord.z, 1.0);
}