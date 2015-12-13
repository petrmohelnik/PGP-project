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
	virtual void render(Camera &cam, std::vector<Light> &lights, glm::vec3 ambientLight, int dt, DrawType drawType) = 0;
};

#endif //RENDERER_H