#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  float x;
  float z;
  int ind;
  float bomb_pump;
} Bomb;

Bomb bomb = {0.0, 0.0, 0, 0.0};

void init() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, 1.0, 1.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void draw_bomb() {
  glPushMatrix();
      glTranslatef(bomb.x, 0.5, bomb.z);
      glColor3f(.7, 0, 0);
      glutSolidSphere(0.4 + cos((bomb.bomb_pump) / 5) * 0.05, 20, 20);
  glPopMatrix();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  draw_bomb();

  glFlush();
  glutSwapBuffers();
}

void timer(int value) {
  bomb.bomb_pump += 1.0;
  glutPostRedisplay();
  glutTimerFunc(16, timer, 0);
}

int main(int argc, char * argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 100);
  glutInitWindowSize(400, 400);
  glutCreateWindow("Bomb");
  init();
  glutDisplayFunc(display);
  glutTimerFunc(0, timer, 0);
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();

  return 0;
}