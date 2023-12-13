#include "global.h"
#include "interation.h"
#include "./settings/structures.h"


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
    case  '4':
        v_lados += 0.03;
        configCamera();
        break;
    case  '6':
        v_lados -= 0.03;
        configCamera();
        break;
    case  '8':
        v_vertical += 0.1;
        configCamera();
        break;
    case  '2':
        v_vertical -= 0.1;
        configCamera();
        break;
    case  '7':
        d_lados -= 0.1;
        configCamera();
        break;
    case  '9':
        d_lados += 0.1;
        configCamera();
        break;
    case  '1':
        d_frente -= 0.1;
        configCamera();
        break;
    case  '0':
        d_frente += 0.1;
        configCamera();
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