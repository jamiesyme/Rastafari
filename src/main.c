#include "window.h"
#include "screen.h"
#include <unistd.h>


int main()
{
	openWindow(800, 600);
	
	initScreen(200, 150);
	
	clearScreen();
	
	setScreenPixel(5, 1);
	
	renderScreen();
	
	sleep(5);
	
	killScreen();
	
	closeWindow();	
	
	return 0;
}
