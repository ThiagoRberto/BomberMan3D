#include "../global.h"
#include "structures.h"

void lightning()
{
    GLfloat light0_pos[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    GLfloat light1_pos[] = { -2.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    GLfloat direction[] = { 1.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);       //vetor direção
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);               //espalhamento angular
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.1f);              //atenuação angular

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void configCamera(void) {
    glMatrixMode(GL_MODELVIEW); //define que a matrix é a model view
    glLoadIdentity(); //carrega a matrix de identidade
    gluLookAt(0.0 + d_lados, 2.0, 4.8 + d_frente,   //posição da câmera
        0.0 + v_lados + d_lados, 0.6 + v_vertical, 0.0 + d_frente,   //para onde a câmera aponta
        0.0, 1.0, 0.0);  //vetor view-up
}

void init(void)
{
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.6f, 0.6f, 0.9f, 0.0f); //define a cor de fundo
    glEnable(GL_DEPTH_TEST); //habilita o teste de profundidade

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    configCamera();

    glMatrixMode(GL_PROJECTION); //define que a matrix é a de projeção
    glLoadIdentity(); //carrega a matrix de identidade
    gluPerspective(25.0, 1.0, 2.0, 8.0); //define uma projeção perspectiva
    glViewport(0, 0, 500, 500);
    quadratic = gluNewQuadric();
    lightning();
}
