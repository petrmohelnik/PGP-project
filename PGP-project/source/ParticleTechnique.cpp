#include "ParticleTechnique.h"

void ParticleTechnique::init(Mesh &m, int count, GLuint p, GLuint pShaft, GLuint simulateComputeP, GLuint emitComputeP, GLuint sortPreComputeP, GLuint sortComputeP, GLuint sortLocalComputeP,
	GLuint sortLocalInnerComputeP, GLuint gridDivideComputeP, GLuint gridFindStartComputeP,
	GLuint simulateDensityComputeP, GLuint simulatePressureComputeP, GLuint simulateForceComputeP)
{
	mesh = &m;
	program = p;
	programShaft = pShaft;

	simulateComputeProgram = simulateComputeP;
	emitComputeProgram = emitComputeP;
	sortPreComputeProgram = sortPreComputeP;
	sortComputeProgram = sortComputeP;
	sortLocalComputeProgram = sortLocalComputeP;
	sortLocalInnerComputeProgram = sortLocalInnerComputeP;
	gridDivideComputeProgram = gridDivideComputeP;
	gridFindStartComputeProgram = gridFindStartComputeP;
	simulateDensityComputeProgram = simulateDensityComputeP;
	simulatePressureComputeProgram = simulatePressureComputeP;
	simulateForceComputeProgram = simulateForceComputeP;

	glGenVertexArrays(1, &vao);
	glGenBuffers(9, vbo);
	indices = count;

	mvUniform = glGetUniformLocation(program, "mv");
	mvpDepthUniform = glGetUniformLocation(program, "mvpDepth");
	mvpDepth2Uniform = glGetUniformLocation(program, "mvpDepth2");
	pUniform = glGetUniformLocation(program, "p");
	ambientLightUniform = glGetUniformLocation(program, "ambientLight");
	texDifSamplerUniform = glGetUniformLocation(program, "texDifSampler");
	texDepthSamplerUniform = glGetUniformLocation(program, "texDepthSampler");
	texDepth2SamplerUniform = glGetUniformLocation(program, "texDepth2Sampler");
	texDepth3SamplerUniform = glGetUniformLocation(program, "texDepth3Sampler");
  texDepth4SamplerUniform = glGetUniformLocation(program, "texDepth4Sampler");
	dtUniform = glGetUniformLocation(simulateComputeProgram, "dt");
	//halfVectorUniform = glGetUniformLocation(simulateComputeProgram, "halfVector");
	maxParticlesUniform = glGetUniformLocation(simulateComputeProgram, "maxParticles");
	//hGridSimulateUniform = glGetUniformLocation(simulateComputeProgram, "h");
	//gridMaxIndexUniform = glGetUniformLocation(simulateComputeProgram, "gridMaxIndex");
	//gridSizeSimulateUniform = glGetUniformLocation(simulateComputeProgram, "gridSize");
	buoyancySimulateUniform = glGetUniformLocation(simulateComputeProgram, "buoyancy");
	restDensitySimulateUniform = glGetUniformLocation(simulateComputeProgram, "restDensity");
	gravitySimulateUniform = glGetUniformLocation(simulateComputeProgram, "gravity");
	timeSimulateUniform = glGetUniformLocation(simulateComputeProgram, "time");
	maxEmitUniform = glGetUniformLocation(emitComputeProgram, "maxEmit");
	halfVectorUniform = glGetUniformLocation(sortPreComputeProgram, "halfVector");
	maxParticlesSortPreUniform = glGetUniformLocation(sortPreComputeProgram, "maxParticles");
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
	hGridSimulateDensityUniform = glGetUniformLocation(simulateDensityComputeProgram, "h");
	gridMaxIndexDensityUniform = glGetUniformLocation(simulateDensityComputeProgram, "gridMaxIndex");
	gridSizeSimulateDensityUniform = glGetUniformLocation(simulateDensityComputeProgram, "gridSize");
	massSimulateDensityUniform = glGetUniformLocation(simulateDensityComputeProgram, "mass");
	maxParticlesPressureUniform = glGetUniformLocation(simulatePressureComputeProgram, "maxParticles");
	gasConstantPressureUniform = glGetUniformLocation(simulatePressureComputeProgram, "gasConstant");
	restDensityPressureUniform = glGetUniformLocation(simulatePressureComputeProgram, "restDensity");
	maxParticlesForceUniform = glGetUniformLocation(simulateForceComputeProgram, "maxParticles");
	hGridSimulateForceUniform = glGetUniformLocation(simulateForceComputeProgram, "h");
	gridMaxIndexForceUniform = glGetUniformLocation(simulateForceComputeProgram, "gridMaxIndex");
	gridSizeSimulateForceUniform = glGetUniformLocation(simulateForceComputeProgram, "gridSize");
	massSimulateForceUniform = glGetUniformLocation(simulateForceComputeProgram, "mass");
	viscositySimulateForceUniform = glGetUniformLocation(simulateForceComputeProgram, "viscosity");

	std::vector<ParticlePool> particlePool;
	particlePool.reserve(indices);
	for (int i = 0; i < indices; i++) {
		particlePool.push_back(ParticlePool(glm::vec4((((double)rand()) / RAND_MAX) * 4 - 2, (((double)rand()) / RAND_MAX) * 4, (((double)rand()) / RAND_MAX) * 4 - 2, -1.0),
			glm::vec4(0), glm::vec4(0)));
	}

	//obsahuje castice - jejich pozici a cas zivota
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, particlePool.size() * sizeof(ParticlePool), &(particlePool[0]), GL_STATIC_DRAW);

	//obsahuje castice - do nej se ulozi kopie castic po serazeni do mrizky, aby kdyz se k nim pristupuje, byly u sebe - urychleni
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, particlePool.size() * sizeof(ParticlePool), &(particlePool[0]), GL_STATIC_DRAW);

	std::vector<SortList> sortList;
	sortList.reserve(indices);
	for (int i = 0; i < indices; i++) {
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

	// shafts
	glGenVertexArrays(1, &vaoShaft);
	glBindVertexArray(vaoShaft);
	mvUniformShaft = glGetUniformLocation(programShaft, "mv");
	pUniformShaft = glGetUniformLocation(programShaft, "p");
	ambientLightUniformShaft = glGetUniformLocation(programShaft, "ambientLight");
	texDifSamplerUniformShaft = glGetUniformLocation(programShaft, "texDifSampler");
	attr = glGetAttribLocation(programShaft, "particleID");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribIPointer(attr, 1, GL_UNSIGNED_INT, sizeof(SortList), (GLvoid*)offsetof(SortList, index));
	glEnableVertexAttribArray(attr);
	glBindVertexArray(0);
	// shafts end

	std::vector<DeadList> deadList;
	deadList.reserve(indices);
	for (int i = 0; i < indices; i++) {
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
	for (int i = 0; i < indices; i++) {
		gridList.push_back(GridList(0, 0));
	}
	//rozrazeni castic do mrizky
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, gridList.size() * sizeof(GridList), &(gridList[0]), GL_STATIC_DRAW);

	std::vector<StartIndexList> startIndexList;
	startIndexList.reserve(static_cast<unsigned int>(pow((GRID_SIZE / GRID_H), 3.0)));
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
	glDispatchCompute(static_cast<GLuint>(ceil(sortCounter / 512.0)), 1, 1);
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
			unsigned int size = static_cast<unsigned int>(pow(2.0, deg));
			glDispatchCompute(static_cast<GLuint>(ceil(closestPowOfTwo / (128.0 * size))), 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			for (unsigned int i = 0; i < deg - 1; i++) {
				compareDist /= 2; pow2--;
			}
		}
		glUseProgram(sortLocalInnerComputeProgram);
		glUniform1ui(maxSortLocalInnerUniform, sortCounter);
		glUniform1ui(subArraySizeLocalInnerUniform, subArraySize);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
		glDispatchCompute(static_cast<GLuint>(ceil(sortCounter / 1024.0)), 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
}

void ParticleTechnique::simulate()
{
	counter++;
	float fdt = static_cast<float>(dt) * 0.001f;
	if (fdt > 0.005f)
		fdt = 0.005f;
	time += fdt;

	unsigned data[4] = { 0, 0, 0, 0 };
	//nulovani grid counteru - kazdy snimek se vytvari znova, proto ze hodi pozice na zacatek
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[7]);
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned)* 4, data);

	//emit
	//bere mrtve castice z dead bufferu a nastavuje jim kladny cas zivota
	/*if (counter == 1) {
		glUseProgram(emitComputeProgram);
		glUniform1ui(maxEmitUniform, (unsigned int)(10000));
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, vbo[4]);
		glDispatchCompute(ceil((10000) / 256.0), 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);
	}*/
	if (time - prevTime > 0.2) {
		prevTime = time;
		glUseProgram(emitComputeProgram);
		glUniform1ui(maxEmitUniform, (unsigned int)(200));
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, vbo[4]);
		glDispatchCompute(static_cast<GLuint>(ceil((200) / 256.0)), 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);
	}

	//grid divide
	//rozdeleni castic podle pozice do mrizky, kdyz vyleze castice mimo, tak ji zabije
	//dale incializuje pole rpo pocatecni indexy
	glUseProgram(gridDivideComputeProgram);
	glUniform1ui(maxParticlesGridDivideUniform, (unsigned int)indices);
	glUniform1f(sizeGridDivideUniform, GRID_SIZE);
	glUniform1f(hGridDivideUniform, static_cast<float>(GRID_H));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, vbo[4]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 7, vbo[7]);
	glDispatchCompute(
		static_cast<GLuint>(ceil((pow(GRID_SIZE / GRID_H, 3)) > static_cast<GLuint>(indices) ?
		static_cast<GLuint>(pow(GRID_SIZE / GRID_H, 3)) :
		static_cast<GLuint>(indices)) / 256.0), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	//precteni pozice v grid counteru, aby se vedelo kolik castic je ve mrizce
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[7]);
	GLuint *ptr = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT);
	gridCounter = ptr[0];
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

	//serazeni gridListu podle indexu do mrizky
	sort(gridCounter, vbo[5]);

	//grid find start
	//najde prvni indexy zacatku bucketu jednotlivych voxelu
	glUseProgram(gridFindStartComputeProgram);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 7, vbo[7]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, vbo[8]);
	glDispatchCompute(static_cast<GLuint>(ceil(gridCounter / 256.0)), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//simulace

	//vypocet density
	glUseProgram(simulateDensityComputeProgram);
	glUniform1ui(maxParticlesDensityUniform, (unsigned int)gridCounter);
	glUniform1f(hGridSimulateDensityUniform, static_cast<float>(GRID_H));
	glUniform1ui(gridMaxIndexDensityUniform, (unsigned int)pow((GRID_SIZE / GRID_H), 3));
	glUniform1ui(gridSizeSimulateDensityUniform, (unsigned int)(GRID_SIZE / GRID_H));
	glUniform1f(massSimulateDensityUniform, static_cast<float>(MASS));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, vbo[8]);
	glDispatchCompute(static_cast<GLuint>(ceil(gridCounter / 256.0)), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//vypocet pressure
	glUseProgram(simulatePressureComputeProgram);
	glUniform1ui(maxParticlesPressureUniform, (unsigned int)gridCounter);
	glUniform1f(gasConstantPressureUniform, GAS_CONSTANT);
	glUniform1f(restDensityPressureUniform, static_cast<float>(REST_DENSITY));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, vbo[8]);
	glDispatchCompute(static_cast<GLuint>(ceil(gridCounter / 256.0)), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//vypocet interni sily
	glUseProgram(simulateForceComputeProgram);
	glUniform1ui(maxParticlesForceUniform, (unsigned int)gridCounter);
	glUniform1f(hGridSimulateForceUniform, static_cast<float>(GRID_H));
	glUniform1ui(gridMaxIndexForceUniform, (unsigned int)pow((GRID_SIZE / GRID_H), 3));
	glUniform1ui(gridSizeSimulateForceUniform, (unsigned int)(GRID_SIZE / GRID_H));
	glUniform1f(massSimulateForceUniform, static_cast<float>(MASS));
	glUniform1f(viscositySimulateForceUniform, static_cast<float>(VISCOSITY));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vbo[6]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, vbo[8]);
	glDispatchCompute(static_cast<GLuint>(ceil(gridCounter / 256.0)), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//hleda castice s nezapornym casem zivota, ty odsimuluje
	glUseProgram(simulateComputeProgram);
	glUniform1f(dtUniform, fdt);
	glUniform1ui(maxParticlesUniform, (unsigned int)gridCounter);
	glUniform1f(buoyancySimulateUniform, BUOYANCY);
	glUniform1f(restDensitySimulateUniform, static_cast<float>(REST_DENSITY));
	glUniform1f(gravitySimulateUniform, static_cast<float>(GRAVITY));
	glUniform1f(timeSimulateUniform, time);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbo[2]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, vbo[4]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, vbo[8]);
	glDispatchCompute(static_cast<GLuint>(ceil(gridCounter / 256.0)), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);
}

void ParticleTechnique::sortParticles()
{
	//nulovani sort counteru - kazdy snimek se vytvari znova, proto se hodi pozice na zacatek
	unsigned data[4] = { 0, 0, 0, 0 };
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[3]);
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned)* 4, data);

	//naplneni bufery pro razeni klici
	glUseProgram(sortPreComputeProgram);
	glUniform3f(halfVectorUniform, halfVector.x, halfVector.y, halfVector.z);
	glUniform1ui(maxParticlesSortPreUniform, (unsigned int)gridCounter);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo[1]);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, vbo[3]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vbo[5]);
	glDispatchCompute(static_cast<GLuint>(ceil(gridCounter / 256.0)), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	//precteni pozice v sort counteru, aby se vedelo kolik castic se bude vykreslovat
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vbo[3]);
	GLuint *ptr = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT);
	sortCounter = ptr[0];
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

	//razeni
	sort(sortCounter, vbo[1]);
}

int ParticleTechnique::getNumParticles()
{
	return sortCounter;
}

void ParticleTechnique::draw()
{
	//vykresleni
	//glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	//glBlendEquation(GL_FUNC_ADD);
	if(flippedHalfVector)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else
		//glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	glUseProgram(program);

	glm::mat4 mv = v * m;

	glUniformMatrix4fv(mvUniform, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(mvpDepthUniform, 1, GL_FALSE, glm::value_ptr(mvpDepth));
	glUniformMatrix4fv(mvpDepth2Uniform, 1, GL_FALSE, glm::value_ptr(mvpDepth2));
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, glm::value_ptr(p));
	glUniform3f(ambientLightUniform, ambientLight.x, ambientLight.y, ambientLight.z);
	glUniform1i(texDifSamplerUniform, texDifSampler);
	glUniform1i(texDepthSamplerUniform, texDepthSampler);
	glUniform1i(texDepth2SamplerUniform, texDepth2Sampler);
	glUniform1i(texDepth3SamplerUniform, texDepth3Sampler);
  glUniform1i(texDepth4SamplerUniform, texDepth4Sampler);

	glBindVertexArray(vao);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glDrawArrays(GL_POINTS, 0, sortCounter);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
	//glDepthMask(GL_TRUE);
}

void ParticleTechnique::drawShafts()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(programShaft);

	glm::mat4 mv = v * m;

	glUniformMatrix4fv(mvUniformShaft, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(pUniformShaft, 1, GL_FALSE, glm::value_ptr(p));
	glUniform3f(ambientLightUniformShaft, ambientLight.x, ambientLight.y, ambientLight.z);
	glUniform1i(texDifSamplerUniformShaft, texDifSampler);

	glBindVertexArray(vaoShaft);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo[0]);
	glDrawArrays(GL_POINTS, 0, sortCounter);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}

void ParticleTechnique::setM(const glm::mat4 &mat)
{
	m = mat;
}

void ParticleTechnique::setV(const glm::mat4 &mat)
{
	v = mat;
}

void ParticleTechnique::setP(const glm::mat4 &mat)
{
	p = mat;
}

void ParticleTechnique::setDt(int t)
{
	dt = t;
}

void ParticleTechnique::setViewPos(const glm::vec3 &pos)
{
	viewPos = pos;
}

void ParticleTechnique::setHalfVector(const glm::vec3 &halfVec, bool flip)
{
	halfVector = halfVec;
	flippedHalfVector = flip;
}

void ParticleTechnique::setAmbientLight(const glm::vec3 &a)
{
	ambientLight = a;
}

void ParticleTechnique::setDepth(const glm::mat4 &mvp, const glm::mat4 &mvp2, GLuint texture, GLuint texture2, GLuint texture3, GLuint texture4)
{
	mvpDepth = mvp;
	mvpDepth2 = mvp2;
	texDepth = texture;
	texDepth2 = texture2;
	texDepth3 = texture3;
  texDepth4 = texture4;
}

void ParticleTechnique::bindTexDif(int t)
{
	glActiveTexture(GL_TEXTURE0 + t);
	glBindTexture(GL_TEXTURE_2D, texDif);
	texDifSampler = t;
}

void ParticleTechnique::bindTexDepth(int t, int t2, int t3, int t4)
{
	glActiveTexture(GL_TEXTURE0 + t);
	glBindTexture(GL_TEXTURE_2D, texDepth);
	texDepthSampler = t;
	glActiveTexture(GL_TEXTURE0 + t2);
	glBindTexture(GL_TEXTURE_2D, texDepth2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	texDepth2Sampler = t2;
	glActiveTexture(GL_TEXTURE0 + t3);
	glBindTexture(GL_TEXTURE_2D, texDepth3);
	texDepth3Sampler = t3;
  glActiveTexture(GL_TEXTURE0 + t4);
  glBindTexture(GL_TEXTURE_2D, texDepth4);
  texDepth4Sampler = t4;
}

Mesh *ParticleTechnique::getMesh()
{
	return mesh;
}