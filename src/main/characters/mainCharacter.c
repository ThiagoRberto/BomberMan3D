#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float x_angle = 0.0;
float y_angle = 0.0;
float theta = 0.0, thetaDead = 0.0;

GLUquadricObj* quadratic;

float x = 0.0;
float y = 0.0;
float z = 1.0;

int stop = 1, inclination = 0, died = 0;
int runAuxiliar = 0, heightAuxiliar = 0;
float height = 0.0;
float red, green, blue;

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

void running(int value) {
    if (stop && inclination == 0) {
        theta = 0;
        height = 0;
        runAuxiliar = 0;
        heightAuxiliar = 0;
        inclination = 0;
        glutPostRedisplay();
        return;
    }
    else if (stop && inclination >= 0) inclination--;
    else {
        if (theta >= 20) {
            runAuxiliar = 1;
        }
        else if (theta <= -20) {
            runAuxiliar = 0;
        }
        if (runAuxiliar) {
            value = value * -1;
        }

        theta += value * 0.3;

        if (!heightAuxiliar) {
            height = height + 0.0007;
            if (height >= 0.05) heightAuxiliar = 1;
        }
        else {
            height = height - 0.0007;
            if (height <= 0) heightAuxiliar = 0;
        }
        if (inclination <= 15) inclination++;
    }

    glutPostRedisplay();
    glutTimerFunc(10, running, 1);
}

void dying(int value) {
    thetaDead += 6;
    if (thetaDead >= 120) thetaDead = 120;

    glutPostRedisplay();
    glutTimerFunc(30, dying, 1);
}

void eyes() {
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.05, 0.6, 0.15);
    glScalef(0.6, 2.75, 0.6);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-0.05, 0.6, 0.15);
    glScalef(0.6, 2.75, 0.6);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();
}

void deadEyes() {
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.05, 0.6, 0.15);
    glRotatef(30, 0.0, 0.0, 1.0);
    glScalef(0.6, 2.75, 0.6);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.05, 0.6, 0.15);
    glRotatef(-30, 0.0, 0.0, 1.0);
    glScalef(0.6, 2.75, 0.6);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-0.05, 0.6, 0.15);
    glRotatef(30, 0.0, 0.0, 1.0);
    glScalef(0.6, 2.75, 0.6);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-0.05, 0.6, 0.15);
    glRotatef(-30, 0.0, 0.0, 1.0);
    glScalef(0.6, 2.75, 0.6);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();
}

void hat() {
    glPushMatrix();
    glTranslatef(0.0, 0.6, 0.0);
    glScalef(0.4, 0.3, 0.3);
    glRotatef(-120, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.0);
    glutSolidCone(0.3, 0.9, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.4, 0.4);
    glTranslatef(0.0, 0.85, -0.15);
    glutSolidSphere(0.05, 20, 10);
    glPopMatrix();
}

void face() {
    glPushMatrix();
    glColor3f(1.0, 0.6, 0.4);
    glTranslatef(0.0, 0.6, 0.13);
    glScalef(0.47, 0.37, 0.08);
    glutSolidSphere(0.25, 20, 10);
    glPopMatrix();
}

void head() {
    glPushMatrix();
    glColor3f(red, green, blue);
    glTranslatef(0.0, 0.6, 0.0);
    glScalef(0.4, 0.3, 0.3);
    glTranslatef(0.0, 0.0, 0.0);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();
    hat();
    if (!died) eyes();
    else deadEyes();
    face();
}

void buckle() {
    glPushMatrix();
    glColor3f(0.7, 0.7, 0.0);
    glTranslatef(0.0, 0.25, 0.125);

    glScalef(0.35, 0.2, 0.05);
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();
}

void belt() {
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, 0.25, 0.0);
    glRotatef(90, 1, 0.0, 0.0);

    glScalef(1.4, 1.3, 0.6);
    glutSolidTorus(0.05, 0.05, 20, 20);
    glPopMatrix();
}

void body() {
    glPushMatrix();
    glColor3f(0.4, 0.4, 0.7);
    glTranslatef(0.0, 0.25, 0.0);
    glScalef(0.3, 0.35, 0.25);
    glTranslatef(0.0, 0.25, 0.0);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();
    belt();
    buckle();
}

void hand() {
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.4);
    glTranslatef(0.0, -0.2, -0.01);
    glutSolidSphere(0.1, 20, 10);
    glPopMatrix();
}

void shoulder() {
    glPushMatrix();
    glScalef(1.1, 1.0, 1.13);
    glutSolidSphere(0.05, 20, 10);
    glPopMatrix();
}

void elbow() {
    glPushMatrix();
    glTranslatef(0.023, -0.18, -0.061);
    glutSolidSphere(0.027, 20, 10);
    glPopMatrix();
}


void rightArm() {
    glPushMatrix();
    glColor3f(red, green, blue);
    glTranslatef(0.14, 0.4, 0.0);

    if (died) glRotatef(thetaDead, 0.0, 0.0, 1.0);
    else glRotatef(theta * 5, 1, 0.0, 0.0);
    shoulder();

    glRotatef(20, 0.0, 0.0, 1.0);
    glRotatef(90, 1, 0.0, 0.0);

    glScalef(1.1, 1.0, 1.13);
    gluCylinder(quadratic, 0.05, 0.03, 0.16, 20, 10);
    glPopMatrix();

    // Antebraço
    glPushMatrix();
    glTranslatef(0.14, 0.4, 0.0);

    if (died) glRotatef(thetaDead, 0.0, 0.0, 1.0);
    else glRotatef(theta * 5, 1.0, 0.0, 0.0);
    glRotatef(20, -0.55, -0.5, 0.5);
    elbow();

    glTranslatef(0.02, -0.17, -0.05);
    int theta2 = theta * 5;
    if (theta2 >= 0) theta2 = 0;
    if (!died)glRotatef(theta2, 1.0, 0.0, 0.0);
    else glRotatef(thetaDead * 0.2, 0.0, 0.0, 1.0);

    hand();
    glRotatef(90, 1, 0.0, 0.0);
    glColor3f(red, green, blue);
    glScalef(0.1, 0.1, 0.1);

    gluCylinder(quadratic, 0.3, 0.25, 1.2, 20, 10);
    glPopMatrix();
}

void leftArm() {
    glPushMatrix();
    glScalef(-1.0, 1.0, 1.0);
    if (!died) theta = -theta;
    rightArm();
    glPopMatrix();
}

void feet() {
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.4);
    glTranslatef(0.0, -0.17, 0.075);
    glScalef(1.0, 0.65, 1.4);
    glutSolidSphere(0.1, 20, 10);
    glPopMatrix();
}

void thigh() {
    glPushMatrix();
    //glTranslatef(0.3, 0.35, 0.0);
    glScalef(1.17, 1.0, 1.1);
    glutSolidSphere(0.045, 20, 10);
    glPopMatrix();
}

void knee() {
    glPushMatrix();
    glTranslatef(0.0, -0.15, 0.0);
    glScalef(1.5, 1.0, 1.5);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();
}

void rightLeg() {
    float thetaLegs = theta;
    float thetaLegsDead = thetaDead;
    if (thetaLegsDead >= 35) thetaLegsDead = 35;
    if (thetaLegs <= -15) thetaLegs = -15;
    glPushMatrix();
    glColor3f(red, green, blue);
    glTranslatef(0.07, 0.18, 0.0);

    if (died) glRotatef(thetaLegsDead, 0.0, 0.0, 1.0);
    else glRotatef(-thetaLegs * 1.5, 1.0, 0.0, 0.0);
    glRotatef(7.5, 0, 0, 1);

    thigh();
    knee();

    glRotatef(90, 1, 0.0, 0.0);
    glScalef(0.1, 0.1, 0.1);
    gluCylinder(quadratic, 0.5, 0.35, 1.5, 20, 10);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.07, 0.18, 0.0);
    if (died) glRotatef(thetaLegsDead, 0.0, 0.0, 1.0);
    else glRotatef(-thetaLegs * 1.5, 1.0, 0.0, 0.0);
    glRotatef(7.5, 0, 0, 1);

    glTranslatef(0.0, -0.15, 0.0);

    int thetaLegs2 = thetaLegs;
    if (thetaLegs2 >= 0) thetaLegs = 0;
    glRotatef(-thetaLegs * 2.5, 1.0, 0.0, 0.0);
    feet();

    glColor3f(red, green, blue);
    glRotatef(90, 1, 0.0, 0.0);
    glScalef(0.1, 0.1, 0.1);
    gluCylinder(quadratic, 0.35, 0.3, 1.5, 20, 10);

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
    red = 0.1;
    green = 0.1;
    blue = 0.4;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glRotatef(x_angle, 1.0, 0.0, 0.0);
    glRotatef(y_angle, 0.0, 1.0, 0.0);
    if (!stop) glRotatef(-inclination, 1.0, 0.0, 0.0);

    glTranslatef(0.0, height, 0.0);
    body();
    rightArm();
    leftArm();
    rightLeg();
    leftLeg();

    if (!stop) {
        glTranslatef(0.0, 0.65, 0.05);
        glRotatef(inclination, 1.0, 0.0, 0.0);
        glTranslatef(0.0, -0.65, 0.0);
    }
    head();
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
        glutTimerFunc(10, running, 1);
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
    case 'X':
    case 'x':
        died = 1;
        glutTimerFunc(10, dying, 1);
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
    glutCreateWindow("BomberMan");
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    init();
    glutMainLoop();
    return 0;
}