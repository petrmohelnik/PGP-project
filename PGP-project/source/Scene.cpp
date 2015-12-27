#include "Scene.h"

void Scene::setName(const char *n)
{
	name.append(n);
}

void Scene::addObject(std::shared_ptr<Renderer> o)
{
	objects.push_back(o);
}

void Scene::addLight(Light l)
{
	lights.push_back(l);
}

void Scene::setAmbientLight(glm::vec3 a)
{
	ambientLight = a;
}

void Scene::initCamera(float fov, int width, int height, float nearPlane, float farPlane, int mode)
{
	camera.init(fov, width, height, nearPlane, farPlane, mode);
}

Camera* Scene::getCamera()
{
	return &camera;
}

void Scene::render(Uint32 dt)
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->render(camera, lights, ambientLight, glm::mat4(), glm::mat4(), 0, 0, 0, dt, Renderer::DRAW_STANDARD);
	}
}

const char* Scene::getName()
{
	return name.c_str();
}

void Scene::handleSdlEvent(SDL_Event &event)
{

}

GLuint Scene::createFboMap(Uint32 w, Uint32 h, bool depth, bool cmp)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	if(depth)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if(cmp)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

MainScene::MainScene()
{
	textureDepth = createFboMap(FBO_SHADOW_WIDTH, FBO_SHADOW_HEIGHT, true, true);
	textureDepthParticle = createFboMap(FBO_SHADOW_PARTICLE_WIDTH, FBO_SHADOW_PARTICLE_HEIGHT, true, false);
	textureDepthParticleAccum = createFboMap(FBO_SHADOW_PARTICLE_WIDTH, FBO_SHADOW_PARTICLE_HEIGHT, false, false);
	textureShafts = createFboMap(FBO_SHAFTS_WIDTH, FBO_SHAFTS_HEIGHT, false, false);
	textureShaftsOut = createFboMap(FBO_SHAFTS_WIDTH, FBO_SHAFTS_HEIGHT, false, false);

	/*glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, FBO_WIDTH, FBO_HEIGHT);*/

	glGenFramebuffers(1, &fboDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, fboDepth);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepth, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureDepthAccum, 0);

	/*const GLenum att[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, att);*/
	glDrawBuffer(GL_NONE);

	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	if(GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "fbo error: " << status << "\n";

	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// druhý
	glGenFramebuffers(1, &fboDepthParticle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboDepthParticle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepthParticle, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureDepthParticleAccum, 0);
	const GLenum att[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, att);

	if(GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "fbo error: " << status << "\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// shafts
	glGenFramebuffers(1, &fboShafts);
	glBindFramebuffer(GL_FRAMEBUFFER, fboShafts);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureShafts, 0);
	glDrawBuffers(1, att);
	if(GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "fbo error: " << status << "\n";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// shafts out
	glGenFramebuffers(1, &fboShaftsOut);
	glBindFramebuffer(GL_FRAMEBUFFER, fboShaftsOut);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureShaftsOut, 0);
	glDrawBuffers(1, att);
	if(GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "fbo error: " << status << "\n";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MainScene::handleSdlEvent(SDL_Event &event)
{
	//Call proper event handlers
	switch (event.type)
	{
	case SDL_KEYDOWN:
		if (cameraMode == CAM_TRANS_ROT)
			break;
		onKeyDown(event.key.keysym.sym);
		break;
	case SDL_MOUSEMOTION:
		onMouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, event.motion.state);
		break;		
	case SDL_MOUSEWHEEL:
		if (cameraMode == CAM_ROT_TRANS)
			break;
		onMouseWheel(event.wheel.x, event.wheel.y);
		break;
	}

  if(event.window.event == SDL_WINDOWEVENT_RESIZED)
  {
    glViewport(0, 0, event.window.data1, event.window.data2);
    camera.resize(event.window.data1, event.window.data2);
  }
}

void MainScene::onKeyDown(SDL_Keycode key)
{
	glm::vec3 dir;
	switch (key) {
	case SDLK_UP:
		dir = glm::normalize(glm::rotate(glm::vec3(0.0f, 0.0f, 1.0f), camera.getRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)));
		dir = glm::normalize(glm::rotate(dir, camera.getRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)));
		camera.translate(glm::vec3(dir.x, dir.y, -dir.z));
		break;
	case SDLK_DOWN:
		dir = glm::normalize(glm::rotate(glm::vec3(0.0f, 0.0f, -1.0f), camera.getRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)));
		dir = glm::normalize(glm::rotate(dir, camera.getRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)));
		camera.translate(glm::vec3(dir.x, dir.y, -dir.z));
		break;
	case SDLK_LEFT:
		dir = glm::normalize(glm::rotate(glm::vec3(1.0f, 0.0f, 0.0f), camera.getRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)));
		dir = glm::normalize(glm::rotate(dir, camera.getRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)));
		camera.translate(glm::vec3(-dir.x, dir.y, dir.z));
		break;
	case SDLK_RIGHT:
		dir = glm::normalize(glm::rotate(glm::vec3(-1.0f, 0.0f, 0.0f), camera.getRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)));
		dir = glm::normalize(glm::rotate(dir, camera.getRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)));
		camera.translate(glm::vec3(-dir.x, dir.y, dir.z));
		break;
	}
}

void MainScene::onMouseMove(Sint32 x, Sint32 y, Sint32 xrel, Sint32 yrel, Uint32 state)
{
	if (state == SDL_BUTTON_RMASK) {
		camera.rotateX(xrel * 0.02f);
		camera.rotateY(yrel * 0.02f);
	}
}

void MainScene::onMouseWheel(Sint32 x, Sint32 y)
{
	camera.translate(glm::vec3(0.0, 0.0, -y));
}

void MainScene::setCameraMode(int mode)
{
	camera.setMode(mode);
	cameraMode = mode;
}

void MainScene::initCamera(float fov, int width, int height, float nearPlane, float farPlane, int mode)
{
	camera.init(fov, width, height, nearPlane, farPlane, mode);
	cameraMode = mode;
}

void MainScene::addParticleSystem(std::shared_ptr<ParticleSystemRenderer> o)
{
	particleSystem = o;
}

void MainScene::setShaftPrograms(GLuint s, GLuint sOut)
{
	pShafts = s;
	pShaftsOut = sOut;

	glGenVertexArrays(1, &fsQuadVao);
	glBindVertexArray(fsQuadVao);
	glGenBuffers(1, &fsQuadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, fsQuadVbo);
	const float vbodata[] =
	{
		-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vbodata, GL_STATIC_DRAW);

	GLint attr = glGetAttribLocation(pShafts, "v_pos");
	glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attr);

	fsQuadSamplerShafts = glGetUniformLocation(pShafts, "texDifSampler");
	sunPosUniformShafts = glGetUniformLocation(pShafts, "lightPos");
	fsQuadSamplerShaftsOut = glGetUniformLocation(pShaftsOut, "texDifSampler");

	glBindVertexArray(0);
}

void MainScene::render(Uint32 dt)
{
	glClearColor(0.05f, 0.1f, 0.12f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_CW);

	// shadows
	glBindFramebuffer(GL_FRAMEBUFFER, fboDepth);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(3.0f, 1.0f);

	const Camera camOld = camera;
	camera.resize(FBO_SHADOW_WIDTH, FBO_SHADOW_HEIGHT);
	camera.setRange(0.1f, 100.0f);
	glViewport(0, 0, static_cast<GLsizei>(camera.getSize().x), static_cast<GLsizei>(camera.getSize().y));
	camera.setPosRot(glm::vec3(0.0f, 0.0f, -30.0f), glm::vec2(-0.7854f, 0.7854f));
	const glm::mat4 mvpDepth = camera.getProjection() * camera.getView();

	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if(reinterpret_cast<BasicTechnique *>(objects[i]->getTechnique())->getMesh()->getMaterial()->getEmission().x == 0.0f)
			objects[i]->render(camera, lights, ambientLight, mvpDepth, glm::mat4(), 0, 0, 0, dt, Renderer::DRAW_SHADOW);
	}

	// shadows particle
	glBindFramebuffer(GL_FRAMEBUFFER, fboDepthParticle);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonOffset(1.0f, 1.0f);

	camera.resize(FBO_SHADOW_PARTICLE_WIDTH, FBO_SHADOW_PARTICLE_HEIGHT);
	glViewport(0, 0, static_cast<GLsizei>(camera.getSize().x), static_cast<GLsizei>(camera.getSize().y));
	const glm::mat4 mvpDepth2 = camera.getProjection() * camera.getView();

	particleSystem->simulate(dt);
	particleSystem->sort(camera, lights);
	particleSystem->render(camera, lights, ambientLight, mvpDepth, mvpDepth2, 0, 0, 0, dt, Renderer::DRAW_SHADOW);

	// shadts
	camera = camOld;
	glBindFramebuffer(GL_FRAMEBUFFER, fboShafts);
	glClearColor(0.2f, 0.4f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_POLYGON_OFFSET_FILL);
	//camera.resize(FBO_SHAFTS_WIDTH, FBO_SHAFTS_HEIGHT);
	glViewport(0, 0, FBO_SHAFTS_WIDTH, FBO_SHAFTS_HEIGHT);

	for(unsigned int i = 0; i < objects.size(); i++)
		objects[i]->renderShafts(camera, dt);

	particleSystem->sort(camera, lights);
	particleSystem->renderShafts(camera, dt);

	// shafts out
	glBindFramebuffer(GL_FRAMEBUFFER, fboShaftsOut);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_BLEND);

	glUseProgram(pShafts);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureShafts);
	glUniform1i(fsQuadSamplerShafts, 0);

	const glm::vec3 sunPos(150.0f, 210.0f, 150.0f);
	glm::vec4 sunScreen = glm::vec4(camera.getProjection() * camera.getView() * glm::vec4(sunPos, 1.0f)) * glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) + glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	//const float dist = 1.0f / glm::distance(sunPos, camera.getPos());
	sunScreen = glm::vec4((glm::vec3(sunScreen) / sunScreen.w) + glm::vec3(0.5f, 0.5f, 0.5f), 1.0);
	sunScreen.z = glm::dot(glm::normalize(glm::vec3(glm::vec3(glm::vec4(camera.getView() * glm::vec4(0.0, 0.0, -1.0, 1.0))) - camera.getPos())), glm::normalize(sunPos));
	glUniform3f(sunPosUniformShafts, sunScreen.x/* - dist*/, sunScreen.y/* - dist*/, -sunScreen.z);
	//std::cout << sunScreen.x << " " << sunScreen.y << " " << -sunScreen.z << " " << sunScreen.w << "\n";

	glBindVertexArray(fsQuadVao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);

	// out
	glEnable(GL_DEPTH_TEST);
	//camera = camOld;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, static_cast<GLsizei>(camera.getSize().x), static_cast<GLsizei>(camera.getSize().y));

	for(unsigned int i = 0; i < objects.size(); i++)
		objects[i]->render(camera, lights, ambientLight, mvpDepth, mvpDepth2, textureDepth, textureDepthParticle, textureDepthParticleAccum, dt, Renderer::DRAW_STANDARD);

	particleSystem->render(camera, lights, ambientLight, mvpDepth, mvpDepth, textureDepth, textureDepthParticle, textureDepthParticleAccum, dt, Renderer::DRAW_STANDARD);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glUseProgram(pShaftsOut);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureShaftsOut);
	glUniform1i(fsQuadSamplerShaftsOut, 0);

	glBindVertexArray(fsQuadVao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	// testing only, don't panic (SDL_GL_CONTEXT_PROFILE_COMPATIBILITY) --------------------------
	const bool showDebugShadow = false;

	if(showDebugShadow)
	{
		glUseProgram(0);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureShafts);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		const float scale = 1.0f / FBO_SHADOW_WIDTH * 256.0f;
		const float fboW = FBO_SHADOW_WIDTH / camera.getSize().x * scale;
		const float fboH = FBO_SHADOW_HEIGHT / camera.getSize().y * scale;

		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-1.0f + fboW, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-1.0f + fboW, 1.0f - fboH, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f - fboH, 1.0f);
		glEnd();

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, textureShaftsOut);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f - fboH, 1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-1.0f + fboW, 1.0f - fboH, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-1.0f + fboW, 1.0f - 2.0f * fboH, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f - 2.0f * fboH, 1.0f);
		glEnd();

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, textureDepthParticleAccum);

		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f - 2.0f * fboH, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f + fboW, 1.0f - 2.0f * fboH, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f + fboW, 1.0f - 3.0f * fboH, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f - 3.0f * fboH, 1.0f);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
	}
}