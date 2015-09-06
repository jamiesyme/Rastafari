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
	int px, py, npy, px1, py1, px2, py2;
	int dir, i;
	float slopeY, slopeZ;
	float z;

	// Translate the coordinates
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
	
	// Cull the line if it's clearly out of the z planes
	if (z1 < 0 && z2 < 0)
		return;
	if (z1 > 1 && z2 > 1)
		return;
	
	// Convert coords to pixels
	px1 = (int)floor(x1 * (float)_screenWidth);
	py1 = (int)floor(y1 * (float)_screenHeight);
	px2 = (int)floor(x2 * (float)_screenWidth);
	py2 = (int)floor(y2 * (float)_screenHeight);
	
	// Prepare for rendering
	px = px1;
	py = py1;
	z  = z1;
	
	setScreenPixel(px1, py1);
	setScreenPixel(px2, py2);
	
	// -- Render vertical line (edge-case)
	if (px1 - px2 == 0) {
		dir    = (py1 < py2 ? 1 : -1);
		slopeZ = (z2 - z1) / (float)(py2 - py1);
		while (1) {
			// Draw pixel
			if (z >= 0 && z <= 1)
				setScreenPixel(px, py);
				
			// Are we done?
			if (py == py2)
				return;
				
			// Move the pixel
			py += dir;
			//z   = z1 + slopeZ * (float)(py - py1);
		}
		
	// -- Render line
	} else {
		dir    = (px1 < px2 ? 1 : -1);
		slopeY = (float)(py2 - py1) / (float)(px2 - px1);
		slopeZ = (z2 - z1) / (float)(px2 - px1);
		while (1) {
			// Draw pixels
			npy = py1 + (int)round(slopeY * (float)(px + dir - px1));
			if (z >= 0 && z <= 1) {
				if (npy < py) {
					for (i = py; i > npy; i--) {
						setScreenPixel(px, i);
						if (i == py2)
							break;
					}
				} else if (py < npy) {
					for (i = py; i < npy; i++) {
						setScreenPixel(px, i);
						if (i == py2)
							break;
					}
				} else
					setScreenPixel(px, py);
			}
				
			// Are we done?
			if (px == px2 && py == py2)
				return;
				
			// Move the pixel
			px += dir;
			py = npy;
			//py  = py1 + (int)round(slopeY * (float)(px - px1));
			//z   = z1 + slopeZ * (float)(px - px1);
		}
	}
}


void drawCube(float x, float y, float z, float s, float ry)
{
	float vert[8][3];
	float xCosTheta, xSinTheta;
	float yCosTheta, ySinTheta;
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
	yCosTheta = cos(ry * 3.141592654f / 180.0f);
	ySinTheta = sin(ry * 3.141592654f / 180.0f);
	for (i = 0; i < 8; i++) {
		tmpX = vert[i][0];
		tmpZ = vert[i][2];
		vert[i][0] = tmpX * yCosTheta + tmpZ * ySinTheta;
		vert[i][2] = tmpZ * yCosTheta - tmpX * ySinTheta;
	}
	
	// Rotate the vertices 45 degrees on the x axis
	xCosTheta = cos(20.0f * 3.141592654f / 180.0f);
	xSinTheta = sin(20.0f * 3.141592654f / 180.0f);
	for (i = 0; i < 8; i++) {
		tmpY = vert[i][1];
		tmpZ = vert[i][2];
		vert[i][1] = tmpY * xCosTheta - tmpZ * xSinTheta;
		vert[i][2] = tmpY * xSinTheta + tmpZ * xCosTheta;
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
