#include "../global.h"
#include "modelAmbientSimple.h"
#include "../interation.h"

void eixo();
void cube();
void drawcubes();
void pointinitial();
void coordenadas();

void plane() {

    // coordenadas();

    pointinitial();

    drawcubes();
}

void coordenadas() {

    // glPushMatrix();
    // glScalef(0.0, 1.0, 0.0);
    // // Translatef(0.0, 0.0, 0.0);
    // eixo();
    // glPopMatrix();

    glPushMatrix();
    // glScalef(0.0, 3.0, 0.0);
    // glTranslatef(0.0, 1.5, 0.0);
    eixo();
    glPopMatrix();
}

void pointinitial() {
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glutSolidSphere(0.25, 20, 10);
    glPopMatrix();
}

void drawcubes() {
    glPushMatrix();
    glTranslatef(3.0, 0.0, 0.0);
    cube();
    glPopMatrix();

    glPushMatrix();
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.0, 0.0, 0.0);
    cube();
    glPopMatrix();
}

void cube() {
    glRotatef(x_angle, 1.0, 0.0, 0.0);
    glRotatef(y_angle, 0.0, 1.0, 0.0);

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, -0.25, 0.25);
    glVertex3f(0.25, -0.25, 0.25);
    glVertex3f(0.25, 0.25, 0.25);
    glVertex3f(-0.25, 0.25, 0.25);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.25, 0.25, 0.25);
    glVertex3f(0.25, -0.25, 0.25);
    glVertex3f(0.25, -0.25, -0.25);
    glVertex3f(0.25, 0.25, -0.25);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.25, -0.25, 0.25);
    glVertex3f(-0.25, -0.25, 0.25);
    glVertex3f(-0.25, -0.25, -0.25);
    glVertex3f(0.25, -0.25, -0.25);
    glEnd();
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, 0.25, 0.25);
    glVertex3f(0.25, 0.25, 0.25);
    glVertex3f(0.25, 0.25, -0.25);
    glVertex3f(-0.25, 0.25, -0.25);
    glEnd();
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, -0.25, -0.25);
    glVertex3f(-0.25, 0.25, -0.25);
    glVertex3f(0.25, 0.25, -0.25);
    glVertex3f(0.25, -0.25, -0.25);
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, 0.25, -0.25);
    glVertex3f(-0.25, -0.25, -0.25);
    glVertex3f(-0.25, -0.25, 0.25);
    glVertex3f(-0.25, 0.25, 0.25);
    glEnd();
}


// void eixo(float r, float g, float b, float d) {
void eixo() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.015625, -0.015625, 0.015625);
    glVertex3f(0.015625, -0.015625, 0.015625);
    glVertex3f(0.015625, 0.015625, 0.015625);
    glVertex3f(-0.015625, 0.015625, 0.015625);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.015625, 0.015625, 0.015625);
    glVertex3f(0.015625, -0.015625, 0.015625);
    glVertex3f(0.015625, -0.015625, -0.015625);
    glVertex3f(0.015625, 0.015625, -0.015625);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.015625, -0.015625, 0.015625);
    glVertex3f(-0.015625, -0.015625, 0.015625);
    glVertex3f(-0.015625, -0.015625, -0.015625);
    glVertex3f(0.015625, -0.015625, -0.015625);
    glEnd();
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.015625, 0.015625, 0.015625);
    glVertex3f(0.015625, 0.015625, 0.015625);
    glVertex3f(0.015625, 0.015625, -0.015625);
    glVertex3f(-0.015625, 0.015625, -0.015625);
    glEnd();
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.015625, -0.015625, -0.015625);
    glVertex3f(-0.015625, 0.015625, -0.015625);
    glVertex3f(0.015625, 0.015625, -0.015625);
    glVertex3f(0.015625, -0.015625, -0.015625);
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.015625, 0.015625, -0.015625);
    glVertex3f(-0.015625, -0.015625, -0.015625);
    glVertex3f(-0.015625, -0.015625, 0.015625);
    glVertex3f(-0.015625, 0.015625, 0.015625);
    glEnd();
}