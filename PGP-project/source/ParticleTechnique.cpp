#include "ParticleTechnique.h"

void ParticleTechnique::init(Mesh &m, int count, GLuint p, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortComputeP, GLuint sortLocalComputeP, 
	GLuint sortLocalInnerComputeP, GLuint gridDivideComputeP, GLuint gridFindStartComputeP,
	GLuint simulateDensityComputeP, GLuint simulatePressureComputeP, GLuint simulateForceComputeP)
{
	program = p;
	simulateComputeProgram = simulateComputeP;
	emitComputeProgram = emitComputeP;
	sortComputeProgram = sortComputeP;
	sortLocalComputeProgram = sortLocalComputeP;
	sortLocalInnerComputeProgram = sortLocalInnerComputeP;
	gridDivideComputeProgram = gridDivideComputeP;
	gridFindStartComputeProgram = gridFindStartComputeP;
	simulateDensityComputeProgram = simulateDensityComputeP;
	simulatePressureComputeProgram = simulatePressureComputeP;
	simulateForceComputeProgram = simulateForceComputeP;

	glGenVertexArrays(1, &vao);
	glGenBuffers(8, vbo);
	indices = count;

	mvUniform = glGetUniformLocation(program, "mv");
	pUniform = glGetUniformLocation(program, "p");
	texDifSamplerUniform = glGetUniformLocation(program, "texDifSampler");
	dtUniform = glGetUniformLocation(simulateComputeProgram, "dt");
	halfVectorUniform = glGetUniformLocation(simulateComputeProgram, "halfVector");
	maxParticlesUniform = glGetUniformLocation(simulateComputeProgram, "maxParticles");
	hGridSimulateUniform = glGetUniformLocation(simulateComputeProgram, "h");
	gridMaxIndexUniform = glGetUniformLocation(simulateComputeProgram, "gridMaxIndex");
	gridSizeSimulateUniform = glGetUniformLocation(simulateComputeProgram, "gridSize");
	maxEmitUniform = glGetUniformLocation(emitComputeProgram, "maxEmit");
	maxSortUniform = glGetUniformLocation(sortComputeProgram, "numParticles");
	compareDistUniform = glGetUniformLocation(sortComputeProgram, "compareDist");
	subArraySizeUniform = glGetUniformLocation(sortComputeProgram, "subArraySize");
	degreeSortUniform = glGetUniformLocation(sortComputeProgram, "degree");
	subArraySizeLocalInnerUniform = glGetUniformLocation(sortLocalInnerComputeProgram, "subArraySize");
	maxSortLocalUniform = glGetUniformLocation(sortLocalComputeProgram, "numParticles");
	maxSortLocalInnerUniform = glGetUniformLocation(sortLocalInnerComputeProgram, "numParticles");
	maxParticlesGridDivideUniform = glGetUniformLocation(gridDivideComputeProgram, "maxParticles");
	sizeGridDivideUniform = glGetUniformLocation(gridDivideComputeProgram, "size");
	hGridDivideUniform = glGetUniformLocation(gridDivideComputeProgram, "h");
	maxParticlesDensityUniform = glGetUniformLocation(simulateDensityComputeProgram, "maxParticles");
	hGridSimulateDensityUniform = glGetUniformLocation(simulateDensityComputeProgram, "hGridSimulate");
	gridMaxIndexDensityUniform = glGetUniformLocation(simulateDensityComputeProgram, "gridMaxIndex");
	gridSizeSimulateDensityUniform = glGetUniformLocation(simulateDensityComputeProgram, "gridSize");
	massSimulateDensityUniform = glGetUniformLocation(simulateDensityComputeProgram, "mass");
	gasConstantPressureUniform = glGetUniformLocation(simulatePressureComputeProgram, "gasConstant");
	restDensityPressureUniform = glGetUniformLocation(simulatePressureComputeProgram, "restDensity");
	maxParticlesForceUniform = glGetUniformLocation(simulateForceComputeProgram, "maxParticles");
	hGridSimulateForceUniform = glGetUniformLocation(simulateForceComputeProgram, "hGridSimulate");
	gridMaxIndexForceUniform = glGetUniformLocation(simulateForceComputeProgram, "gridMaxIndex");
	gridSizeSimulateForceUniform = glGetUniformLocation(simulateForceComputeProgram, "gridSize");
	massSimulateForceUniform = glGetUniformLocation(simulateForceComputeProgram, "mass");

	std::vector<ParticlePool> particlePool;
	particlePool.reserve(indices);
	for (int i = 0; i < indices; i++) {
		particlePool.push_back(ParticlePool(glm::vec4((((double)rand()) / RAND_MAX) * 4 - 2, (((double)rand()) / RAND_MAX) * 4, (((double)rand()) / RAND_MAX) * 4 - 2, -1.0),
			glm::vec4(0), glm::vec4(0)));
	}

	//obsahuje castice - jejich pozici a cas zivota
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, particlePool.size() * sizeof(ParticlePool), &(particlePool[0]), GL_STATIC_DRAW);

	std::vector<SortList> sortList;
	sortList.reserve(indices);
	for (unsigned int i = 0; i < indices; i++) {
		sortList.push_back(SortList(i, 0.0));
	}
	glBindVertexArray(vao);
	GLint attr = glGetAttribLocation(program, "particleID");
	//tady budou ulozeny castice, ktere sou nazivu a budou se radit a zobrazovat
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sortList.size() * sizeof(SortList), &(sortList[0]), GL_STATIC_DRAW);
	glVertexAttribIPointer(attr, 1, GL_UNSIGNED_INT, sizeof(SortList), (GLvoid*)offsetof(SortList, index));
	glEnableVertexAttribArray(attr);
	glBindVertexArray(0);

	std::vector<DeadList> deadList;
	deadList.reserve(indices);
	for (unsigned int i = 0; i < indices; i++) {
		deadList.push_back(DeadList(i));
	}
	//mrtve castice, z nich se potom bere pri emitu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, deadList.size() * sizeof(DeadList), &(deadList[0]), GL_STATIC_DRAW);

	//pozice konce sort bufferu
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[3]);
	unsigned data[4] = { 0, 0, 0, 0 };
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned) * 4, data, GL_DYNAMIC_DRAW);

	//pozice v dead bufferu, vpravo od ni jsou mrtve castice
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[4]);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned) * 4, data, GL_DYNAMIC_DRAW);

	std::vector<GridList> gridList;
	gridList.reserve(indices);
	for (unsigned int i = 0; i < indices; i++) {
		gridList.push_back(GridList(0, 0));
	}
	//rozrazeni castic do mrizky
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, gridList.size() * sizeof(GridList), &(gridList[0]), GL_STATIC_DRAW);

	std::vector<StartIndexList> startIndexList;
	startIndexList.reserve(pow((GRID_SIZE / GRID_H), 3));
	for (unsigned int i = 0; i < pow((GRID_SIZE / GRID_H), 3); i++) {
		startIndexList.push_back(StartIndexList(0));
	}
	//pocatecni indexy v serazenem poli s indexy do mrizky
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, startIndexList.size() * sizeof(StartIndexList), &(startIndexList[0]), GL_STATIC_DRAW);

	//pozice v gridList, kolik je tam castic a kolik se jich bude radit
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[7]);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned)* 4, data, GL_DYNAMIC_DRAW);

	glGenTextures(1, &texDif);
	glBindTexture(GL_TEXTURE_2D, texDif);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m.getMaterial()->getDifTex().width, m.getMaterial()->getDifTex().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(m.getMaterial()->getDifTex().tex[0]));
	glGenerateMipmap(GL_TEXTURE_2D);
}

void ParticleTechnique::sort(GLuint sortCounter, GLuint buffer)
{
	glUseProgram(sortLocalComputeProgram);
	glUniform1ui(maxSortLocalUniform, sortCounter);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
	glDispatchCompute(ceil(sortCounter / 512.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	unsigned int closestPowOfTwo = 1;
	while (closestPowOfTwo < sortCounter)
		closestPowOfTwo *= 2;

	for (unsigned int subArraySize = 1024, pow1 = 10; subArraySize <= closestPowOfTwo; subArraySize *= 2, pow1++) {
		for (unsigned int compareDist = subArraySize / 2, pow2 = pow1 - 1; compareDist > 512; compareDist /= 2, pow2--) {
			unsigned int deg = pow2 - 9;
			if (deg > 4)
				deg = deg == 5 ? 3 : 4;
			glUseProgram(sortComputeProgram);
			glUniform1ui(maxSortUniform, sortCounter);
			glUniform1ui(compareDistUniform, compareDist);
			glUniform1ui(subArraySizeUniform, subArraySize);
			glUniform1ui(degreeSortUniform, deg);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
			unsigned int size = pow(2, deg);
			glDispatchCompute(ceil(closestPowOfTwo / (128.0 * size)), 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			for (int i = 0; i < deg - 1; i++) {
				compareDist /= 2; pow2--;
			}
		}
		glUseProgram(sortLocalInnerComputeProgram);
		glUniform1ui(maxSortLocalInnerUniform, sortCounter);
		glUniform1ui(subArraySizeLocalInnerUniform, subArraySize);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
		glDispatchCompute(ceil(sortCounter / 1024.0), 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
}

void ParticleTechnique::draw()
{
	float fdt = dt * 0.001;
	//nulovani sort counteru - kazdy snimek se vytvari znova, proto ze hodi pozice na zacatek
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[3]);
	unsigned data[4] = { 0, 0, 0, 0 };
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned)* 4, data);

	//nulovani grid counteru - kazdy snimek se vytvari znova, proto ze hodi pozice na zacatek
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[7]);
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned)* 4, data);

	//emit
	//bere mrtve castice z dead bufferu a nastavuje jim kladny cas zivota
	glUseProgram(emitComputeProgram);
	glUniform1ui(maxEmitUniform, (unsigned int)(100000 * fdt));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, vbo[4]);
	glDispatchCompute(ceil((100000 * fdt) / 256.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	//grid divide
	//rozdeleni castic podle pozice do mrizky, kdyz vyleze castice mimo, tak ji zabije
	//dale incializuje pole rpo pocatecni indexy
	glUseProgram(gridDivideComputeProgram);
	glUniform1ui(maxParticlesGridDivideUniform, (unsigned int)indices);
	glUniform1f(sizeGridDivideUniform, GRID_SIZE);
	glUniform1f(hGridDivideUniform, GRID_H);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, vbo[4]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 7, vbo[7]);
	glDispatchCompute(ceil(indices / 256.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	//precteni pozice v grid counteru, aby se vedelo kolik castic je ve mrizce
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[7]);
	GLuint *ptr = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT);
	GLuint gridCounter = ptr[0];
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

	//serazeni gridListu podle indexu do mrizky
	sort(gridCounter, vbo[5]);

	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[5]);
	GridList *ptr3 = (GridList*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GridList), GL_MAP_READ_BIT);
	std::vector<float> sortedList;
	for (int i = 0; i < gridCounter; i++) {
		sortedList.push_back(ptr3[i].cell_id);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	float prevDist = -10000.0;
	for (int i = 0; i < gridCounter; i++) {
		if (sortedList[i] < prevDist)
			i = i;
		prevDist = sortedList[i];
	}*/

	//grid find start
	//najde prvni indexy zacatku bucketu jednotlivych voxelu
	glUseProgram(gridFindStartComputeProgram);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 7, vbo[7]);
	glDispatchCompute(ceil(indices / 256.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[6]);
	StartIndexList *ptr4 = (StartIndexList*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(StartIndexList), GL_MAP_READ_BIT);
	std::vector<float> startIndexList;
	for (int i = 0; i < pow(GRID_SIZE / GRID_H, 3); i++) {
		startIndexList.push_back(ptr4[i].start);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/

	
	//simulace
	
	//vypocet density
	glUseProgram(simulateDensityComputeProgram);
	glUniform1ui(maxParticlesDensityUniform, (unsigned int)gridCounter);
	glUniform1f(hGridSimulateDensityUniform, GRID_H);
	glUniform1ui(gridMaxIndexDensityUniform, (unsigned int)pow((GRID_SIZE / GRID_H), 3));
	glUniform1ui(gridSizeSimulateDensityUniform, (unsigned int)(GRID_SIZE / GRID_H));
	glUniform1f(massSimulateDensityUniform, MASS);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
	glDispatchCompute(ceil(gridCounter / 256.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//vypocet pressure
	glUseProgram(simulatePressureComputeProgram);
	glUniform1ui(maxParticlesDensityUniform, (unsigned int)gridCounter);
	glUniform1f(gasConstantPressureUniform, GAS_CONSTANT);
	glUniform1f(restDensityPressureUniform, REST_DENSITY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glDispatchCompute(ceil(gridCounter / 256.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//vypocet interni sily
	glUseProgram(simulateForceComputeProgram);
	glUniform1ui(maxParticlesForceUniform, (unsigned int)gridCounter);
	glUniform1f(hGridSimulateForceUniform, GRID_H);
	glUniform1ui(gridMaxIndexForceUniform, (unsigned int)pow((GRID_SIZE / GRID_H), 3));
	glUniform1ui(gridSizeSimulateForceUniform, (unsigned int)(GRID_SIZE / GRID_H));
	glUniform1f(massSimulateForceUniform, MASS);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
	glDispatchCompute(ceil(gridCounter / 256.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//hleda castice s nezapornym casem zivota, ty odsimuluje a da je do sort bufferu
	glUseProgram(simulateComputeProgram);
	glUniform1f(dtUniform, fdt);
	glUniform3f(halfVectorUniform, halfVector.x, halfVector.y, halfVector.z);
	glUniform1ui(maxParticlesUniform, (unsigned int)gridCounter);
	glUniform1f(hGridSimulateUniform, GRID_H);
	glUniform1ui(gridMaxIndexUniform, (unsigned int)pow((GRID_SIZE / GRID_H), 3));
	glUniform1ui(gridSizeSimulateUniform, (unsigned int)(GRID_SIZE / GRID_H));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo[1]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, vbo[3]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, vbo[4]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
	glDispatchCompute(ceil(gridCounter / 256.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	//precteni pozice v sort counteru, aby se vedelo kolik castic se bude vykreslovat
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[3]);
	ptr = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT);
	GLuint sortCounter = ptr[0];
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

	//razeni
	sort(sortCounter, vbo[1]);
	
	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo[1]);
	SortList *ptr3 = (SortList*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SortList), GL_MAP_READ_BIT);
	std::vector<float> sortedList;
	for (int i = 0; i < sortCounter; i++) {
		sortedList.push_back(ptr3[i].distance);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	prevDist = -10000.0;
	for (int i = 0; i < sortCounter; i++) {
		if (sortedList[i] < prevDist)
			i = i;
		prevDist = sortedList[i];
	}*/
	
	//vykresleni
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	glUseProgram(program);

	glm::mat4 mv = v * m;

	glUniformMatrix4fv(mvUniform, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, glm::value_ptr(p));
	glUniform1i(texDifSamplerUniform, texDifSampler);

	glBindVertexArray(vao);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glDrawArrays(GL_POINTS, 0, sortCounter);
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

void ParticleTechnique::setHalfVector(glm::vec3 halfVec, bool flip)
{
	halfVector = halfVec;
	flippedHalfVector = flip;
}