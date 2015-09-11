#include "graphics.h"
#include "screen.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


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

float _translateX;
float _translateY;
float _translateZ;


extern int _screenWidth;
extern int _screenHeight;


void swapi(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}


void swapf(float* a, float* b)
{
	float temp = *a;
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


void loadIdentity()
{
	_translateX = 0.0f;
	_translateY = 0.0f;
	_translateZ = 0.0f;
}


void translate(float x, float y, float z)
{
	_translateX += x;
	_translateY += y;
	_translateZ += z;
}


void drawLine(float x1, float y1, float z1, 
              float x2, float y2, float z2,
              unsigned char colR,
              unsigned char colG,
              unsigned char colB)
{
	int px1, py1, px2, py2;
	float slopeX, slopeY, slopeZ;
	
	// Apply the camera
	x1 -= _translateX;
	x2 -= _translateX;
	y1 -= _translateY;
	y2 -= _translateY;
	z1 -= _translateZ;
	z2 -= _translateZ;

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
		x1 = x1 / (_perspProj.hFovTan * fabs(z1)) * 0.5f + 0.5f;
		x2 = x2 / (_perspProj.hFovTan * fabs(z2)) * 0.5f + 0.5f;
		y1 = y1 / (_perspProj.vFovTan * fabs(z1)) * 0.5f + 0.5f;
		y2 = y2 / (_perspProj.vFovTan * fabs(z2)) * 0.5f + 0.5f;
		z1 = (z1 - _perspProj.near) / (_perspProj.far - _perspProj.near);
		z2 = (z2 - _perspProj.near) / (_perspProj.far - _perspProj.near);
	}
	
	// Cull the line if it's clearly out of the planes
	if ((x1 < 0.0f && x2 < 0.0f) ||
	    (x1 > 1.0f && x2 > 1.0f) ||
	    (y1 < 0.0f && y2 < 0.0f) ||
	    (y1 > 1.0f && y2 > 1.0f) ||
	    (z1 < 0.0f && z2 < 0.0f) ||
	    (z1 > 1.0f && z2 > 1.0f))
		return;
	
	// Convert coords to pixels
	px1 = (int)floor(x1 * (float)_screenWidth);
	py1 = (int)floor(y1 * (float)_screenHeight);
	px2 = (int)floor(x2 * (float)_screenWidth);
	py2 = (int)floor(y2 * (float)_screenHeight);
	
	// Render vertical line
	if (px1 == px2) {
	
		// Put p1 < p2
		if (py1 > py2) {
			swapi(&py1, &py2);
			swapf(&z1, &z2);
		}
		
		// Calculate the slopes
		slopeZ = (z2 - z1) / (float)(py2 - py1);
		
		// Clamp pixels to screen
		if (py1 < 0)
			py1 = 0;
		if (py2 >= _screenHeight)
			py2 = _screenHeight - 1;
			
		// Draw pixels
		for (; py1 <= py2; py1++) {
			if (z1 >= 0.0f && z1 <= 1.0f)
				setScreenPixel(px1, py1, z1, colR, colG, colB);
			z1 += slopeZ;
		}
			
		return;
	}
		
	// Render horizontal line
	if (py1 == py2) {
	
		// Put p1 < p2
		if (px1 > px2) {
			swapi(&px1, &px2);
			swapf(&z1, &z2);
		}
		
		// Calculate the slopes	
		slopeZ = (z2 - z1) / (float)(px2 - px1);
			
		// Clamp pixels to screen
		if (px1 < 0)
			px1 = 0;
		if (px2 >= _screenWidth)
			px2 = _screenWidth - 1;
			
		// Draw pixels
		for (; px1 <= px2; px1++) {
			if (z1 >= 0.0f && z1 <= 1.0f)
				setScreenPixel(px1, py1, z1, colR, colG, colB);
			z1 += slopeZ;
		}
			
		return;
	}
	
	// Render horizontal-ish line
	if (abs(px2 - px1) > abs(py2 - py1)) {
		
		// Put p1 < p2
		if (px1 > px2) {
			swapi(&px1, &px2);
			swapi(&py1, &py2);
			swapf(&z1, &z2);
		}
		
		// Calculate slopes
		slopeY = (float)(py2 - py1) / (float)(px2 - px1);
		slopeZ = (z2 - z1) / (float)(px2 - px1);
		
		// Clamp pixel to screen
		y1 = (float)py1;
		if (px1 < 0) {
			y1 += slopeY * (float)(0 - px1);
			z1 += slopeZ * (float)(0 - px1);
			px1 = 0;
		}
		if (px2 >= _screenWidth)
			px2 = _screenWidth - 1;
		
		// Draw pixels
		for (; px1 <= px2; px1++) {
			if (z1 >= 0.0f && z1 <= 1.0f &&
			    (int)y1 >= 0 && (int)y1 < _screenHeight)
				setScreenPixel(px1, (int)y1, z1, colR, colG, colB);
			y1 += slopeY;
			z1 += slopeZ;
		}
		
		return;
	}
	
	// Render vertical-ish line
	
	// Put p1 < p2
	if (py1 > py2) {
		swapi(&py1, &py2);
		swapi(&px1, &px2);
		swapf(&z1, &z2);
	}
	
	// Calculate slopes
	slopeX = (float)(px2 - px1) / (float)(py2 - py1);
	slopeZ = (z2 - z1) / (float)(py2 - py1);
	
	// Clamp pixels to screen
	x1 = (float)px1;
	if (py1 < 0) {
		x1 += slopeX * (float)(0 - py1);
		z1 += slopeZ * (float)(0 - py1);
		py1 = 0;
	}
	if (py2 >= _screenHeight)
		py2 = _screenHeight - 1;
	
	// Draw pixels
	for (; py1 <= py2; py1++) {
		if (z1 >= 0.0f && z1 <= 1.0f &&
		    (int)x1 >= 0 && (int)x1 < _screenWidth)
			setScreenPixel((int)x1, py1, z1, colR, colG, colB);
		x1 += slopeX;
		z1 += slopeZ;
	}
	
	return;
}


void drawCube(float x, float y, float z, 
              float s, 
              float rx, float ry)
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
	
	// Rotate the vertices ry degrees on the y axis
	yCosTheta = cos(ry * 3.141592654f / 180.0f);
	ySinTheta = sin(ry * 3.141592654f / 180.0f);
	for (i = 0; i < 8; i++) {
		tmpX = vert[i][0];
		tmpZ = vert[i][2];
		vert[i][0] = tmpX * yCosTheta + tmpZ * ySinTheta;
		vert[i][2] = tmpZ * yCosTheta - tmpX * ySinTheta;
	}
	
	// Rotate the vertices rx degrees on the x axis
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
	
	// Draw the bottom square
	drawLine(vert[0][0], vert[0][1], vert[0][2], 
	         vert[1][0], vert[1][1], vert[1][2],
	         0, 0, 0);
	drawLine(vert[1][0], vert[1][1], vert[1][2], 
	         vert[2][0], vert[2][1], vert[2][2],
	         0, 0, 0);
	drawLine(vert[2][0], vert[2][1], vert[2][2], 
	         vert[3][0], vert[3][1], vert[3][2],
	         0, 0, 0);
	drawLine(vert[3][0], vert[3][1], vert[3][2], 
	         vert[0][0], vert[0][1], vert[0][2],
	         0, 0, 0);
	
	// Draw the top square
	drawLine(vert[4][0], vert[4][1], vert[4][2], 
	         vert[5][0], vert[5][1], vert[5][2],
	         255, 0, 0);
	drawLine(vert[5][0], vert[5][1], vert[5][2], 
	         vert[6][0], vert[6][1], vert[6][2],
	         255, 0, 0);
	drawLine(vert[6][0], vert[6][1], vert[6][2], 
	         vert[7][0], vert[7][1], vert[7][2],
	         255, 0, 0);
	drawLine(vert[7][0], vert[7][1], vert[7][2], 
	         vert[4][0], vert[4][1], vert[4][2],
	         255, 0, 0);
	         
	// Draw the connecting lines
	drawLine(vert[0][0], vert[0][1], vert[0][2], 
	         vert[4][0], vert[4][1], vert[4][2],
	         0, 255, 255);
	drawLine(vert[1][0], vert[1][1], vert[1][2], 
	         vert[5][0], vert[5][1], vert[5][2],
	         255, 255, 0);
	drawLine(vert[2][0], vert[2][1], vert[2][2], 
	         vert[6][0], vert[6][1], vert[6][2],
	         255, 0, 255);
	drawLine(vert[3][0], vert[3][1], vert[3][2], 
	         vert[7][0], vert[7][1], vert[7][2],
	         0, 255, 0);
}
