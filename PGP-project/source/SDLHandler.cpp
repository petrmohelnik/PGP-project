#include "SDLHandler.h"

SDLHandler::SDLHandler(int w, int h)
{
	width = w;
	height = h;
	lastFPS = 0;
}

bool SDLHandler::init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "ERROR: Cannot initialize SDL\nSDL ERROR: " << SDL_GetError() << "\n";
		return false;
	}

	//request 4.5 OpenGL context nad core funcionality
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE/*SDL_GL_CONTEXT_PROFILE_COMPATIBILITY*/);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//create window
	mainwindow = SDL_CreateWindow("smoke", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!mainwindow) {
		std::cout << "ERROR: Cannot create window\nSDL ERROR: " << SDL_GetError() << "\n";
		return false;
	}

	//create opengl context and attach it to window
	maincontext = SDL_GL_CreateContext(mainwindow);
	if (!maincontext) {
		std::cout << "ERROR: Cannot create opengl context\nSDL ERROR: " << SDL_GetError() << "\n";
		return false;
	}

	//vertical synchronization
	SDL_GL_SetSwapInterval(1);

	return true;
}

void SDLHandler::destroy()
{
	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();
}

void SDLHandler::mainLoop(Application &app)
{
	// Window is not minimized
	//bool active = true;
	while (1)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			app.sdlEvent(event);
		}
		if (event.key.keysym.sym == SDLK_ESCAPE) 
				break;
		if (event.type == SDL_QUIT)
			break;

		Uint32 tics = SDL_GetTicks();
		if (lastTics == 0)
			lastTics = tics;
		Uint32 dt = tics - lastTics;

		Uint32 fps = static_cast<Uint32>((1.0f / static_cast<float>(dt) * 1000.0f));
		if ((fps < (lastFPS - 5)) || (fps >(lastFPS + 5))) // rozsah pro odstraneni blikani textu
		{
			std::stringstream ss;
			ss << "Smoke, FPS: " << fps;
			SDL_SetWindowTitle(mainwindow, ss.str().c_str());
			lastFPS = fps;
		}

		lastTics = tics;

		app.display(dt);
		SDL_GL_SwapWindow(mainwindow);
	}

}