#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
class Model;
class Camera;
struct Light;

class Renderer
{
protected:
public:
	enum DrawType
	{
		DRAW_STANDARD = 0,
		DRAW_SHADOW
	};

	//virtual bool initRenderer(Model &m, GLuint p) = 0;
	virtual void render(Camera &cam, const std::vector<Light> &lights, const glm::vec3 &ambientLight, const glm::mat4 &mvpDepth, const glm::mat4 &mvpDepth2, GLuint texDepth, GLuint texDepth2, GLuint texDepth3, int dt, DrawType drawType) = 0;
	virtual void renderShafts(Camera &cam, int dt) = 0;
	virtual void *getTechnique() = 0;
};

#endif //RENDERER_H