#include "screen.h"

#include <SDL.h>
#include <SDL_opengl.h>


int   _screenWidth = 0;
int   _screenHeight = 0;
char* _screenPixels = 0;

extern SDL_Window* _sdlWindow;


void initScreen(int width, int height)
{
	_screenWidth   = width;
	_screenHeight  = height;
	_screenPixels  = (char*)malloc(width * height * 3);
}


void killScreen()
{
	free(_screenPixels);
	_screenWidth  = 0;
	_screenHeight = 0;
	_screenPixels = 0;
}


void clearScreen()
{
	int i;
	for (i = 0; i < _screenWidth * _screenHeight; i++) {
		_screenPixels[i * 3 + 0] = 255;
		_screenPixels[i * 3 + 1] = 255;
		_screenPixels[i * 3 + 2] = 255;
	}
}


void renderScreen()
{
	glDrawPixels(_screenWidth, 
	             _screenHeight, 
	             GL_RGB, 
	             GL_UNSIGNED_BYTE, 
	             _screenPixels);
	
	SDL_GL_SwapWindow(_sdlWindow);
}


void setScreenPixel(int x, int y)
{
	y = _screenHeight - 1 - y;

	if (x < 0) return;
	if (y < 0) return;
	if (x >= _screenWidth)  return;
	if (y >= _screenHeight) return;
	
	int i = (y * _screenWidth + x) * 3;
	_screenPixels[i + 0] = 0;
	_screenPixels[i + 1] = 0;
	_screenPixels[i + 2] = 0;
}


int getScreenPixel(int x, int y)
{
	y = _screenHeight - 1 - y;

	if (x < 0) return 0;
	if (y < 0) return 0;
	if (x >= _screenWidth)  return 0;
	if (y >= _screenHeight) return 0;

	int i = (y * _screenWidth + x) * 3;
	if (_screenPixels[i + 0] ||
	    _screenPixels[i + 1] ||
	    _screenPixels[i + 2])
		return 1;
	return 0;
}
