#include "Model.h"

Material::Material() : emission(0.0f)
{

}

void Material::setDifTex(const Texture &tex)
{
	difTex = tex;
}

void Material::setEmission(const glm::vec3 &e)
{
	emission = e;
}

Texture Material::getDifTex()
{
	return difTex;
}

const glm::vec3 &Material::getEmission()
{
	return emission;
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

void Mesh::createPlane()
{
	v.push_back(glm::vec3(-10.0f, 0.0f, -10.0f));
	v.push_back(glm::vec3(10.0f, 0.0f, -10.0f));
	v.push_back(glm::vec3(10.0f, 0.0f, 10.0f));
	v.push_back(glm::vec3(-10.0f, 0.0f, -10.0f));
	v.push_back(glm::vec3(10.0f, 0.0f, 10.0f));
	v.push_back(glm::vec3(-10.0f, 0.0f, 10.0f));
	n.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	n.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	n.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	n.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	n.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	n.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	t.push_back(glm::vec2(0.0f, 0.0f));
	t.push_back(glm::vec2(10.0f, 0.0f));
	t.push_back(glm::vec2(10.0f, 10.0f));
	t.push_back(glm::vec2(0.0f, 0.0f));
	t.push_back(glm::vec2(10.0f, 10.0f));
	t.push_back(glm::vec2(0.0f, 10.0f));

	drawMode = GL_TRIANGLES; // nesly mi quady nvm proc
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