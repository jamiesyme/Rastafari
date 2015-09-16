#include "graphics.h"
#include "screen.h"
#include "mat4.h"
#include "vec3.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


float projMat[16];
float camMat[16];


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


void setOrtho(float w, float h, float near, float far)
{
	mat4_ortho(projMat, w, h, near, far);
}


void setPersp(float hFov, float near, float far)
{
	float aspectRatio = (float)_screenHeight / (float)_screenWidth;
	mat4_perspective(projMat, hFov, aspectRatio, near, far);
}


void loadIdentity()
{
	mat4_identity(camMat);
}


void translate(float x, float y, float z)
{
	// Negate the params because this camera should move all the objects in the
	// world around it.
	mat4_translate(camMat, -x, -y, -z);
}


void drawLine(float x1, float y1, float z1, 
              float x2, float y2, float z2,
              unsigned char colR,
              unsigned char colG,
              unsigned char colB)
{
	int px1, py1, px2, py2;
	float slopeX, slopeY, slopeZ;
	float v1[3], v2[3];
	
	// Set the vectors
	vec3_set(v1, x1, y1, z1);
	vec3_set(v2, x2, y2, z2);
	
	// Apply the camera
	vec3_multMat(v1, camMat);
	vec3_multMat(v2, camMat);
	
	printf("bV1: %f, %f, %f\n", v1[0], v1[1], v1[2]);
	//printf("bV2: %f, %f, %f\n", v2[0], v2[1], v2[2]);
	
	// Apply the projection
	vec3_multMat(v1, projMat);
	vec3_multMat(v2, projMat);
	
	printf("aV1: %f, %f, %f\n", v1[0], v1[1], v1[2]);
	//printf("aV2: %f, %f, %f\n\n", v2[0], v2[1], v2[2]);
	
	
	// Cull the line if it's clearly out of the planes
	if ((v1[0] < 0.0f && v2[0] < 0.0f) ||
	    (v1[0] > 1.0f && v2[0] > 1.0f) ||
	    (v1[1] < 0.0f && v2[1] < 0.0f) ||
	    (v1[1] > 1.0f && v2[1] > 1.0f) ||
	    (v1[2] < 0.0f && v2[2] < 0.0f) ||
	    (v1[2] > 1.0f && v2[2] > 1.0f))
		return;
		
		
	// Move some values around for convenience
	z1 = v1[2];
	z2 = v2[2];
	
	
	// Convert coords to pixels
	px1 = (int)floor(v1[0] * (float)_screenWidth);
	py1 = (int)floor(v1[1] * (float)_screenHeight);
	px2 = (int)floor(v2[0] * (float)_screenWidth);
	py2 = (int)floor(v2[1] * (float)_screenHeight);
	
	
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
	float modelMat[16];
	int i;

	// Half the size length
	s /= 2;
	
	// Calculate the cube vertices (no translation)
	vec3_set(vert[0], -s, -s, -s);
	vec3_set(vert[1],  s, -s, -s);	
	vec3_set(vert[2],  s, -s,  s);
	vec3_set(vert[3], -s, -s,  s);
	vec3_set(vert[4], -s,  s, -s);
	vec3_set(vert[5],  s,  s, -s);
	vec3_set(vert[6],  s,  s,  s);
	vec3_set(vert[7], -s,  s,  s);
	
	// Construct a model matrix
	mat4_rotationX(modelMat, rx);
	mat4_rotateY(modelMat, ry);
	mat4_translate(modelMat, x, y, z);
	
	// Rotate/Translate the vertices
	for (i = 0; i < 8; i++)
		vec3_multMat(vert[i], modelMat);
	
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
