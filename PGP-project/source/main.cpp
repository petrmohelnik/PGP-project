//#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <windows.h>
#include <iostream>
#include <memory>
#include "SDLHandler.h"
#include "FileSystem.h"
#include "BasicRenderer.h"
#include "ParticleSystemRenderer.h"
#include "Shader.h"
#include "Scene.h"
#include "Application.h"
#include "Model.h"
#ifdef main
#undef main //remove SDL's main() hook if it exists
#endif

#define W_WIDTH 800
#define W_HEIGHT 600

using namespace std;

/* Use nvidia graphics card */
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int main(int argc, char **argv)
{
	//init SDL
	SDLHandler sdl(W_WIDTH, W_HEIGHT);
	if (!sdl.init())
		return -1;

	//init glew
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		cout << "ERROR " << result << ": Initializing glew\n";
		return -1;
	}

	if (GLEW_VERSION_1_1) {
		cout << "----------------------------------------------------------------\n";
		cout << "Graphics Successfully Initialized\n";
		cout << "OpenGL Info\n";
		cout << "    Version: " << glGetString(GL_VERSION) << endl;
		cout << "     Vendor: " << glGetString(GL_VENDOR) << endl;
		cout << "   Renderer: " << glGetString(GL_RENDERER) << endl;
		cout << "    Shading: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
		cout << "----------------------------------------------------------------\n";
	}
	else {
		printf("Unable to get any OpenGL version from GLEW!");
	}

	Application app;
	FileSystem f;
	Shader s;
	string strVs, strFs, strPVs, strPGs, strPFs;
	if (!f.loadFile("resource/basic.vs", strVs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/basic.fs", strFs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/particle.vs", strPVs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/particle.gs", strPGs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/particle.fs", strPFs)) {
		cin.get();
		return -1;
	}
	GLuint vs, fs, Pvs, Pgs, Pfs;
	if (!s.compileShader(strVs.c_str(), GL_VERTEX_SHADER, "basic_vs", vs)) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strFs.c_str(), GL_FRAGMENT_SHADER, "basic_fs", fs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(vs, fs, "basic_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strPVs.c_str(), GL_VERTEX_SHADER, "particle_vs", Pvs)) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strPGs.c_str(), GL_GEOMETRY_SHADER, "particle_gs", Pgs)) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strPFs.c_str(), GL_FRAGMENT_SHADER, "particle_fs", Pfs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(Pvs, Pgs, Pfs, "particle_program")) {
		cin.get();
		return -1;
	}
	
	Model m;
	std::shared_ptr<Mesh> sphereMesh(new Mesh);
	sphereMesh->createSphere(1.0, 36);
	std::shared_ptr<Material> mat(new Material);
	Texture tex;
	if (!f.loadTexture("resource/white_D.png", tex))
		return false;
	mat->setDifTex(tex);
	sphereMesh->addMaterial(mat);
	m.addMesh(sphereMesh);
	std::shared_ptr<BasicRenderer> sphereRenderer(new BasicRenderer(glm::vec3(0.0)));
	if (!sphereRenderer->initRenderer(m, s.getProgram("basic_program"))) {
		cin.get();
		return -1;
	}

	Model particles;
	std::shared_ptr<Mesh> particlesMesh(new Mesh);
	particlesMesh->createParticles(100000);
	std::shared_ptr<Material> particleMat(new Material);
	Texture particleTex;
	if (!f.loadTexture("resource/particle_D.png", particleTex))
		return false;
	particleMat->setDifTex(particleTex);
	particlesMesh->addMaterial(particleMat);
	particles.addMesh(particlesMesh);
	std::shared_ptr<ParticleSystemRenderer> ParticleSystemRenderer(new ParticleSystemRenderer(glm::vec3(0.0)));
	if (!ParticleSystemRenderer->initRenderer(particles, s.getProgram("particle_program"))) {
		cin.get();
		return -1;
	}

	std::shared_ptr<MainScene> scene(new MainScene);
	scene->setName("mainScene");
	scene->addObject(sphereRenderer);
	scene->addObject(ParticleSystemRenderer);
	scene->initCamera(45.0f, W_WIDTH, W_HEIGHT, 0.1f, 1000.0f, CAM_TRANS_ROT);
	scene->getCamera()->translate(glm::vec3(0.0f, 0.0f, 2.0f));
	Light light(glm::vec3(10.0, 10.0, 10.0));
	scene->addLight(light);
	scene->setAmbientLight(glm::vec3(0.1, 0.1, 0.1));
	
	app.addScene(scene);
	app.setActiveScene("mainScene");

	sdl.mainLoop(app);

	sdl.destroy();

	return 0;
}