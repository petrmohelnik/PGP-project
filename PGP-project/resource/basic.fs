#version 450

uniform mat3 ti_m;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 ambientLight;
uniform sampler2D texDifSampler;
uniform sampler2DShadow texDepthSampler;
uniform sampler2DShadow texDepth2Sampler;
uniform sampler2D texDepth3Sampler;
uniform sampler2D texDepth4Sampler;

in vec3 f_pos;
in vec3 f_norm;
in vec2 f_texCoord;
in vec4 f_depthPos;
in vec4 f_depth2Pos;

out vec4 glFragColor;

void main()
{
	//vec3 color = vec3(0.7, 0.7, 0.7);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	float shininess = 100.0;

	vec3 viewDir = normalize(viewPos - f_pos);
	vec3 lightDir= normalize(lightPos - f_pos);
	vec3 normal = normalize(ti_m * f_norm);

	vec3 ambientReflection = ambientLight;

	vec3 diffuseReflection = lightColor * max(0.0, dot(normal, lightDir));

	float depthVis = textureProj(texDepthSampler, f_depthPos).r;// texture(texDepthSampler, f_depthPos.xyz / f_depthPos.w).r;
	float depth2Vis = textureProj(texDepth2Sampler, f_depth2Pos).r; // kombinace zastínìní

	if(depth2Vis < 1.0)
	{
    //const float diff = clamp((((f_depth2Pos.z) / f_depth2Pos.w) - (1.0 - textureProj(texDepth4Sampler, f_depth2Pos.xyw).r)) * 10000.0, 0.0, 1.0);
		depth2Vis += 1.0 - (clamp(textureProj(texDepth3Sampler, f_depth2Pos).r * 10.0, 0.0, 1.0)/* * diff*/);
		depth2Vis = clamp(depth2Vis, 0.0, 1.0);
	}

	vec3 specularReflection;
	if ((dot(normal, lightDir) < 0.0) || (depthVis == 0.0)) // light source on the wrong side?
	{
		specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
	}
	else
	{
		specularReflection = lightColor * pow(max(0.0, dot(reflect(-lightDir, normal), viewDir)), shininess);
	}

	glFragColor = vec4(((diffuseReflection + specularReflection) * depthVis * depth2Vis + ambientReflection) * texture(texDifSampler, f_texCoord).rgb, 1.0);
}