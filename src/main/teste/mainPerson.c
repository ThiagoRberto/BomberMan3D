#include "../settings/structures.h"
#include "modelOne.h"
#include "character.h"

int abraco = 0;
float x_angle = 0.0;
float y_angle = 180.0;
float theta = 0.0;

GLUquadricObj* quadratic;

float x = 0.0;
float y = 0.0;
float z = 1.0;

int stop = 1;
int speed = 0;
int runAuxiliar = 0;

float d_frente = 0.0;
float d_lados = 0.0;
float v_vertical = 0.0;
float v_lados = 0.0;

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Testes de elementos");
    glutDisplayFunc(character);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutSpecialFunc(special);
    init();
    glutMainLoop();
    return 0;
}