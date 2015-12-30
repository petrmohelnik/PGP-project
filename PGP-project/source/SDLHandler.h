#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <SDL/SDL.h>
#include <iostream>
#include <sstream>
#include "Application.h"

class SDLHandler
{
private:
	SDL_Window *mainwindow; //window handle
	SDL_GLContext maincontext; //context handle
	int width, height;
	Uint32 lastTics;
	Uint32 lastFPS;
	Uint32 prevNumParticles = 0;
public:
	SDLHandler(int w, int h);
	bool init();
	void destroy();
	void mainLoop(Application &app);
};

#endif //SDL_HANDLER_H