#include "vec3.h"


void vec3_zero(vec3_t vec)
{
	vec[0] = 0.0f;
	vec[1] = 0.0f;
	vec[2] = 0.0f;
}


void vec3_set(vec3_t vec, float x, float y, float z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}


void vec3_copy(vec3_t vec1, vec3_t vec2)
{
	vec1[0] = vec2[0];
	vec1[1] = vec2[1];
	vec1[2] = vec2[2];
}


void vec3_add(vec3_t vec1, vec3_t vec2)
{
	vec1[0] += vec2[0];
	vec1[1] += vec2[1];
	vec1[2] += vec2[2];
}


void vec3_sub(vec3_t vec1, vec3_t vec2)
{
	vec1[0] -= vec2[0];
	vec1[1] -= vec2[1];
	vec1[2] -= vec2[2];
}


void vec3_mult(vec3_t vec, float scalar)
{
	vec[0] *= scalar;
	vec[1] *= scalar;
	vec[2] *= scalar;
}


void vec3_div (vec3_t vec, float scalar)
{
	vec[0] /= scalar;
	vec[1] /= scalar;
	vec[2] /= scalar;
}


void vec3_multMat(vec3_t vec, mat4_t mat)
{
	float vecDup[3];
	vec3_copy(vecDup, vec);
	vec[0] = vecDup[0] * mat[0] + 
	         vecDup[1] * mat[1] +
	         vecDup[2] * mat[2] + mat[3];
	vec[1] = vecDup[0] * mat[4] +
	         vecDup[1] * mat[5] +
	         vecDup[2] * mat[6] + mat[7];
	vec[2] = vecDup[0] * mat[8] +
	         vecDup[1] * mat[9] +
	         vecDup[2] * mat[10] + mat[11];
}

