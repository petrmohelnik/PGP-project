#include "Model.h"

void Material::setDifTex(Texture tex)
{
	difTex = tex;
}

Texture Material::getDifTex()
{
	return difTex;
}

void Mesh::addVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 texCoord)
{
	v.push_back(vertex);
	n.push_back(normal);
	t.push_back(texCoord);
}

void Mesh::addMaterial(std::shared_ptr<Material> mat)
{
	m = mat;
}

float *Mesh::getVertices()
{
	return &v[0].x;
}

float *Mesh::getNormals()
{
	return &n[0].x;
}

float *Mesh::getTexCoords()
{
	return &t[0].x;
}

int Mesh::getSize()
{
	return v.size();
}

std::shared_ptr<Material> Mesh::getMaterial()
{
	return m;
}

void Mesh::createSphere(float r, int vertices)
{
	float pi = 3.141593f;
	int space = 360 / vertices;

	for (int b = 0; b <= 180; b += space)
	{
		for (int a = 0; a <= 360; a += space)
		{
			n.push_back(glm::vec3(sin((a * pi) / 180) * sin((b * pi) / 180), cos((a * pi) / 180), cos((b * pi) / 180) * sin((a * pi) / 180)));
			v.push_back(n[n.size() - 1] * r);
			t.push_back(glm::vec2(b / 180.0, a / 360.0));

			n.push_back(glm::vec3(sin((a * pi) / 180) * sin(((b + space) * pi) / 180), cos((a * pi) / 180), cos(((b + space) * pi) / 180) * sin((a * pi) / 180)));
			v.push_back(n[n.size() - 1] * r);
			t.push_back(glm::vec2(b / 180.0, a / 360.0));
		}
	}

	drawMode = GL_TRIANGLE_STRIP;
}

void Mesh::createParticles(int count)
{
	for (int i = 0; i < count; i++)
		v.push_back(glm::vec3((((double)rand()) / RAND_MAX) * 4 - 2, (((double)rand()) / RAND_MAX) * 4 - 2, (((double)rand()) / RAND_MAX) * 4 - 2));
}

void Mesh::setDrawMode(GLuint m)
{
	drawMode = m;
}

GLuint Mesh::getDrawMode() {
	return drawMode;
}

std::vector<std::shared_ptr<Mesh> > Model::getMeshes()
{
	return meshes;
}

int Model::getMeshesSize()
{
	return meshes.size();
}

int Model::addMesh(std::shared_ptr<Mesh> mesh)
{
	meshes.push_back(mesh);

	return meshes.size();
}