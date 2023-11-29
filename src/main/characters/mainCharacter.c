#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int abraco = 0;
float x_angle = 0.0;
float y_angle = 0.0;
float theta = 0.0;


float x = 0.0;
float y = 0.0;
float z = 1.0;

int stop = 1;
int speed = 0;
int runAuxiliar = 0;

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

    lightning();
}

void running(int value) {
    if (stop) {
        theta = 0;
        glutPostRedisplay();
        return;
    }
    if (theta >= 45) {
        runAuxiliar = 1;
    }
    else if (theta <= -45) {
        runAuxiliar = 0;
    }
    if (runAuxiliar) {
        value = value * -1;
    }
    theta += value;
    glutPostRedisplay();
    glutTimerFunc(10, running, 1);
}

void walking(int value) {
    if (stop || speed != 1) {
        theta = 0;
        glutPostRedisplay();
        return;
    }
    if (theta >= 25) {
        runAuxiliar = 1;
    }
    else if (theta <= -25) {
        runAuxiliar = 0;
    }
    if (runAuxiliar) {
        value = value * -1;
    }
    theta += value;
    glutPostRedisplay();
    glutTimerFunc(10, walking, 1);
}

void eyes() {
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.05, 0.65, 0.15);
    glScalef(0.75, 3, 0.1);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-0.05, 0.65, 0.15);
    glScalef(0.75, 3, 0.1);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();
}

void head() {
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.65, 0.0);
    glScalef(0.4, 0.3, 0.3);
    glTranslatef(0.0, 0.0, 0.0);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();
    eyes();
}

void body() {
    glPushMatrix();
    glColor3f(0.4, 0.4, 0.7);
    glTranslatef(0.0, 0.25, 0.0);
    glScalef(0.3, 0.4, 0.3);
    glTranslatef(0.0, 0.25, 0.0);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();
}

void hand() {
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.4);
    glTranslatef(-0.05, -0.4, 0.1);
    glutSolidSphere(0.1, 20, 10);
    glPopMatrix();
}

void rightArm() {
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.6, 0.0);

    glRotatef(theta * 1.8, 1, 0.0, 0.0);
    glRotatef(20, 0.0, 0.0, 1.0);
    glTranslatef(0.1, -0.05, 0.0);
    glScalef(0.1, 0.175, 0.1);
    glTranslatef(-0.2, -1, 0.0);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.6, 0.0);

    glRotatef(theta, 1.5, 0.0, 0.0);
    glTranslatef(0.25, -0.05, 0.0);
    glRotatef(theta, 2, 0.0, 0.0);
    hand();
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(20, -0.6, -0.5, 0.5);
    glScalef(0.1, 0.225, 0.1);
    glTranslatef(-1, -1.1, -0.15);
    glutSolidSphere(0.5, 10, 10);
    glPopMatrix();
}

void leftArm() {
    glPushMatrix();
    glScalef(-1.0, 1.0, 1.0);
    theta = -theta;
    rightArm();
    glPopMatrix();
}

void feet() {
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.4);
    glTranslatef(0.1, -0.5, 0.025);
    glScalef(1.0, 0.65, 1.4);
    glutSolidSphere(0.1, 20, 10);
    glPopMatrix();
}

void rightLeg() {
    float thetaLegs = theta;
    if (thetaLegs <= -15) thetaLegs = -15;
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.25, 0.0);

    glRotatef(thetaLegs * 1.15, 1.0, 0.0, 0.0);

    glRotatef(7.5, 0, 0, 1);
    glTranslatef(0.015, -0.225, 0.0);
    glRotatef(-thetaLegs, 1.5, 0.0, 0.0);
    glScalef(0.11, 0.3, 0.11);
    glTranslatef(0.3, 0.25, 0.25);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.25, 0.0);

    glRotatef(thetaLegs, 1.0, 0.0, 0.0);
    feet();
    glColor3f(1.0, 1.0, 1.0);

    glRotatef(7.5, 0, 0, 1);
    glTranslatef(0.015, -0.44, 0.0);
    glScalef(0.1, 0.375, 0.125);
    glTranslatef(0.25, 0.25, 0.25);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();

}

void leftLeg() {
    glPushMatrix();
    glScalef(-1.0, 1.0, 1.0);
    theta = -theta;
    rightLeg();
    glPopMatrix();
}

void draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glRotatef(x_angle, 1.0, 0.0, 0.0);
    glRotatef(y_angle, 0.0, 1.0, 0.0);

    head();
    body();
    rightArm();
    leftArm();
    rightLeg();
    leftLeg();
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
        stop = 0;
        glutTimerFunc(10, walking, 1);
        speed++;
        if (speed) glutTimerFunc(10, walking, 1);
        if (speed > 1) {
            speed = 0;
            glutTimerFunc(10, running, 1);
        }
        break;
    case 'S':
    case 's':
        stop = 1;
        break;
    case 'A':
    case 'a':
        break;
    case 'D':
    case 'd':
        break;
    case 'B':
    case 'b':
        abraco = 1;
        break;
    case 'P':
    case 'p':
        abraco = 0;
        x = 0.0;
        y = 0.0;
        z = 1.0;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
            printf("x = %i, y = %i\n", x, y);
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
    glutCreateWindow("Ex15");
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutSpecialFunc(special);
    init();
    glutMainLoop();
    return 0;
}