#include "screen.h"

#include <SDL.h>
#include <SDL_opengl.h>


int            _screenWidth  = 0;
int            _screenHeight = 0;
unsigned char* _screenPixels = 0;

extern SDL_Window* _sdlWindow;


void initScreen(int width, int height)
{
	// Setup the pixel data
	_screenWidth   = width;
	_screenHeight  = height;
	_screenPixels  = (unsigned char*)malloc(width * height * 4);
	
	// Initial the pixels
	clearScreen();
	
	// Disable any GL modifiers that are on by default
	glDisable(GL_DITHER);
	glDisable(GL_MULTISAMPLE);
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
	memset(_screenPixels, 255, _screenWidth * _screenHeight * 4);
}


void renderScreen()
{
	// Draw
	glDrawPixels(_screenWidth, 
	             _screenHeight, 
	             GL_RGBA, 
	             GL_UNSIGNED_BYTE, 
	             _screenPixels);
	
	// Present the screen
	SDL_GL_SwapWindow(_sdlWindow);
}


void setScreenPixel(int x, int y)
{
	y = _screenHeight - 1 - y;

	if (x < 0) return;
	if (y < 0) return;
	if (x >= _screenWidth)  return;
	if (y >= _screenHeight) return;
	
	int i = (y * _screenWidth + x) * 4;
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

	int i = (y * _screenWidth + x) * 4;
	if (_screenPixels[i + 0] ||
	    _screenPixels[i + 1] ||
	    _screenPixels[i + 2])
		return 1;
	return 0;
}
