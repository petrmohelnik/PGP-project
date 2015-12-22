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

#define W_WIDTH 1024
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
	string strVs, strFs, strPVs, strPGs, strPFs, strEmitPCs, strSortPCs, strSortLocalPCs,
		strSortLocalInnerPCs, strSimulatePCs, strGridDividePCs, strGridFindStartPCs,
		strSimulateDensityPCs, strSimulatePressurePCs, strSimulateForcePCs;
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
	if (!f.loadFile("resource/simulate_particle.comp", strSimulatePCs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/simulate_particle_density.comp", strSimulateDensityPCs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/simulate_particle_pressure.comp", strSimulatePressurePCs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/simulate_particle_force.comp", strSimulateForcePCs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/emit_particle.comp", strEmitPCs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/sort_particle.comp", strSortPCs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/sort_particle_local.comp", strSortLocalPCs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/sort_particle_local_inner.comp", strSortLocalInnerPCs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/grid_particle_divide.comp", strGridDividePCs)) {
		cin.get();
		return -1;
	}
	if (!f.loadFile("resource/grid_particle_find_start.comp", strGridFindStartPCs)) {
		cin.get();
		return -1;
	}
	GLuint vs, fs, Pvs, Pgs, Pfs, SimulatePCs, EmitPCs, SortPCs, SortLocalPCs, SortLocalInnerPCs,
		GridDividePCs, GridParticleStartPCs, SimulateDensityPCs, SimulatePressurePCs, SimulateForcePCs;
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
	if (!s.compileShader(strSimulatePCs.c_str(), GL_COMPUTE_SHADER, "simulate_particle_compute", SimulatePCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(SimulatePCs, "simulate_particle_compute_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strSimulateDensityPCs.c_str(), GL_COMPUTE_SHADER, "simulate_particle_density_compute", SimulateDensityPCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(SimulateDensityPCs, "simulate_particle_density_compute_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strSimulatePressurePCs.c_str(), GL_COMPUTE_SHADER, "simulate_particle_pressure_compute", SimulatePressurePCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(SimulatePressurePCs, "simulate_particle_pressure_compute_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strSimulateForcePCs.c_str(), GL_COMPUTE_SHADER, "simulate_particle_force_compute", SimulateForcePCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(SimulateForcePCs, "simulate_particle_force_compute_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strEmitPCs.c_str(), GL_COMPUTE_SHADER, "emit_particle_compute", EmitPCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(EmitPCs, "emit_particle_compute_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strSortPCs.c_str(), GL_COMPUTE_SHADER, "sort_particle_compute", SortPCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(SortPCs, "sort_particle_compute_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strSortLocalPCs.c_str(), GL_COMPUTE_SHADER, "sort_particle_local_compute", SortLocalPCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(SortLocalPCs, "sort_particle_local_compute_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strSortLocalInnerPCs.c_str(), GL_COMPUTE_SHADER, "sort_particle_local_inner_compute", SortLocalInnerPCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(SortLocalInnerPCs, "sort_particle_local_inner_compute_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strGridDividePCs.c_str(), GL_COMPUTE_SHADER, "grid_particle_divide_compute", GridDividePCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(GridDividePCs, "grid_particle_divide_compute_program")) {
		cin.get();
		return -1;
	}
	if (!s.compileShader(strGridFindStartPCs.c_str(), GL_COMPUTE_SHADER, "grid_particle_find_start_compute", GridParticleStartPCs)) {
		cin.get();
		return -1;
	}
	if (!s.linkProgram(GridParticleStartPCs, "grid_particle_start_compute_program")) {
		cin.get();
		return -1;
	}

	std::shared_ptr<Material> matGrid(new Material);
	Texture texGrid;
	if(!f.loadTexture("resource/grid.png", texGrid))
		return false;
	matGrid->setDifTex(texGrid);

	std::shared_ptr<Material> mat(new Material);
	Texture tex;
	if(!f.loadTexture("resource/white_D.png", tex))
		return false;
	mat->setDifTex(tex);

	Model modelPlane;
	std::shared_ptr<Mesh> planeMesh(new Mesh);
	planeMesh->createPlane();
	planeMesh->addMaterial(matGrid);
	modelPlane.addMesh(planeMesh);
	std::shared_ptr<BasicRenderer> planeRenderer(new BasicRenderer(glm::vec3(0.0f, 0.0f, 0.0f)));
	if(!planeRenderer->initRenderer(modelPlane, s.getProgram("basic_program")))
	{
		cin.get();
		return -1;
	}

	Model modelSphere;
	const unsigned int spheresCount = 6;
	const glm::vec3 spherePos[] =
	{
		glm::vec3(1.0f, 1.5f, 0.5f),
		glm::vec3(-0.7f, 0.7f, 0.2f),
		glm::vec3(0.2f, 1.0f, -0.7f),
		glm::vec3(0.0f, 2.5f, 0.0f),
		glm::vec3(-3.0f, 2.0f, 1.0f),
		glm::vec3(12.0f, 15.0f, 10.0f)
	};
	std::shared_ptr<Mesh> sphereMesh(new Mesh);
	sphereMesh->createSphere(0.3f, 108);
	sphereMesh->addMaterial(mat);
	modelSphere.addMesh(sphereMesh);
	std::shared_ptr<BasicRenderer> sphereRenderer[spheresCount];
	for(int i = 0; i < spheresCount; i++)
	{
		sphereRenderer[i] = std::make_shared<BasicRenderer>(spherePos[i]);
		if(!sphereRenderer[i]->initRenderer(modelSphere, s.getProgram("basic_program")))
		{
			cin.get();
			return -1;
		}
	}

	Model particles;
	std::shared_ptr<Mesh> particlesMesh(new Mesh);
	//particlesMesh->createParticles(1280000);
	std::shared_ptr<Material> particleMat(new Material);
	Texture particleTex;
	if (!f.loadTexture("resource/particle_D.png", particleTex))
		return false;
	particleMat->setDifTex(particleTex);
	particlesMesh->addMaterial(particleMat);
	particles.addMesh(particlesMesh);
	std::shared_ptr<ParticleSystemRenderer> ParticleSystemRenderer(new ParticleSystemRenderer(glm::vec3(0.0)));
	if (!ParticleSystemRenderer->initRenderer(particles, 200000, s.getProgram("particle_program"),
		s.getProgram("simulate_particle_compute_program"), s.getProgram("emit_particle_compute_program"),
		s.getProgram("sort_particle_compute_program"), s.getProgram("sort_particle_local_compute_program"),
		s.getProgram("sort_particle_local_inner_compute_program"),
		s.getProgram("grid_particle_divide_compute_program"), s.getProgram("grid_particle_start_compute_program"),
		s.getProgram("simulate_particle_density_compute_program"), s.getProgram("simulate_particle_pressure_compute_program"),
		s.getProgram("simulate_particle_force_compute_program"))) {
		cin.get();
		return -1;
	}

	std::shared_ptr<MainScene> scene(new MainScene);
	scene->setName("mainScene");
	for(int i = 0; i < spheresCount; i++)
		scene->addObject(sphereRenderer[i]);
	scene->addObject(planeRenderer);
	scene->addParticleSystem(ParticleSystemRenderer);
	scene->initCamera(45.0f, W_WIDTH, W_HEIGHT, 0.1f, 100.0f, CAM_TRANS_ROT);
	scene->getCamera()->translate(glm::vec3(0.0f, 1.5f, 2.0f));
	Light light(glm::vec3(10.0, 10.0, 10.0), glm::normalize(glm::vec3(0) - glm::vec3(10.0, 10.0, 10.0)));
	scene->addLight(light);
	scene->setAmbientLight(glm::vec3(0.1, 0.1, 0.1));

	app.addScene(scene);
	app.setActiveScene("mainScene");

	sdl.mainLoop(app);

	sdl.destroy();

	return 0;
}