#include "Application.h"

void Application::addScene(std::shared_ptr<Scene> s)
{
	scenes.push_back(s);
}

bool Application::setActiveScene(const char *name)
{
	for (unsigned int i = 0; i < scenes.size(); i++)	{
		if (strcmp(scenes[i]->getName(), name) == 0) {
			activeScene = scenes[i];
			return true;
		}
	}

	return false;
}

std::shared_ptr<Scene> Application::getScene(const char *name)
{
	for (unsigned int i = 0; i < scenes.size(); i++) {
		if (strcmp(scenes[i]->getName(), name) == 0) {
			return scenes[i];
		}
	}

	return nullptr;
}

void Application::display(Uint32 dt)
{
	activeScene->render(dt);
}

int Application::getNumParticles()
{
	return ((MainScene*)(activeScene.get()))->getNumParticles();
}

void Application::sdlEvent(SDL_Event &event)
{
	activeScene->handleSdlEvent(event);
}
