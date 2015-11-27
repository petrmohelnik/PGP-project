#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(glm::vec3 position)
{
	pos = position;
	technique.reset(new BasicTechnique);
}

bool BasicRenderer::initRenderer(Model &m, GLuint p)
{
	if (m.getMeshesSize() < 1) {
		std::cout << "ERROR: Model is empty!";
			return false;
	}

	technique->init(*(m.getMeshes()[0]), p);
	return true;
}

void BasicRenderer::render(Camera &cam, std::vector<Light> &lights, glm::vec3 ambientLight)
{
	if (lights.size() > 0)
		technique->setLightPos(lights[0].pos);
	technique->setAmbientLight(ambientLight);
	technique->setP(cam.getProjection());
	technique->setV(cam.getView());
	technique->setViewPos(cam.getPos());
	technique->bindTexDif(0);

	glm::mat4 M = glm::mat4(1.0);
	M = glm::translate(M, pos);

	technique->setM(M);

	technique->draw();


	M = glm::translate(M, glm::vec3(0.25));
	M = glm::scale(M, glm::vec3(0.7));
	technique->setM(M);

	technique->draw();

	M = glm::translate(M, glm::vec3(0.25));
	M = glm::scale(M, glm::vec3(0.7));
	technique->setM(M);

	technique->draw();
}

