#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <math.h>

typedef struct{
	float x;
	float z;
	int direction;
	int p;
}SphereWithSpikes;;

GLfloat x = 0.0f;
GLfloat y = 0.0f;

GLfloat theta = 0.0f;
GLfloat alfa = 0.0f;

GLuint textureID;

void loadTexture() {
    textureID = SOIL_load_OGL_texture(
        "./src/main/Texture/spike.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (textureID == 0) {
        printf("Erro ao carregar a textura: %s\n", SOIL_last_result());
        exit(1);
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void customSolidSphere(double radius, int slices, int stacks) {
    const double pi = 3.14159265358979323846;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    for (int i = 0; i < stacks; ++i) {
        double lat0 = pi * (-0.5 + (double)(i) / stacks);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = pi * (-0.5 + (double)(i + 1) / stacks);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            double lng = 2 * pi * (double)(j - 1) / slices;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3d(x * zr0, y * zr0, z0);
            glTexCoord2d((double)j / slices, (double)i / stacks);
            glVertex3d(radius * x * zr0, radius * y * zr0, radius * z0);

            glNormal3d(x * zr1, y * zr1, z1);
            glTexCoord2d((double)j / slices, (double)(i + 1) / stacks);
            glVertex3d(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}

void drawSolidCube(float size) {
    float halfSize = size * 0.5;

    glBegin(GL_QUADS);

    // Face frontal
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(halfSize, -halfSize, halfSize);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(halfSize, halfSize, halfSize);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-halfSize, halfSize, halfSize);

    // Face traseira
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(halfSize, halfSize, -halfSize);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-halfSize, halfSize, -halfSize);

    // Face esquerda
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-halfSize, -halfSize, halfSize);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-halfSize, halfSize, halfSize);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-halfSize, halfSize, -halfSize);

    // Face direita
    glTexCoord2f(1.0, 0.0);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(halfSize, -halfSize, halfSize);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(halfSize, halfSize, halfSize);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(halfSize, halfSize, -halfSize);

    // Face superior
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-halfSize, halfSize, -halfSize);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(halfSize, halfSize, -halfSize);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(halfSize, halfSize, halfSize);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-halfSize, halfSize, halfSize);

    // Face inferior
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-halfSize, -halfSize, -halfSize);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(halfSize, -halfSize, -halfSize);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(halfSize, -halfSize, halfSize);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-halfSize, -halfSize, halfSize);

    glEnd();
}

SphereWithSpikes sphereWithSpikes;

float sphereWithSpikes_param = 0.0;

void drawSphereWithSpikes(float i, float j) {
    glTranslatef(i, 0, j);
    glScalef(.7, .7, .7);

	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    glTranslatef(0, .5, 0);
    glScalef(1, .75, 1);
    customSolidSphere(0.5, 40, 40);

	glDisable(GL_TEXTURE_2D);

	// Desenhar os spikes
    int numCubes = 16; // Número de cubos a serem desenhados

	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

	for (int k = 0; k < numCubes; ++k) {
        float angle = 360.0f * k / numCubes;

        glPushMatrix();
        glRotatef(angle + sphereWithSpikes_param, 0, 1, 0);
        glTranslatef(0.8, 0.0, 0);
        glScalef(8, 1, 1);
        drawSolidCube(0.1);
        glPopMatrix();
    }

	glDisable(GL_TEXTURE_2D);

    glPopMatrix();

	glDisable(GL_TEXTURE_2D);
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
	
	// gluLookAt(7, 15, 10,
	// 		  7, 0, 5,
	// 		  0, 1, 0);

    glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);

	// glPushMatrix();
	// 	glColor3f(.95,.95,.1);
	// 	if(sphereWithSpikes.p){
	// 		glPushMatrix();
	// 			drawSphereWithSpikes(sphereWithSpikes.x/10,sphereWithSpikes.z/10);
	// 		glPopMatrix();
	// 	}
	// glPopMatrix();

    glPushMatrix();
        glRotatef(90, 0.0, 1.0, 0.0);
        // glTranslatef(sphereWithSpikes.x, 0, sphereWithSpikes.z);    
        glTranslatef(x, y, 0);
        glRotatef(theta, 0.0, 1.0, 0.0);
        glRotatef(alfa, 1.0, 0.0, 0.0);
        drawSphereWithSpikes(0, 0);
    glPopMatrix();

	glutSwapBuffers();
}

void init(){
    glClearColor(0.07, 0.14, 0.20, 0);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	loadTexture(); 

    // srand(time(0));
    // sphereWithSpikes.direction = rand()%4;
	// sphereWithSpikes.p = 1;  
}

int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
    // glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutMouseFunc(mouse);
	
    init();

	glutMainLoop();
	
	return 0;
}