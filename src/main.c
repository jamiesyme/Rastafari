#include <SDL.h>
#include "window.h"
#include "screen.h"
#include "graphics.h"
#include <unistd.h>


int main()
{
	int lastTicks;
	float rotX, rotY;
	
	openWindow(800, 600);
	initScreen(200, 150);
	//setOrtho(-4, -3, 8, 6, 0.1f, 100.0f);
	setPersp(55.0f, 0.1f, 100.0f);
	
	lastTicks = SDL_GetTicks();
	//rotX = 0.0f;
	//rotY = -10.0f;
	rotX = -45.0f;
	rotY = -45.0f;
	while (SDL_GetTicks() < 10000) {
		clearScreen();
		
		drawCube(0, 0, 6, 
		         3, 
		         rotX, rotY);
		
		//rotY -= (float)(SDL_GetTicks() - lastTicks) / 1000.0f * 90.0f;
		lastTicks = SDL_GetTicks();
		
		renderScreen();
	}
	
	
	
	killScreen();
	closeWindow();	
	
	return 0;
}
