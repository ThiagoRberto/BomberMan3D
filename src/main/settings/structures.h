#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <GL/glut.h>

extern float d_lados;
extern float d_frente;
extern float v_vertical;
extern float v_lados;

extern GLUquadricObj* quadratic;

void lightning();

void init(void);

void configCamera();

#endif // CAMERA_H_INCLUDED