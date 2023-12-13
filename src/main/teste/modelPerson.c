#include "modelOne.h"
#include "../settings/structures.h"
#include "character.h"

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

void hat() {
    glPushMatrix();
    glTranslatef(0.0, 0.65, 0.0);
    glScalef(0.4, 0.3, 0.3);
    glRotatef(-120, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.0);
    glutSolidCone(0.3, 0.9, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.4, 0.4);
    glTranslatef(0.0, 0.9, -0.15);
    glutSolidSphere(0.05, 20, 10);
    glPopMatrix();
}

void face() {
    glPushMatrix();
    glColor3f(1.0, 0.6, 0.4);
    glTranslatef(0.0, 0.65, 0.13);
    glScalef(0.47, 0.37, 0.08);
    glutSolidSphere(0.25, 20, 10);
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
    hat();
    eyes();
    face();
}

void belt() {
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, 0.25, 0.0);
    glRotatef(90, 1, 0.0, 0.0);
    glScalef(1.5, 1.65, 0.8);
    glutSolidTorus(0.05, 0.05, 20, 10);
    glPopMatrix();
}

void body() {
    glPushMatrix();
    glColor3f(0.4, 0.4, 0.7);
    glTranslatef(0.0, 0.25, 0.0);
    glScalef(0.3, 0.45, 0.3);
    glTranslatef(0.0, 0.25, 0.0);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();
    belt();
}

void hand() {
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.4);
    glTranslatef(-0.05, -0.4, 0.1);
    glutSolidSphere(0.1, 20, 10);
    glPopMatrix();
}

void shoulder() {
    glPushMatrix();
    glTranslatef(-0.14, -0.05, 0.0);
    glScalef(1.3, 1.0, 1.15);
    glutSolidSphere(0.045, 20, 10);
    glPopMatrix();
}

void elbow() {
    glPushMatrix();
    glTranslatef(-0.07, -0.21, 0.0);
    glutSolidSphere(0.027, 20, 10);
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
    glTranslatef(-0.2, -0.5, 0.0);
    //glutSolidSphere(0.5, 20, 10);
    glRotatef(90, 1, 0.0, 0.0);
    gluCylinder(quadratic, 0.5, 0.3, 1, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.6, 0.0);

    glRotatef(theta, 1.5, 0.0, 0.0);
    glTranslatef(0.25, -0.05, 0.0);
    glRotatef(theta, 2, 0.0, 0.0);
    shoulder();
    elbow();
    hand();
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(20, -0.6, -0.5, 0.5);
    glScalef(0.1, 0.225, 0.1);
    glTranslatef(-1.1, -0.85, -0.3);
    //glutSolidSphere(0.5, 10, 10);
    glRotatef(90, 1, 0.0, 0.0);
    gluCylinder(quadratic, 0.3, 0.25, 1, 20, 10);
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
    glTranslatef(0.1, -0.5, 0.075);
    glScalef(1.0, 0.65, 1.4);
    glutSolidSphere(0.1, 20, 10);
    glPopMatrix();
}

void thigh() {
    glPushMatrix();
    glTranslatef(0.06, -0.1, 0.0);
    glScalef(1.17, 1.0, 1.1);
    glutSolidSphere(0.045, 20, 10);
    glPopMatrix();
}

void knee() {
    glPushMatrix();
    glTranslatef(0.085, -0.26, 0.0);
    glScalef(1.5, 1.0, 1.5);
    glutSolidSphere(0.025, 20, 10);
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
    glTranslatef(0.3, 0.35, 0.0);
    //glutSolidSphere(0.5, 20, 10);
    glRotatef(thetaLegs, 1.0, 0.0, 0.0);
    glRotatef(90, 1, 0.0, 0.0);
    gluCylinder(quadratic, 0.5, 0.35, 0.5, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.25, 0.0);

    glRotatef(thetaLegs, 1.0, 0.0, 0.0);
    thigh();
    knee();
    feet();
    glColor3f(1.0, 1.0, 1.0);

    glRotatef(7.5, 0, 0, 1);
    glTranslatef(0.015, -0.37, 0.0);
    glScalef(0.1, 0.375, 0.125);
    glTranslatef(0.35, 0.25, 0.0);
    //glutSolidSphere(0.5, 20, 10);
    glRotatef(90, 1, 0.0, 0.0);
    gluCylinder(quadratic, 0.35, 0.3, 0.5, 20, 10);
    glPopMatrix();

}

void leftLeg() {
    glPushMatrix();
    glScalef(-1.0, 1.0, 1.0);
    theta = -theta;
    rightLeg();
    glPopMatrix();
}

void character() {

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
