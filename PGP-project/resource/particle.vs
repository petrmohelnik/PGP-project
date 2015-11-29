#version 450

struct ParticlePool {
	vec4 pos;
};

layout(std430, binding = 0) buffer particleBuffer{
	ParticlePool particle[];
};

in unsigned int particleID;

out float g_ttl;

void main()
{
	vec4 particle = particle[particleID].pos;
	g_ttl = particle.w;
	gl_Position = vec4(particle.xyz, 1.0);
}