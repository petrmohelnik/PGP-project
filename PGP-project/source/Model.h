#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <GL/glew.h>

struct Texture
{
	std::vector<unsigned char> tex;
	unsigned int width;
	unsigned int height;
};

class Material
{
private:
	Texture difTex;
public:
	void setDifTex(Texture tex);
	Texture getDifTex();
};

class Mesh
{
protected:
	std::vector<glm::vec3> v;
	std::vector<glm::vec3> n;
	std::vector<glm::vec2> t;
	std::shared_ptr<Material> m;
	GLuint drawMode;
public:
	virtual void addVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 texCoord);
	void addMaterial(std::shared_ptr<Material> mat);
	float *getVertices();
	float *getNormals();
	float *getTexCoords();
	int getSize();
	std::shared_ptr<Material> getMaterial();
	void createSphere(float r, int vertices);
	void createParticles(int count);
	void setDrawMode(GLuint m);
	GLuint getDrawMode();
};

class Model
{
private:
	std::vector<std::shared_ptr<Mesh> > meshes;
public:
	std::vector<std::shared_ptr<Mesh> > getMeshes();
	int getMeshesSize();
	int addMesh(std::shared_ptr<Mesh> mesh);
};

#endif //MODEL_H