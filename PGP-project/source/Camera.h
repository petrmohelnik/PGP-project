#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#define CAM_TRANS_ROT 0
#define CAM_ROT_TRANS 1

struct Light
{
	glm::vec3 pos;
	glm::vec3 dir;
	Light(glm::vec3 p, glm::vec3 d) : pos(p), dir(d) {}
};

class Camera
{
private:
	float fov;
	int width;
	int height;
	float nearPlane;
	float farPlane;
	glm::vec3 pos;
	glm::vec2 rot;
	int mode;
public:
	void init(float fov, int width, int height, float nearPlane, float farPlane, int _mode); //nacpat uhel pohledu a dalsi cipoviny
	void rotateX(float r);
	void rotateY(float r);
	void translate(const glm::vec3 &t);
	void setPosRot(const glm::vec3 &p, const glm::vec2 &r);
	void resize(int w, int h);
	glm::mat4 getProjection();
	glm::mat4 getProjectionOrtho(float l, float r, float b, float t);
	glm::mat4 getView();
	glm::vec3 getPos();
	glm::vec3 getDir();
	glm::vec2 getRotation() const;
	glm::vec2 getSize() const;
	void setMode(int mode);
	void setRange(float n, float f);
};

#endif //CAMERA_H