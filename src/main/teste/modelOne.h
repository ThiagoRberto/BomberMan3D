#ifndef MODEL_INTERATION_INCLUDED
#define MODEL_INTERATION_INCLUDED

#include <GL/glut.h>
#include <stdio.h>

extern int abraco;
extern float x_angle;
extern float y_angle;
extern float theta;

extern float x;
extern float y;
extern float z;

extern int stop;
extern int speed;
extern int runAuxiliar;

void special(int key, int x, int y);

void keyboard(unsigned char key, int x, int y);

void mouse(int button, int state, int x, int y);

void walking(int value);

void running(int value);

#endif // MODEL_INTERATION_INCLUDED