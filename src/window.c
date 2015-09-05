#include "window.h"

#include <SDL.h>


SDL_Window*   _sdlWindow = 0;
SDL_Renderer* _sdlRenderer = 0;


void openWindow(int width, int height)
{
	SDL_CreateWindowAndRenderer(width, height, 0, &_sdlWindow, &_sdlRenderer);
}


void closeWindow()
{
	SDL_DestroyRenderer(_sdlRenderer);
	SDL_DestroyWindow(_sdlWindow);
	_sdlRenderer = 0;
	_sdlWindow = 0;
}
