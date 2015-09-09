#pragma once


void initScreen(int width, int height);

void killScreen();

void clearScreen();

void renderScreen();


void setScreenPixel(int x, int y, 
                    unsigned char cr, 
                    unsigned char cg, 
                    unsigned char cb);

void getScreenPixel(int x, int y,
                    unsigned char* cr,
                    unsigned char* cg,
                    unsigned char* cb);

