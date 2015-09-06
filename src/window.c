#include "window.h"

#include <SDL.h>
#include <SDL_opengl.h>


SDL_Window*   _sdlWindow = 0;
SDL_GLContext _glContext;


void openWindow(int width, int height)
{
	SDL_Init(SDL_INIT_VIDEO | 
	         SDL_INIT_TIMER);
	_sdlWindow = SDL_CreateWindow("Rasterizer",
	                              SDL_WINDOWPOS_CENTERED,
	                              SDL_WINDOWPOS_CENTERED,
	                              width, height,
	                              SDL_WINDOW_OPENGL);
	_glContext = SDL_GL_CreateContext(_sdlWindow);
}


void closeWindow()
{
	SDL_GL_DeleteContext(_glContext);
	SDL_DestroyWindow(_sdlWindow);
	SDL_Quit();
	_sdlWindow = 0;
}
