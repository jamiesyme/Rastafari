#include "screen.h"

#include <SDL.h>


int          _screenWidth = 0;
int          _screenHeight = 0;
char*        _screenPixels = 0;
SDL_Texture* _screenTexture = 0;

extern SDL_Renderer* _sdlRenderer;


void initScreen(int width, int height)
{
	_screenWidth   = width;
	_screenHeight  = height;
	_screenPixels  = (char*)malloc(width * height * 3);
	_screenTexture = SDL_CreateTexture(_sdlRenderer,
	                                   SDL_PIXELFORMAT_RGB24,
	                                   SDL_TEXTUREACCESS_STREAMING,
	                                   width, height);
}


void killScreen()
{
	SDL_DestroyTexture(_screenTexture);
	free(_screenPixels);
	_screenWidth   = 0;
	_screenHeight  = 0;
	_screenPixels  = 0;
	_screenTexture = 0;
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
	SDL_UpdateTexture(_screenTexture,
	                  0, 
	                  _screenPixels,
	                  _screenWidth * sizeof(char) * 3);
	                  
	SDL_RenderClear(_sdlRenderer);
	SDL_RenderCopy(_sdlRenderer, _screenTexture, 0, 0);
	SDL_RenderPresent(_sdlRenderer);
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
