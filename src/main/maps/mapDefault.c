
// =================================== IMAGE =================================== //

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  unsigned short type;
  unsigned int size;
  unsigned short reserved1;
  unsigned short reserved2;
  unsigned int offsetbits;
} BITMAPFILEHEADER;

typedef struct {
  unsigned int size;
  unsigned int width;
  unsigned int height;
  unsigned short planes;
  unsigned short bitcount;
  unsigned int compression;
  unsigned int sizeimage;
  int xpelspermeter;
  int ypelspermeter;
  unsigned int colorsused;
  unsigned int colorsimportant;
} BITMAPINFOHEADER;

typedef struct Image {
  int width, height; 
  char *pixels; 
} Image;

Image *image_init(int width, int height) {

  Image *image;

  assert(width >= 0 && height >= 0);

  image = (Image *) malloc(sizeof(Image));
  assert(image != NULL);

  image->width = width;
  image->height = height;
  if (width == 0 || height == 0)
    image->pixels = NULL;
  else {
    image->pixels = (char *)malloc(3 * width * height * sizeof(char));
    assert(image->pixels != NULL);
  }

  return image;
}

void image_done(Image *image) {
  free(image->pixels);
  free(image);
}

void image_read(Image *image, char *filename) {
  FILE *file;
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  unsigned int i;
  unsigned char r, g, b, a;

  free(image->pixels);
  image->pixels = NULL;

  assert((file = fopen(filename, "rb")) != NULL);

  fread(&bfh.type, 2, 1, file);
  fread(&bfh.size, 4, 1, file);
  fread(&bfh.reserved1, 2, 1, file);
  fread(&bfh.reserved2, 2, 1, file);
  fread(&bfh.offsetbits, 4, 1, file);

  fread(&bih.size, 4, 1, file);
  fread(&bih.width, 4, 1, file);
  fread(&bih.height, 4, 1, file);
  fread(&bih.planes, 2, 1, file);
  fread(&bih.bitcount, 2, 1, file);
  fread(&bih.compression, 4, 1, file);
  fread(&bih.sizeimage, 4, 1, file);
  fread(&bih.xpelspermeter, 4, 1, file);
  fread(&bih.ypelspermeter, 4, 1, file);
  fread(&bih.colorsused, 4, 1, file);
  fread(&bih.colorsimportant, 4, 1, file);

  image->width = bih.width;
  image->height = bih.height;

  if (bih.bitcount == 24)
    image->pixels = (char *)malloc(3 * bih.width * bih.height * sizeof(char));
  else if (bih.bitcount == 32)
    image->pixels = (char *)malloc(4 * bih.width * bih.height * sizeof(char));
  else {
    fprintf(stderr, "image_read(): Error Bits.\n");
    exit(1);
  }
  assert(image->pixels != NULL);

  if (bih.bitcount == 24)
    for (i = 0; i < bih.width * bih.height; i++) {
      fread(&b, sizeof(char), 1, file);
      fread(&g, sizeof(char), 1, file);
      fread(&r, sizeof(char), 1, file);

      image->pixels[3 * i] = r;
      image->pixels[3 * i + 1] = g;
      image->pixels[3 * i + 2] = b;
    }
  else if (bih.bitcount == 32)
    for (i = 0; i < bih.width * bih.height; i++) {
      fread(&b, sizeof(char), 1, file);
      fread(&g, sizeof(char), 1, file);
      fread(&r, sizeof(char), 1, file);
      fread(&a, sizeof(char), 1, file);

      image->pixels[4 * i] = r;
      image->pixels[4 * i + 1] = g;
      image->pixels[4 * i + 2] = b;
      image->pixels[4 * i + 3] = a;
    }
  fclose(file);
}

// =================================== END =================================== //

#include <GL/glut.h>
#include <time.h>

#define FLOOR "./src/main/Texture/grass.bmp"
#define BOX "./src/main/Texture/crate.bmp"
#define WALL "./src/main/Texture/graybrick.bmp"

static GLuint names[3];

// =================================== TEXTURE =================================== //

void InitializeTexture(){
    Image * image;

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    image = image_init(0, 0);

    image_read(image, FLOOR);

    glGenTextures(3, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
                 
    image_read(image, BOX);

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
                 
    image_read(image, WALL);

    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    image_done(image);
}

static void ApplyTexture(double edge,int texture){
    int coef_of_mapping = 1;
    double offset = 0.01; 
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix(); 
        glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(-edge/2, -edge/2, edge/2);
            glTexCoord2f(0, edge);
            glVertex3f(-edge/2, edge/2, edge/2);
            glTexCoord2f(edge*coef_of_mapping , edge);
            glVertex3f(edge/2, edge/2, edge/2);
            glTexCoord2f(edge*coef_of_mapping, 0);
            glVertex3f(edge/2, -edge/2, edge/2);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3f(-edge/2, -edge/2, -edge/2);
            glTexCoord2f(0, edge);
            glVertex3f(-edge/2, edge/2, -edge/2);
            glTexCoord2f(edge*coef_of_mapping, edge);
            glVertex3f(edge/2, edge/2, -edge/2);
            glTexCoord2f(edge*coef_of_mapping, 0);
            glVertex3f(edge/2, -edge/2, -edge/2);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(-edge/2, edge/2,edge/2);
            glTexCoord2f(0, edge);
            glVertex3f(-edge/2, edge/2,-edge/2);
            glTexCoord2f(edge*coef_of_mapping, edge);
            glVertex3f(edge/2, edge/2,-edge/2);
            glTexCoord2f(edge*coef_of_mapping, 0);
            glVertex3f(edge/2, edge/2,edge/2);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(-edge/2-offset, -edge/2,edge/2);
            glTexCoord2f(0, edge);
            glVertex3f(-edge/2-offset, edge/2,edge/2);
            glTexCoord2f(edge*coef_of_mapping, edge);
            glVertex3f(-edge/2-offset, edge/2,-edge/2);
            glTexCoord2f(edge*coef_of_mapping, 0);
            glVertex3f(-edge/2-offset, -edge/2,-edge/2);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0 , 0);
            glVertex3f(edge/2+offset, -edge/2,edge/2);
            glTexCoord2f(0, edge);
            glVertex3f(edge/2+offset, edge/2,edge/2);
            glTexCoord2f(edge*coef_of_mapping , edge);
            glVertex3f(edge/2+offset, edge/2,-edge/2);
            glTexCoord2f(edge*coef_of_mapping, 0);
            glVertex3f(edge/2+offset, -edge/2,-edge/2);
        glEnd();
    glPopMatrix();   
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

// =================================== FIM =================================== //

// =================================== MAP =================================== //

static int destroy_block[17][13];

void RandomWalls(void){
    srand(time(0));
    int i,j;

    for(i = 0; i < 17; i++){
		for(j = 0; j < 13; j++){
			if(i == 0 || j == 0 || j == 12 || i == 16 || (j % 2 == 0 && i % 2 == 0))
				destroy_block[i][j] = 1;
			else if(((int) rand()) % 3 == 0)
				destroy_block[i][j] = 2;
			else 
				destroy_block[i][j] = 0;
		}
	}
	
	destroy_block[1][1] = 0;
	destroy_block[1][2] = 0;
	destroy_block[2][1] = 0;
	
	int n = 0;

	while(n != 2){
		i = ((int) rand()) % 6 + 6;
		j = ((int) rand()) % 4 + 4;

		if(destroy_block[i][j] == 0){
			if(n == 0){
				destroy_block[i][j] = 0;
			}
			else if(n == 1){
				destroy_block[i][j] = 0;
			}
			n++;
		}
	}
}

void DrawMap(){
	glPushMatrix();
		glTranslatef(-1, -0.5, -1);

		glPushMatrix();
			glColor3f(.1, .3, .1);
			glTranslatef(8,.2,5.5);
			glScalef(17,.5,12);
			glutSolidCube(1);
		glPopMatrix();

		int i;
		for(i=0; i<13; i++){
			int j;
			for(j=0; j<17; j++){
				if(i == 0 || i == 12 || j==0 || j==16 || (j%2==0 && i%2==0)){
					glPushMatrix();
						glScalef(1,.7,1);
						glTranslatef(0,1,0);
						ApplyTexture(1.0,names[2]);
					glPopMatrix();
				}
				else {
					glColor3f(.1, .5, .1);
					glutSolidCube(.97);
					if(destroy_block[j][i] == 2)
					{
						glPushMatrix();
							glScalef(.9,.6,.9);
							glTranslatef(0,1.5,0);
							ApplyTexture(1.0,names[1]);
						glPopMatrix();
					}
				}
				glTranslatef(1,0,0);
			}
			glTranslatef(-17,0,1);
		}
		
	glPopMatrix();
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27:
        //ESC = Exit
            glDeleteTextures(3, names);
            exit(0);
            break;
    }
}

void reshape(int width, int height){
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 1, 100);
    glutFullScreen();
}

void display(){	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(7, 15, 10,
			  7, 0, 5,
			  0, 1, 0);

    // Floor
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-1, 0, -1);

        glTexCoord2f(6, 0);
        glVertex3f(15, 0, -1);

        glTexCoord2f(6, 6);
        glVertex3f(15, 0, 11);

        glTexCoord2f(0, 6);
        glVertex3f(-1, 0, 11);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

	glPushMatrix();
		glColor3f(0, 0.3, 0.4);
		DrawMap();
	glPopMatrix();
		
	glutSwapBuffers();
}

void init(){
    glClearColor(0.07, 0.14, 0.20, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    float light_position[] = {-1, 3, 2, 1};
    float light_ambient[] = {.3f, .3f, .3f, 1};
    float light_diffuse[] = {.7f, .7f, .7f, 1};
    float light_specular[] = {.7f, .7f, .7f, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    
    glEnable(GL_COLOR_MATERIAL);
    
    // glShadeModel(GL_SMOOTH);
    RandomWalls();
    InitializeTexture();	

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
}

int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
  glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	
  init();

	glutMainLoop();
	
	return 0;
}
