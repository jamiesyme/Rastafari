#include <SDL.h>
#include "window.h"
#include "screen.h"
#include "graphics.h"
#include <unistd.h>


int main()
{
	int lastTicks;
	float rot;
	
	openWindow(800, 600);
	initScreen(200, 150);
	setOrtho(-4, -3, 8, 6, -8, 16);
	
	lastTicks = SDL_GetTicks();
	rot = 0.0f;
	while (SDL_GetTicks() < 3000) {
		clearScreen();
		
		//drawLine(-2, -2, 1, 2, 2, 2);
		drawCube(0, 0, 4, 3, rot);
		
		rot += (float)(SDL_GetTicks() - lastTicks) / 1000.0f * 90.0f;
		lastTicks = SDL_GetTicks();
		
		renderScreen();
	}
	
	
	
	killScreen();
	closeWindow();	
	
	return 0;
}
