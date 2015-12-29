#include "BasicTechnique.h"

void BasicTechnique::init(Mesh &m, GLuint p, GLuint pShaft)
{
	mesh = &m;
	program = p;
	programShaft = pShaft;

	GLuint vbo[3];
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(3, vbo);

	indices = m.getSize();
	drawMode = m.getDrawMode();

	mvpUniform = glGetUniformLocation(program, "mvp");
	mvpDepthUniform = glGetUniformLocation(program, "mvpDepth");
	mvpDepth2Uniform = glGetUniformLocation(program, "mvpDepth2");
	mUniform = glGetUniformLocation(program, "m");
	ti_mUniform = glGetUniformLocation(program, "ti_m");
	viewPosUniform = glGetUniformLocation(program, "viewPos");
	lightPosUniform = glGetUniformLocation(program, "lightPos");
	ambientLightUniform = glGetUniformLocation(program, "ambientLight");
	texDifSamplerUniform = glGetUniformLocation(program, "texDifSampler");
	texDepthSamplerUniform = glGetUniformLocation(program, "texDepthSampler");
	texDepth2SamplerUniform = glGetUniformLocation(program, "texDepth2Sampler");
	texDepth3SamplerUniform = glGetUniformLocation(program, "texDepth3Sampler");
  texDepth4SamplerUniform = glGetUniformLocation(program, "texDepth4Sampler");

	GLint attr = glGetAttribLocation(program, "v_pos");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m.getSize() * sizeof(float) * 3, m.getVertices(), GL_STATIC_DRAW);
	glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attr);

	attr = glGetAttribLocation(program, "v_norm");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m.getSize() * sizeof(float) * 3, m.getNormals(), GL_STATIC_DRAW);
	glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attr);

	attr = glGetAttribLocation(program, "v_texCoord");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, m.getSize() * sizeof(float) * 2, m.getTexCoords(), GL_STATIC_DRAW);
	glVertexAttribPointer(attr, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attr);

	glBindVertexArray(0);

	// shafts
	glGenVertexArrays(1, &vaoShaft);
	glBindVertexArray(vaoShaft);
	mvpUniformShaft = glGetUniformLocation(programShaft, "mvp");
	ambientLightUniformShaft = glGetUniformLocation(programShaft, "ambientLight");
	attr = glGetAttribLocation(programShaft, "v_pos");
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attr);
	glBindVertexArray(0);
	// shafts end

	glGenTextures(1, &texDif);
	glBindTexture(GL_TEXTURE_2D, texDif);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m.getMaterial()->getDifTex().width, m.getMaterial()->getDifTex().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(m.getMaterial()->getDifTex().tex[0]));
	glGenerateMipmap(GL_TEXTURE_2D);
}

void BasicTechnique::draw()
{
	glUseProgram(program);

	glm::mat4 mvp = p * v * m;

	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(mvpDepthUniform, 1, GL_FALSE, glm::value_ptr(mvpDepth));
	glUniformMatrix4fv(mvpDepth2Uniform, 1, GL_FALSE, glm::value_ptr(mvpDepth2));
	glUniformMatrix3fv(mUniform, 1, GL_FALSE, glm::value_ptr(glm::mat3(m)));
	glUniformMatrix3fv(ti_mUniform, 1, GL_FALSE, glm::value_ptr(glm::mat3(ti_m)));
	glUniform3f(viewPosUniform, -viewPos.x, -viewPos.y, -viewPos.z);
	glUniform3f(lightPosUniform, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(ambientLightUniform, ambientLight.x, ambientLight.y, ambientLight.z);
	glUniform1i(texDifSamplerUniform, texDifSampler);
	glUniform1i(texDepthSamplerUniform, texDepthSampler);
	glUniform1i(texDepth2SamplerUniform, texDepth2Sampler);
	glUniform1i(texDepth3SamplerUniform, texDepth3Sampler);
  glUniform1i(texDepth4SamplerUniform, texDepth4Sampler);
	
	glBindVertexArray(vao);
	glDrawArrays(drawMode, 0, indices);
	glBindVertexArray(0);
}

void BasicTechnique::drawShafts()
{
	glUseProgram(programShaft);

	glm::mat4 mvp = p * v * m;

	glUniformMatrix4fv(mvpUniformShaft, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform3f(ambientLightUniformShaft, ambientLight.x, ambientLight.y, ambientLight.z);

	glBindVertexArray(vaoShaft);
	glDrawArrays(drawMode, 0, indices);
	glBindVertexArray(0);
}

void BasicTechnique::setM(const glm::mat4 &mat)
{
	m = mat;
	ti_m = glm::transpose(glm::inverse(mat));
}

void BasicTechnique::setV(const glm::mat4 &mat)
{
	v = mat;
}

void BasicTechnique::setP(const glm::mat4 &mat)
{
	p = mat;
}

void BasicTechnique::setViewPos(const glm::vec3 &v)
{
	viewPos = v;
}

void BasicTechnique::setLightPos(const glm::vec3 &p)
{
	lightPos = p;
}

void BasicTechnique::setAmbientLight(const glm::vec3 &a)
{
	ambientLight = a;
}

void BasicTechnique::setDepth(const glm::mat4 &mvp, const glm::mat4 &mvp2, GLuint texture, GLuint texture2, GLuint texture3, GLuint texture4)
{
	mvpDepth = mvp;
	mvpDepth2 = mvp2;
	texDepth = texture;
	texDepth2 = texture2;
	texDepth3 = texture3;
  texDepth4 = texture4;
}

void BasicTechnique::bindTexDif(int t)
{
	glActiveTexture(GL_TEXTURE0 + t);
	glBindTexture(GL_TEXTURE_2D, texDif);
	texDifSampler = t;
}

void BasicTechnique::bindTexDepth(int t, int t2, int t3, int t4)
{
	glActiveTexture(GL_TEXTURE0 + t);
	glBindTexture(GL_TEXTURE_2D, texDepth);
	texDepthSampler = t;
	glActiveTexture(GL_TEXTURE0 + t2);
	glBindTexture(GL_TEXTURE_2D, texDepth2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	texDepth2Sampler = t2;
	glActiveTexture(GL_TEXTURE0 + t3);
	glBindTexture(GL_TEXTURE_2D, texDepth3);
	texDepth3Sampler = t3;
  glActiveTexture(GL_TEXTURE0 + t4);
  glBindTexture(GL_TEXTURE_2D, texDepth4);
  texDepth4Sampler = t4;
}

Mesh *BasicTechnique::getMesh()
{
	return mesh;
}