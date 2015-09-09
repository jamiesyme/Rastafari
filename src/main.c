#include <SDL.h>
#include "window.h"
#include "screen.h"
#include "graphics.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#define CUBE_NUM 50
int cubes[CUBE_NUM * 3];


int main()
{
	int frameCount;
	int lastTicks;
	int i, j;
	float posZ;
	float rotX, rotY;
	float deltaTime;
	
	// Setup the cubes
	//srand(13);
	for (i = 0; i < CUBE_NUM; i++) {
		j = 5 + (rand() % 95);
		cubes[i * 3 + 2] = 10 + j;
		cubes[i * 3 + 0] = rand() % j - j / 2;
		cubes[i * 3 + 1] = rand() % j - j / 2;
	}
	
	// Prepare the canvas
	openWindow(800, 600);
	initScreen(800, 600);
	//setOrtho(-40, -30, 80, 60, 0.0f, 100.0f);
	setPersp(55.0f, 0.5f, 500.0f);
	loadIdentity();
	
	lastTicks = SDL_GetTicks();
	rotX = 0.f;
	rotY = 0.f;
	posZ = 0.0f;
	frameCount = 0;
	while (SDL_GetTicks() < 10000) {
	
		// Timing
		deltaTime = (float)(SDL_GetTicks() - lastTicks) / 1000.0f;
		lastTicks = SDL_GetTicks();
	
		// Translate
		loadIdentity();
		translate(0.0f, 0.0f, posZ);
		posZ += deltaTime * 5.0f;
	
		// Draw
		clearScreen();
		for (i = 0; i < CUBE_NUM; i++)
			drawCube(cubes[i * 3 + 0], 
			         cubes[i * 3 + 1], 
			         cubes[i * 3 + 2], 
				       3, 
				       rotX, rotY);
		renderScreen();
		
		// Rotate		
		rotX -= deltaTime * 5.0f;
		rotY -= deltaTime * 30.0f;
		
		frameCount++;
	}
	
	printf("Frames per second: %f\n", (float)frameCount / 10.0f);
	
	
	
	killScreen();
	closeWindow();	
	
	return 0;
}
