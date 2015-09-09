#pragma once


void setOrtho(float x, float y, float w, float h, float near, float far);

void setPersp(float vFov, float near, float far);

void loadIdentity();

void translate(float x, float y, float z);

void drawLine(float x1, float y1, float z1, 
              float x2, float y2, float z2,
              unsigned char colR,
              unsigned char colG,
              unsigned char colB);

void drawCube(float x, float y, float z, 
              float s, 
              float rx, float ry);
