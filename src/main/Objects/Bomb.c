#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float x_angle = 0.0;
float y_angle = 0.0;
float theta = 0.0;

GLUquadricObj* quadratic;

float x = 0.0;
float y = 0.0;
float z = 1.0;

int stop = 1;
int auxiliar = 0;
float bomb_pump = 0;
float red = 0.1;

void lightning() {
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

void init(void)
{
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.6f, 0.6f, 0.9f, 0.0f); //define a cor de fundo
    glEnable(GL_DEPTH_TEST); //habilita o teste de profundidade

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_MODELVIEW); //define que a matrix é a model view
    glLoadIdentity(); //carrega a matrix de identidade
    gluLookAt(0.0, 0.0, 4.0,   //posição da câmera
        0.0, 0.0, 0.0,   //para onde a câmera aponta
        0.0, 1.0, 0.0);  //vetor view-up

    glMatrixMode(GL_PROJECTION); //define que a matrix é a de projeção
    glLoadIdentity(); //carrega a matrix de identidade
    gluPerspective(25.0, 1.0, 2.0, 8.0); //define uma projeção perspectiva
    glViewport(0, 0, 500, 500);
    quadratic = gluNewQuadric();
    lightning();
}

void exploding(int value) {
    if (auxiliar) {
        red = 0.5;
        auxiliar = 0;
    }
    else {
        red = 0.1;
        auxiliar = 1;
    }
    bomb_pump += 1.0;
    glutPostRedisplay();
    glutTimerFunc(50, exploding, 1);
}

void cover() {
    glPushMatrix();
    glScalef(1.0, 0.1, 1.0);
    glutSolidSphere(0.15, 15, 15);
    glPopMatrix();
}

void wick() {
    glPushMatrix();
    glColor3f(0.2, 0.0, 0.0);
    glTranslatef(0.0, 0.0, -0.15);
    gluCylinder(quadratic, 0.02, 0.02, 0.2, 20, 10);
    glPopMatrix();
}

void bodyDetail() {
    glPushMatrix();
    glTranslatef(0.0, 0.55, 0.0);
    cover();
    glRotatef(90, 1, 0.0, 0.0);
    gluCylinder(quadratic, 0.15, 0.15, 0.1, 20, 10);
    wick();
    glPopMatrix();
}

void body() {
    glPushMatrix();
    glColor3f(red, 0.1, 0.1);
    glutSolidSphere(0.5, 30, 30);
    glPopMatrix();
    bodyDetail();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glRotatef(x_angle, 1.0, 0.0, 0.0);
    glRotatef(y_angle, 0.0, 1.0, 0.0);

    glScalef(1.0 + cos((bomb_pump) / 5) * 0.05, 1.0 + cos((bomb_pump) / 5) * 0.05, 1.0 + cos((bomb_pump) / 5) * 0.05);
    body();

    glPopMatrix();
    glFlush();
}

void special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_RIGHT:
        y_angle += 2;
        break;
    case GLUT_KEY_LEFT:
        y_angle -= 2;
        break;
    case GLUT_KEY_UP:
        x_angle += 2;
        break;
    case GLUT_KEY_DOWN:
        x_angle -= 2;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'W':
    case 'w':
        auxiliar = 1;
        glutTimerFunc(10, exploding, 1);
        break;
    case 'S':
    case 's':
        auxiliar = 0;
        break;
    case 'A':
    case 'a':
        break;
    case 'D':
    case 'd':
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Bomb");
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    init();
    glutMainLoop();
    return 0;
}