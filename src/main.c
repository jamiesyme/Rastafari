#include "window.h"
#include "screen.h"
#include "graphics.h"
#include <unistd.h>


int main()
{
	openWindow(800, 600);
	initScreen(200, 150);
	setOrtho(-4, -3, 8, 6, -8, 16);
	
	//while (1) {
		clearScreen();
		
		//drawLine(-2, -2, 1, 2, 2, 2);
		drawCube(0, 0, 4, 3);
		
		renderScreen();
	//}
	sleep(3);
	
	
	
	killScreen();
	closeWindow();	
	
	return 0;
}
