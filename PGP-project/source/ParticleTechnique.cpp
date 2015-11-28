#include "ParticleTechnique.h"

void ParticleTechnique::init(Mesh &m, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP)
{
	program = p;
	simulateComputeProgram = simulateComputeP;
	emitComputeProgram = emitComputeP;
	sortComputeProgram = sortComputeP;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(5, vbo);

	indices = m.getSize();

	mvUniform = glGetUniformLocation(program, "mv");
	pUniform = glGetUniformLocation(program, "p");
	texDifSamplerUniform = glGetUniformLocation(program, "texDifSampler");
	dtUniform = glGetUniformLocation(simulateComputeProgram, "dt");
	viewPosUniform = glGetUniformLocation(simulateComputeProgram, "viewPos");

	std::vector<ParticlePool> particlePool;
	particlePool.reserve(indices);
	for (int i = 0; i < indices; i++) {
		particlePool.push_back(ParticlePool(glm::vec4(0.0)));
	}

	GLint attr = glGetAttribLocation(program, "v_pos");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, particlePool.size() * sizeof(ParticlePool), &(particlePool[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(ParticlePool), (GLvoid*)offsetof(ParticlePool, pos));
	glEnableVertexAttribArray(attr);
	/*attr = glGetAttribLocation(program, "v_ttl");
	glVertexAttribPointer(attr, 1, GL_FLOAT, GL_FALSE, sizeof(ParticlePool), (GLvoid*)offsetof(ParticlePool, ttl));
	glEnableVertexAttribArray(attr);*/

	glBindVertexArray(0);

	std::vector<SortList> sortList;
	sortList.reserve(indices);
	for (unsigned int i = 0; i < indices; i++) {
		sortList.push_back(SortList(i, 0.0));
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sortList.size() * sizeof(SortList), &(sortList[0]), GL_STATIC_DRAW);

	std::vector<DeadList> deadList;
	deadList.reserve(indices);
	for (unsigned int i = 0; i < indices; i++) {
		deadList.push_back(DeadList(i));
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, deadList.size() * sizeof(DeadList), &(deadList[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[3]);
	unsigned data[4] = { 0, 0, 0, 0 };
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned) * 4, data, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[4]);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned) * 4, data, GL_DYNAMIC_DRAW);

	glGenTextures(1, &texDif);
	glBindTexture(GL_TEXTURE_2D, texDif);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m.getMaterial()->getDifTex().width, m.getMaterial()->getDifTex().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(m.getMaterial()->getDifTex().tex[0]));
	glGenerateMipmap(GL_TEXTURE_2D);
}

void ParticleTechnique::draw()
{
/*	GLuint *deadCounter;
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[4]);
	// again we map the buffer to userCounters, but this time for read-only access
	deadCounter = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER,
		0,
		sizeof(GLuint),
		GL_MAP_READ_BIT
		);
	// copy the values to other variables because...
	GLuint val = deadCounter[0];
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);*/



	//nulovani sort counteru
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[3]);
	unsigned data[4] = { 0, 0, 0, 0 };
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned)* 4, data);

	//emit
	glUseProgram(emitComputeProgram);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo[2]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, vbo[4]);
	glDispatchCompute(ceil((50000 * dt * 0.001) / 64.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//simulace
	glUseProgram(simulateComputeProgram);
	glUniform1f(dtUniform, dt * 0.001);
	glUniform3f(viewPosUniform, -viewPos.x, -viewPos.y, -viewPos.z);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo[1]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, vbo[3]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, vbo[4]);
	glDispatchCompute(ceil(indices / 64.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//vykresleni
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(program);

	glm::mat4 mv = v * m;

	glUniformMatrix4fv(mvUniform, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, glm::value_ptr(p));
	glUniform1i(texDifSamplerUniform, texDifSampler);

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, indices);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}

void ParticleTechnique::setM(glm::mat4 mat)
{
	m = mat;
}

void ParticleTechnique::setV(glm::mat4 mat)
{
	v = mat;
}

void ParticleTechnique::setP(glm::mat4 mat)
{
	p = mat;
}

void ParticleTechnique::setDt(int t)
{
	dt = t;
}

void ParticleTechnique::setViewPos(glm::vec3 pos)
{
	viewPos = pos;
}

void ParticleTechnique::bindTexDif(int t)
{
	glActiveTexture(GL_TEXTURE0 + t);
	glBindTexture(GL_TEXTURE_2D, texDif);
	texDifSampler = t;
}