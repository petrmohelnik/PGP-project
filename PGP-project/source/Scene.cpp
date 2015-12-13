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
		objects[i]->render(camera, lights, ambientLight, dt, Renderer::DRAW_STANDARD);
	}
}

const char* Scene::getName()
{
	return name.c_str();
}

void Scene::handleSdlEvent(SDL_Event &event)
{

}

MainScene::MainScene()
{
  const Uint32 width = 256;
  const Uint32 height = 256;

  glGenTextures(1, &textureDepth);
  glBindTexture(GL_TEXTURE_2D, textureDepth);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenTextures(1, &textureDepthAccum);
  glBindTexture(GL_TEXTURE_2D, textureDepthAccum);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenRenderbuffers(1, &rboDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

  glGenFramebuffers(1, &fboDepth);
  glBindFramebuffer(GL_FRAMEBUFFER, fboDepth);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepth, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureDepthAccum, 0);

  GLenum att[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, att);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

  if(GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "fbo error: " << status << "\n";

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
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

void MainScene::render(Uint32 dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_CW);

  /*if(lights.size())
  {
    const Camera camOld = camera;

    camera.lookAt(lights[0].pos, glm::vec3(0.0f));

    for(unsigned int i = 0; i < objects.size(); i++)
      objects[i]->render(camera, lights, ambientLight, dt, Renderer::DRAW_STANDARD);

    camera = camOld;
  }*/

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->render(camera, lights, ambientLight, dt, Renderer::DRAW_STANDARD);
	}
}