#include <SDL.h>
#include "window.h"
#include "screen.h"
#include "graphics.h"
#include "vec3.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define CUBE_NUM 1
float cubes[CUBE_NUM * 3];


float genRand() {
	return (float)rand() / (float)RAND_MAX;
}


int main()
{
	int frameCount;
	int lastTicks;
	int i;
	float pos[3];
	float rot[3];
	float deltaTime;
	
	// Prepare the cubes
	//srand(13);
	for (i = 0; i < CUBE_NUM; i++) {
		cubes[i * 3 + 2] = 5.0f + genRand() * 95.0f;
		cubes[i * 3 + 0] = (genRand() - 0.5f) * cubes[i * 3 + 2];
		cubes[i * 3 + 1] = (genRand() - 0.5f) * cubes[i * 3 + 2];
	}
	cubes[0] = 0.0f;
	cubes[1] = 0.0f;
	cubes[2] = 0.0f;
	
	// Prepare the canvas
	openWindow(800, 600);
	initScreen(800, 600);
	//setOrtho(80, 60, 0.0f, 100.0f);
	setPersp(60.0f, 0.5f, 100.0f);
	loadIdentity();
	
	// Prepare the camera
	vec3_set(rot, 45.0f, 45.0f, 0.0f);
	//vec3_set(pos, -40.0f, -30.0f, -10.0f);
	vec3_set(pos, 0.0f, 0.0f, -10.0f);
	
	lastTicks = SDL_GetTicks();
	frameCount = 0;
	/*while (SDL_GetTicks() < 4000)*/ {
	
		// Timing
		deltaTime = (float)(SDL_GetTicks() - lastTicks) / 1000.0f;
		lastTicks = SDL_GetTicks();
	
		// Translate
		loadIdentity();
		translate(pos[0], pos[1], pos[2]);
		pos[2] -= deltaTime * 5.0f;
		
		// Draw
		clearScreen();
		for (i = 0; i < CUBE_NUM; i++)
			drawCube(cubes[i * 3 + 0], 
			         cubes[i * 3 + 1], 
			         cubes[i * 3 + 2], 
				       3, 
				       rot[0], rot[1]);
		renderScreen();
		
		// Rotate		
		//rotX -= deltaTime * 5.0f;
		//rotY -= deltaTime * 30.0f;
		
		frameCount++;
	}
	
	printf("Frames per second: %f\n", (float)frameCount / 10.0f);
	
	
	
	killScreen();
	closeWindow();	
	
	return 0;
}
