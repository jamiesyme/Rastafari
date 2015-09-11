#pragma once

//
// Results are always stored in the first arg.
//

typedef float* mat4_t;

typedef float* vec3_t;


void vec3_zero(vec3_t vec);

void vec3_set(vec3_t vec, float x, float y, float z);

void vec3_copy(vec3_t vec1, vec3_t vec2);

void vec3_add(vec3_t vec1, vec3_t vec2);
void vec3_sub(vec3_t vec1, vec3_t vec2);

void vec3_mult(vec3_t vec, float scalar);
void vec3_div (vec3_t vec, float scalar);

void vec3_multMat(vec3_t vec, mat4_t mat);
