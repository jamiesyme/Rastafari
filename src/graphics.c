#include "graphics.h"
#include "screen.h"
#include <stdlib.h>
#include <math.h>


typedef struct {
	float x;
	float y;
	float w;
	float h;
	float near;
	float far;
} Ortho;

typedef struct {
	float vFovTan;
	float hFovTan;
	float near;
	float far;
} Perspective;


Ortho       _orthoProj;
Perspective _perspProj;
int         _projMode; // 1 == Ortho, 2 == Perspective


extern int _screenWidth;
extern int _screenHeight;


void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}


void setOrtho(float x, float y, float w, float h, float near, float far)
{
	_orthoProj.x    = x;
	_orthoProj.y    = y;
	_orthoProj.w    = w;
	_orthoProj.h    = h;
	_orthoProj.near = near;
	_orthoProj.far  = far;
	_projMode       = 1;
}


void setPersp(float vFov, float near, float far)
{
	float aspect = (float)_screenWidth / (float)_screenHeight;
	_perspProj.vFovTan = tan(vFov / 2.0f * 3.141592654f / 180.0f);
	_perspProj.hFovTan = _perspProj.vFovTan * aspect;
	_perspProj.near    = near;
	_perspProj.far     = far;
	_projMode          = 2;
}


void drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
	int px1, py1, px2, py2;
	float slopeX, slopeY;

	// Apply projection matrix
	if (_projMode == 1) {
		// Translate the coordinates
		x1 -= _orthoProj.x;
		x2 -= _orthoProj.x;
		y1 -= _orthoProj.y;
		y2 -= _orthoProj.y;
		z1 -= _orthoProj.near;
		z2 -= _orthoProj.near;
	
		// Normalize the coordinates
		x1 /= _orthoProj.w;
		x2 /= _orthoProj.w;
		y1 /= _orthoProj.h;
		y2 /= _orthoProj.h;
		z1 /= (_orthoProj.far - _orthoProj.near);
		z2 /= (_orthoProj.far - _orthoProj.near);
		
	} else if (_projMode == 2) {
		// Normalize the coordinates
		x1 = x1 / (_perspProj.hFovTan * z1) * 0.5f + 0.5f;
		x2 = x2 / (_perspProj.hFovTan * z2) * 0.5f + 0.5f;
		y1 = y1 / (_perspProj.vFovTan * z1) * 0.5f + 0.5f;
		y2 = y2 / (_perspProj.vFovTan * z2) * 0.5f + 0.5f;
		z1 = (z1 - _perspProj.near) / (_perspProj.far - _perspProj.near);
		z2 = (z2 - _perspProj.near) / (_perspProj.far - _perspProj.near);
	}
	
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
	
	// Render vertical line
	if (px1 == px2) {
	
		if (py1 > py2)
			swap(&py1, &py2);
			
		for (; py1 <= py2; py1++)
			setScreenPixel(px1, py1);
			
		return;
	}
		
	// Render horizontal line
	if (py1 == py2) {
	
		if (px1 > px2)
			swap(&px1, &px2);
			
		for (; px1 <= px2; px1++)
			setScreenPixel(px1, py1);
			
		return;
	}
	
	// Render horizontal-ish line
	if (abs(px2 - px1) > abs(py2 - py1)) {
		
		if (px1 > px2) {
			swap(&px1, &px2);
			swap(&py1, &py2);
		}
		
		slopeY = (float)(py2 - py1) / (float)(px2 - px1);
		
		for (; px1 <= px2; px1++) {
			py1 = py2 - (int)((float)(px2 - px1) * slopeY);
			setScreenPixel(px1, py1);
		}
		
		return;
	}
	
	// Render vertical-ish line
	if (py1 > py2) {
		swap(&py1, &py2);
		swap(&px1, &px2);
	}
	
	slopeX = (float)(px2 - px1) / (float)(py2 - py1);
	
	for (; py1 <= py2; py1++) {
		px1 = px2 - (int)((float)(py2 - py1) * slopeX);
		setScreenPixel(px1, py1);
	}
	
	return;
}


void drawCube(float x, float y, float z, float s, float rx, float ry)
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
	xCosTheta = cos(rx * 3.141592654f / 180.0f);
	xSinTheta = sin(rx * 3.141592654f / 180.0f);
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
