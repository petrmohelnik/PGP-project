#ifndef BASIC_RENDERER_H
#define BASIC_RENDERER_H

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "BasicTechnique.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "Scene.h"

class BasicRenderer : public Renderer
{
private:
	std::shared_ptr<BasicTechnique> technique;
	glm::vec3 pos;
public:
	BasicRenderer(const glm::vec3 &position);
	bool initRenderer(Model &m, GLuint p, GLuint pShaft);
	void render(Camera &cam, const std::vector<Light> &lights, const glm::vec3 &ambientLight, const glm::mat4 &mvpDepth, const glm::mat4 &mvpDepth2, GLuint texDepth, GLuint texDepth2, GLuint texDepth3, GLuint texDepth4, int dt, DrawType drawType);
	void renderShafts(Camera &cam, int dt);
	void *getTechnique();
};

#endif //BASIC_RENDERER_H