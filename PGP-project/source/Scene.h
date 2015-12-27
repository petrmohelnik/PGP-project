#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <SDL/SDL.h>
#include "ParticleSystemRenderer.h"
#include "BasicTechnique.h"

#define FBO_SHADOW_WIDTH 2048
#define FBO_SHADOW_HEIGHT 2048
#define FBO_SHADOW_PARTICLE_WIDTH 2048
#define FBO_SHADOW_PARTICLE_HEIGHT 2048

class Scene
{
protected:
	std::string name;
	std::vector<std::shared_ptr<Renderer> > objects;
	Camera camera;
	std::vector<Light> lights;
	glm::vec3 ambientLight;
public:
	void setName(const char *n);
	void addObject(std::shared_ptr<Renderer> o);
	void addLight(Light l);
	void setAmbientLight(glm::vec3 a);
	virtual void initCamera(float fov, int width, int height, float nearPlane, float farPlane, int mode = CAM_ROT_TRANS);
	Camera* getCamera();
	virtual void render(Uint32 dt);
	const char* getName();
	virtual void handleSdlEvent(SDL_Event &event);

	static GLuint createFboMap(Uint32 w, Uint32 h, bool depth, bool cmp);
};

class MainScene : public Scene
{
private:
	int cameraMode;
  GLuint fboDepth;
  GLuint fboDepthParticle;
  //GLuint rboDepth;
  GLuint textureDepth;
  GLuint textureDepthParticle;
  GLuint textureDepthParticleAccum;
  std::shared_ptr<ParticleSystemRenderer> particleSystem;

	void onKeyDown(SDL_Keycode key);
	void onMouseMove(Sint32 x, Sint32 y, Sint32 xrel, Sint32 yrel, Uint32 state);
	void onMouseWheel(Sint32 x, Sint32 y);

public:
  MainScene();
	void handleSdlEvent(SDL_Event &event);
	void setCameraMode(int mode);
	void initCamera(float fov, int width, int height, float nearPlane, float farPlane, int mode = CAM_ROT_TRANS);
	void render(Uint32 dt);
	void addParticleSystem(std::shared_ptr<ParticleSystemRenderer> o);
};

#endif //SCENE_H