#pragma once


typedef float* mat4_t;


void mat4_identity(mat4_t mat);

void mat4_perspective(mat4_t mat, float fov, float near, float far);
void mat4_ortho(mat4_t mat, float w, float h, float near, float far);
                            
void mat4_rotationX(mat4_t mat, float deg);
void mat4_rotationY(mat4_t mat, float deg);
void mat4_rotationZ(mat4_t mat, float deg);
                            
void mat4_translate(mat4_t mat, float x, float y, float z);
void mat4_rotateX(mat4_t mat, float deg);
void mat4_rotateY(mat4_t mat, float deg);
void mat4_rotateZ(mat4_t mat, float deg);

void mat4_copy(mat4_t mat1, mat4_t mat2);

void mat4_mult(mat4_t mat1, mat4_t mat2);
