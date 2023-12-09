#include <GL/gl.h>
#include <GL/glut.h>
// #include <time.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct{
	float x;
	float z;
	int smer;
	int p;
}SphereWithSpikes;

GLfloat x = 0.0f;
GLfloat y = 0.0f;

GLfloat theta = 0.0f;
GLfloat alfa = 0.0f;

SphereWithSpikes sphereWithSpikes;

void drawSphereWithSpikes(int horizontalSpikes, int verticalSpikes) {
    // Draw the sphere
    glColor3f(0.0, 0.0, 0.0); 
    glutSolidSphere(0.5, 50, 50);

    // Draw spikes
    glColor3f(0.7, 0.7, 0.7);

    for (int i = 0; i < horizontalSpikes; ++i) {
        for (int j = 0; j < verticalSpikes; ++j) {
            double theta = (i * 2.0 * PI) / horizontalSpikes;
            double phi = (j * PI) / verticalSpikes;
            double x = 0.5 * sin(phi) * cos(theta);
            double y = 0.5 * sin(phi) * sin(theta);
            double z = 0.5 * cos(phi);

            // Draw a spike (cone)
            glPushMatrix();
            glTranslatef(x, y, z);
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            glutSolidCone(0.05, 0.2, 10, 10);
            glPopMatrix();
        }
    }
}

// void reshape(int width, int height){
//     glViewport(0, 0, width, height);
    
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     gluPerspective(60, (float)width/height, 1, 100);
//     // glutFullScreen();
// }

void keyboard(int key, int x, int y){
    switch (key){
        case GLUT_KEY_UP:
            alfa += 5.0;
            break;
        case GLUT_KEY_DOWN:
            alfa -= 5.0;
            break;
        case GLUT_KEY_LEFT:
            theta -= 5.0;
            break;
        case GLUT_KEY_RIGHT:
            theta += 5.0;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    switch (button){
        case 3:
            alfa -= 5.0;
            break;
        case 4:
            alfa += 5.0;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}


void display(){	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// gluLookAt(7, 5, 10,
	// 		  7, 0, 5,
	// 		  0, 1, 0);

    glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);

	// glPushMatrix();
	// 	if(sphereWithSpikes.p){
	// 		glPushMatrix();
    //             glRotatef(90, 0.0, 1.0, 0.0);
    //             glTranslatef(sphereWithSpikes.x, 0, sphereWithSpikes.z);    
    //             glTranslatef(x, y, 0);
    //             glRotatef(theta, 0.0, 1.0, 0.0);
    //             glRotatef(alfa, 1.0, 0.0, 0.0);
	// 			drawSphereWithSpikes(20, 10);
	// 		glPopMatrix();
	// 	}
	// glPopMatrix();

    glPushMatrix();
        glRotatef(90, 0.0, 1.0, 0.0);
        // glTranslatef(sphereWithSpikes.x, 0, sphereWithSpikes.z);    
        glTranslatef(x, y, 0);
        glRotatef(theta, 0.0, 1.0, 0.0);
        glRotatef(alfa, 1.0, 0.0, 0.0);
        drawSphereWithSpikes(20, 10);
    glPopMatrix();

	glutSwapBuffers();
}

void init(){
    glClearColor(0.07, 0.14, 0.20, 0);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    // srand(time(0));
    // sphereWithSpikes.smer = rand()%4;
	// sphereWithSpikes.p = 1;    
}

int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowPosition(0,0);
	glutInitWindowSize(800,800);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
    // glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutMouseFunc(mouse);
	
    init();

	glutMainLoop();
	
	return 0;
}