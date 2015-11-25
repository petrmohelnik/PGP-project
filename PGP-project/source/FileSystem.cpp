#include "FileSystem.h"

std::vector<std::string> &FileSystem::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> FileSystem::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

bool FileSystem::loadFile(const char *path, std::string &buffer)
{
	char *b;

	std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		file.seekg(0, file.end);
		int length = (int)file.tellg();
		file.seekg(0, file.beg);

		b = new char[length + 1];

		file.read(b, length);
		file.close();

		b[length] = '\0';

		std::cout << "file " << path << " loaded" << std::endl;
	}
	else
	{
		std::cout << "Unable to open file " << path << std::endl;
		return false;
	}

	buffer.append(b);
	return true;
}

bool FileSystem::loadTexture(const char *path, Texture &img)
{
	unsigned int error = lodepng::decode(img.tex, img.width, img.height, path);

	if (error) {
		std::cout << path <<  ": Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return false;
	}

	return true;
}

bool FileSystem::parseObj(const char *path, Model &m)
{
	std::string line;
	std::ifstream fileStream(path);
	unsigned int ind = 0; //amout of indices
	Mesh mesh;

	std::vector<glm::vec3> v, n;
	std::vector<glm::vec2> t;

	if (fileStream.is_open())
	{
		while (fileStream.good())
		{
			getline(fileStream, line);

			std::vector<std::string> tokens = split(line, ' ');

			if (line.substr(0, 2) == "v ")
			{
				float x = (float)atof(tokens[1].c_str());
				float y = (float)atof(tokens[2].c_str());
				float z = (float)atof(tokens[3].c_str());
				v.push_back(glm::vec3(x, y, z));
			}

			else if (line.substr(0, 2) == "vt")
			{
				float x = (float)atof(tokens[1].c_str());
				float y = (float)atof(tokens[2].c_str());
				t.push_back(glm::vec2(x, y));
			}

			else if (line.substr(0, 2) == "vn")
			{
				float x = (float)atof(tokens[1].c_str());
				float y = (float)atof(tokens[2].c_str());
				float z = (float)atof(tokens[3].c_str());
				n.push_back(glm::vec3(x, y, z));
			}

			else if (line.substr(0, 2) == "f ")
			{
				glm::vec3 vertex, normal;
				glm::vec2 texCoord;

				std::vector<std::string> v1 = split(tokens[1], '/');
				int i = atoi(v1[0].c_str()) - 1;
				vertex.x = v[i].x; vertex.y = v[i].y; vertex.z = v[i].z;
				i = atoi(v1[1].c_str()) - 1;
				texCoord.x = t[i].x; texCoord.y = t[i].y;
				i = atoi(v1[2].c_str()) - 1;
				normal.x = n[i].x; normal.y = n[i].y; normal.z = n[i].z;
				mesh.addVertex(vertex, normal, texCoord);

				std::vector<std::string> v2 = split(tokens[2], '/');
				i = atoi(v2[0].c_str()) - 1;
				vertex.x = v[i].x; vertex.y = v[i].y; vertex.z = v[i].z;
				i = atoi(v2[1].c_str()) - 1;
				texCoord.x = t[i].x; texCoord.y = t[i].y;
				i = atoi(v2[2].c_str()) - 1;
				normal.x = n[i].x; normal.y = n[i].y; normal.z = n[i].z;
				mesh.addVertex(vertex, normal, texCoord);

				std::vector<std::string> v3 = split(tokens[3], '/');
				i = atoi(v3[0].c_str()) - 1;
				vertex.x = v[i].x; vertex.y = v[i].y; vertex.z = v[i].z;
				i = atoi(v3[1].c_str()) - 1;
				texCoord.x = t[i].x; texCoord.y = t[i].y;
				i = atoi(v3[2].c_str()) - 1;
				normal.x = n[i].x; normal.y = n[i].y; normal.z = n[i].z;
				mesh.addVertex(vertex, normal, texCoord);
			}
		}

		std::shared_ptr<Material> mat(new Material);
		Texture tex;
		if (!loadTexture("resource/white_D.png", tex))
			return false;
		mat->setDifTex(tex);
		mesh.addMaterial(mat);

		mesh.setDrawMode(GL_TRIANGLES);

		m.addMesh(std::make_shared<Mesh>(mesh));

		fileStream.close();
		return true;
	}

	std::cout << "Unable to open file " << path << std::endl;
	return false;
}

int FileSystem::findLastOpen(std::vector<bool> &closed)
{
	for (int i = closed.size() - 1; i >= 0; i--) {
		if (!closed[i])
			return i;
	}

	return -1;
}