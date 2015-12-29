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
#define FBO_SHAFTS_WIDTH 512
#define FBO_SHAFTS_HEIGHT 512

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
	float skyMultiplier;

	GLuint fboDepth;
	GLuint fboDepthParticle;
	GLuint fboShafts;
	GLuint fboShaftsOut;
	//GLuint rboDepth;
	GLuint textureDepth;
	GLuint textureDepthParticle;
  GLuint textureDepthParticle2;
	GLuint textureDepthParticleAccum;
	GLuint textureShafts;
	GLuint textureShaftsOut;

	GLuint pShafts;
	GLuint pShaftsOut;
	GLuint fsQuadVao;
	GLuint fsQuadVbo;
	GLuint fsQuadAttrib;
	GLuint fsQuadSamplerShafts;
	GLuint sunPosUniformShafts;
	GLuint fsQuadSamplerShaftsOut;

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
	void setShaftPrograms(GLuint s, GLuint sOut);
};

#endif //SCENE_H