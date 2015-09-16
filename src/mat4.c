#include "mat4.h"

#include <math.h>


#define PI 3.141592654f


void mat4_identity(mat4_t mat)
{
	mat[0]  = 1.0f;
	mat[1]  = 0.0f;
	mat[2]  = 0.0f;
	mat[3]  = 0.0f;
	
	mat[4]  = 0.0f;
	mat[5]  = 1.0f;
	mat[6]  = 0.0f;
	mat[7]  = 0.0f;
	
	mat[8]  = 0.0f;
	mat[9]  = 0.0f;
	mat[10] = 1.0f;
	mat[11] = 0.0f;
	
	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = 0.0f;
	mat[15] = 1.0f;
}


void mat4_perspective(mat4_t mat, float fov, float aspect, float n, float f)
{
	float tanFov = tan(fov * PI / 180.0f / 2.0f);
	
	mat4_identity(mat);
	mat[0]  = 1.0f / tanFov;
	mat[5]  = 1.0f / (tanFov * aspect);
	mat[10] = 1.0f / (f - n);
	mat[11] = (-n) * mat[10];
	
	//mat[0]  = 1 / (ar * tanFov);
	//mat[5]  = 1 / tanFov;
	//mat[10] = far / (far - near);
	//mat[11] = 1.0f;
	//mat[14] = -far * near / (far - near);
	//mat[15] = 0.0f;
}


void mat4_ortho(mat4_t mat, float w, float h, float n, float f)
{
	mat4_identity(mat);
	mat[0]  = 1.0f / w;
	mat[5]  = 1.0f / h;
	mat[10] = 1.0f / (f - n);
	mat[11] = (-n) * mat[10];
}


void mat4_rotationX(mat4_t mat, float deg)
{
	float rad, cosRad, sinRad;
	rad    = deg * PI / 180.0f;
	cosRad = cos(rad);
	sinRad = sin(rad);
	
	mat4_identity(mat);
	mat[5]  =  cosRad;
	mat[9]  = -sinRad;
	mat[6]  =  sinRad;
	mat[10] =  cosRad;
}


void mat4_rotationY(mat4_t mat, float deg)
{
	float rad, cosRad, sinRad;
	rad    = deg * PI / 180.0f;
	cosRad = cos(rad);
	sinRad = sin(rad);
	
	mat4_identity(mat);
	mat[0]  =  cosRad;
	mat[8]  =  sinRad;
	mat[2]  = -sinRad;
	mat[10] =  cosRad;
}


void mat4_rotationZ(mat4_t mat, float deg)
{
	float rad, cosRad, sinRad;
	rad    = deg * PI / 180.0f;
	cosRad = cos(rad);
	sinRad = sin(rad);
	
	mat4_identity(mat);
	mat[0] =  cosRad;
	mat[4] =  sinRad;
	mat[1] = -sinRad;
	mat[5] =  cosRad;
}


void mat4_translate(mat4_t mat, float x, float y, float z)
{
	mat[3]  += x;
	mat[7]  += y;
	mat[11] += z;
}


void mat4_rotateX(mat4_t mat, float deg)
{
	float matRot[16];
	mat4_rotationX(matRot, deg);
	mat4_mult(mat, matRot);
}


void mat4_rotateY(mat4_t mat, float deg)
{
	float matRot[16];
	mat4_rotationY(matRot, deg);
	mat4_mult(mat, matRot);
}


void mat4_rotateZ(mat4_t mat, float deg)
{
	float matRot[16];
	mat4_rotationZ(matRot, deg);
}


void mat4_copy(mat4_t mat1, mat4_t mat2)
{
	int i;
	for (i = 0; i < 16; i++)
		mat1[i] = mat2[i];
}


void mat4_mult(mat4_t mat1, mat4_t mat2)
{
	int x, y;
	float mat1t[16];
	mat4_copy(mat1t, mat1);
	
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			mat1[y*4+x] = mat1t[y*4+0] * mat2[x+0] + 
			              mat1t[y*4+1] * mat2[x+4] + 
			              mat1t[y*4+2] * mat2[x+8] +
			              mat1t[y*4+3] * mat2[x+12];
		}
	}
}
