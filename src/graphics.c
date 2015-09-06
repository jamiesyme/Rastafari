#include "graphics.h"
#include "screen.h"
#include <math.h>


typedef struct {
	float x;
	float y;
	float w;
	float h;
	float near;
	float far;
} Ortho;


Ortho _projMatrix;

extern int _screenWidth;
extern int _screenHeight;


void setOrtho(float x, float y, float w, float h, float near, float far)
{
	_projMatrix.x = x;
	_projMatrix.y = y;
	_projMatrix.w = w;
	_projMatrix.h = h;
	_projMatrix.near = near;
	_projMatrix.far = far;
}


void drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
	// Offset the coordinates
	x1 -= _projMatrix.x;
	x2 -= _projMatrix.x;
	y1 -= _projMatrix.y;
	y2 -= _projMatrix.y;
	z1 -= _projMatrix.near;
	z2 -= _projMatrix.near;
	
	// Normalize the coordinates
	x1 /= _projMatrix.w;
	x2 /= _projMatrix.w;
	y1 /= _projMatrix.h;
	y2 /= _projMatrix.h;
	z1 /= _projMatrix.far;
	z2 /= _projMatrix.far;
	
	// Convert coords to pixels
	x1 *= (float)_screenWidth;
	x2 *= (float)_screenWidth;
	y1 *= (float)_screenHeight;
	y2 *= (float)_screenHeight;
	
	// Render the line vertices
	if (z1 >= 0 && z1 <= 1)
		setScreenPixel(x1, y1);
	if (z2 >= 0 && z2 <= 1)
		setScreenPixel(x2, y2);
}


void drawCube(float x, float y, float z, float s)
{
	float vert[8][3];
	float cosTheta;
	float sinTheta;
	float tmpX, tmpY, tmpZ;
	int i;

	// Half the size length
	s /= 2;
	
	// Calculate the cube vertices (no translation)
	vert[0][0] = -s;
	vert[0][1] = -s;
	vert[0][2] = -s;
	
	vert[1][0] =  s;
	vert[1][1] = -s;
	vert[1][2] = -s;
	
	vert[2][0] =  s;
	vert[2][1] = -s;
	vert[2][2] =  s;
	
	vert[3][0] = -s;
	vert[3][1] = -s;
	vert[3][2] =  s;
	
	vert[4][0] = -s;
	vert[4][1] =  s;
	vert[4][2] = -s;
	
	vert[5][0] =  s;
	vert[5][1] =  s;
	vert[5][2] = -s;
	
	vert[6][0] =  s;
	vert[6][1] =  s;
	vert[6][2] =  s;
	
	vert[7][0] = -s;
	vert[7][1] =  s;
	vert[7][2] =  s;
	
	// Rotate the vertices 45 degrees on the y axis
	cosTheta = cos(45 * 3.141592654f / 180.0f);
	sinTheta = sin(45 * 3.141592654f / 180.0f);
	for (i = 0; i < 8; i++) {
		tmpX = vert[i][0];
		tmpZ = vert[i][2];
		vert[i][0] = tmpX * cosTheta + tmpZ * sinTheta;
		vert[i][2] = tmpZ * cosTheta - tmpX * sinTheta;
	}
	
	// Rotate the vertices 45 degrees on the x axis
	for (i = 0; i < 8; i++) {
		tmpY = vert[i][1];
		tmpZ = vert[i][2];
		vert[i][1] = tmpY * cosTheta - tmpZ * sinTheta;
		vert[i][2] = tmpY * sinTheta + tmpZ * cosTheta;
	}
	
	// Translate the vertices
	for (i = 0; i < 8; i++) {
		vert[i][0] += x;
		vert[i][1] += y;
		vert[i][2] += z;
	}
	
	// Draw the lines
	for (i = 0; i < 3; i++) {
		drawLine(vert[i  ][0], vert[i  ][1], vert[i  ][2], 
	           vert[i+1][0], vert[i+1][1], vert[i+1][2]);
	}
	for (i = 4; i < 7; i++) {
		drawLine(vert[i  ][0], vert[i  ][1], vert[i  ][2], 
	           vert[i+1][0], vert[i+1][1], vert[i+1][2]);
	}
	drawLine(vert[3][0], vert[3][1], vert[3][2],
	         vert[0][0], vert[0][1], vert[0][2]);
	drawLine(vert[7][0], vert[7][1], vert[7][2],
	         vert[4][0], vert[4][1], vert[4][2]);
	for (i = 0; i < 4; i++) {
		drawLine(vert[i  ][0], vert[i  ][1], vert[i  ][2], 
	           vert[i+4][0], vert[i+4][1], vert[i+4][2]);
	}
}
