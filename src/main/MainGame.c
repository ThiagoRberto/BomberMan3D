// =================================== IMAGE =================================== //
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

#define TIMER_INTERVAL 25
#define PI 3.14159265358979323846

#define FLOOR "./src/main/Texture/grass.bmp"
#define BOX "./src/main/Texture/crate.bmp"
#define WALL "./src/main/Texture/graybrick.bmp"

float sphereWithSpikes_count = 0;
float animation_ongoing = 0;

static GLuint names[3];

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

// =================================== ENEMY =================================== //
typedef struct{
	float x;
	float z;
	int direction;
	int alive;
}SphereWithSpikes;

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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    for (int i = 0; i < stacks; ++i) {
        double lat0 = PI * (-0.5 + (double)(i) / stacks);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = PI * (-0.5 + (double)(i + 1) / stacks);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            double lng = 2 * PI * (double)(j - 1) / slices;
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

SphereWithSpikes sphereWithSpikes1;
SphereWithSpikes sphereWithSpikes2;

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

    glPopMatrix();
}

// =================================== FIM =================================== //

// =================================== BOMBERMAN =================================== //

typedef struct{
	float x;
	float z;
	float old_direction;
	float new_direction;
}Bomberman_t;

int KEY_DOWN_W = 0;
int KEY_DOWN_S = 0;
int KEY_DOWN_A = 0;
int KEY_DOWN_D = 0;
int KEY_DOWN = 0;

int ID;

Bomberman_t bomberman;

int indicador = 1;
float move = 0.0;

GLUquadricObj* quadratic;

float red, green, blue;

void Eyes() {
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

void Hat() {
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

void Face() {
    glPushMatrix();
    glColor3f(1.0, 0.6, 0.4);
    glTranslatef(0.0, 0.6, 0.13);
    glScalef(0.47, 0.37, 0.08);
    glutSolidSphere(0.25, 20, 10);
    glPopMatrix();
}

void Head() {
    glPushMatrix();
    glColor3f(red, green, blue);
    glTranslatef(0.0, 0.6, 0.0);
    glScalef(0.4, 0.3, 0.3);
    glTranslatef(0.0, 0.0, 0.0);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();
    Hat();
    Eyes();
    Face();
}

void Buckle() {
    glPushMatrix();
    glColor3f(0.7, 0.7, 0.0);
    glTranslatef(0.0, 0.25, 0.125);

    glScalef(0.35, 0.2, 0.05);
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();
}

void Belt() {
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, 0.25, 0.0);
    glRotatef(90, 1, 0.0, 0.0);

    glScalef(1.4, 1.3, 0.6);
    glutSolidTorus(0.05, 0.05, 20, 20);
    glPopMatrix();
}

void Body() {
    glPushMatrix();
    glColor3f(0.4, 0.4, 0.7);
    glTranslatef(0.0, 0.25, 0.0);
    glScalef(0.3, 0.35, 0.25);
    glTranslatef(0.0, 0.25, 0.0);
    glutSolidSphere(0.5, 20, 10);
    glPopMatrix();
    Belt();
    Buckle();
}

void Hand() {
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.4);
    glTranslatef(0.0, -0.2, -0.01);
    glutSolidSphere(0.1, 20, 10);
    glPopMatrix();
}

void Shoulder() {
    glPushMatrix();
    glScalef(1.1, 1.0, 1.13);
    glutSolidSphere(0.05, 20, 10);
    glPopMatrix();
}

void Elbow() {
    glPushMatrix();
    glTranslatef(0.023, -0.18, -0.061);
    glutSolidSphere(0.027, 20, 10);
    glPopMatrix();
}


void RightArm() {
    glPushMatrix();
    glColor3f(red, green, blue);
    glTranslatef(0.14, 0.4, 0.0);

    Shoulder();

    glRotatef(20, 0.0, 0.0, 1.0);
    glRotatef(90, 1, 0.0, 0.0);

    glScalef(1.1, 1.0, 1.13);
    gluCylinder(quadratic, 0.05, 0.03, 0.16, 20, 10);
    glPopMatrix();

    // Antebraço
    glPushMatrix();
    glTranslatef(0.14, 0.4, 0.0);

    glRotatef(20, -0.55, -0.5, 0.5);
    Elbow();

    glTranslatef(0.02, -0.17, -0.05);

    Hand();
    glRotatef(90, 1, 0.0, 0.0);
    glColor3f(red, green, blue);
    glScalef(0.1, 0.1, 0.1);

    gluCylinder(quadratic, 0.3, 0.25, 1.2, 20, 10);
    glPopMatrix();
}

void LeftArm() {
    glPushMatrix();
    glScalef(-1.0, 1.0, 1.0);
    RightArm();
    glPopMatrix();
}

void Feet() {
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.4);
    glTranslatef(0.0, -0.17, 0.075);
    glScalef(1.0, 0.65, 1.4);
    glutSolidSphere(0.1, 20, 10);
    glPopMatrix();
}

void Thigh() {
    glPushMatrix();
    //glTranslatef(0.3, 0.35, 0.0);
    glScalef(1.17, 1.0, 1.1);
    glutSolidSphere(0.045, 20, 10);
    glPopMatrix();
}

void Knee() {
    glPushMatrix();
    glTranslatef(0.0, -0.15, 0.0);
    glScalef(1.5, 1.0, 1.5);
    glutSolidSphere(0.025, 20, 10);
    glPopMatrix();
}

void RightLeg() {
    glPushMatrix();
    glColor3f(red, green, blue);
    glTranslatef(0.07, 0.18, 0.0);

    glRotatef(7.5, 0, 0, 1);

    Thigh();
    Knee();

    glRotatef(90, 1, 0.0, 0.0);
    glScalef(0.1, 0.1, 0.1);
    gluCylinder(quadratic, 0.5, 0.35, 1.5, 20, 10);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.07, 0.18, 0.0);
    glRotatef(7.5, 0, 0, 1);

    glTranslatef(0.0, -0.15, 0.0);

    Feet();

    glColor3f(red, green, blue);
    glRotatef(90, 1, 0.0, 0.0);
    glScalef(0.1, 0.1, 0.1);
    gluCylinder(quadratic, 0.35, 0.3, 1.5, 20, 10);

    glPopMatrix();

}

void LeftLeg() {
    glPushMatrix();
    glScalef(-1.0, 1.0, 1.0);
    RightLeg();
    glPopMatrix();
}

void Bomberman(float r, float g, float b) {
    red = r;
    green = g;
    blue = b;

    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);
    glTranslatef(bomberman.x/10, 0, bomberman.z/10);
    glScalef(2, 2, 2);
    glRotatef(90 * bomberman.old_direction, 0, 1, 0);

    Head();
    Body();

    glPushMatrix();
        glTranslatef(.00125,0.0025,0);
        glTranslatef(0, 0.002, 0);
        glRotatef(move/2, 1, 0, 0);
        glTranslatef(.0,-0.002,0.0);
        RightArm();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-.00125,.0025,0);
        glTranslatef(0, 0.002, 0);
        glRotatef(-move/2, 1, 0, 0);
        glTranslatef(0,-0.002,0);
        LeftArm();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(.0031,0.0081,0);
        glTranslatef(0, 0.002, 0);
        glRotatef(0.75*(-move), 1, 0, 0);
        glTranslatef(0,-0.002,0);
        RightLeg();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-.0031,0.0081,0);
        glTranslatef(0, 0.002, 0);
        glRotatef(0.75*(move), 1, 0, 0);
        glTranslatef(0,-0.002,0);
        LeftLeg();
    glPopMatrix();

    glPopMatrix();
}

// =================================== FIM =================================== //

// =================================== MAP =================================== //

static int destroy_block[17][13];

void RandomWalls(void){
    srand(time(0));
    int i,j;

    //Alocando os blocos destrutíveis e indestrutíveis
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
	
    //Não permite que o Bomberman e os inimigos nasçam em blocos destrutíveis
    bomberman.x = 0;
    bomberman.z = 0;
    bomberman.old_direction = 0;
    bomberman.new_direction = 0;
	destroy_block[1][1] = 0;
	destroy_block[1][2] = 0;
	destroy_block[2][1] = 0;
	
    //Desterminando a posição dos inimigos
	int n = 0;

	while(n != 2){
		i = ((int) rand()) % 6 + 6;
		j = ((int) rand()) % 4 + 4;

		if(destroy_block[i][j] == 0){
			if(n == 0){
                sphereWithSpikes1.x = (i-1)*10;
				sphereWithSpikes1.z = (j-1)*10;
				destroy_block[i][j] = 0;
			}
			else if(n == 1){
                sphereWithSpikes2.x = (i-1)*10;
				sphereWithSpikes2.z = (j-1)*10;
				destroy_block[i][j] = 0;
			}
			n++;
		}
	}
}

void EnemyWalk(SphereWithSpikes* sphereWithSpikes){
	if(sphereWithSpikes->direction == 0) {
		if((int)sphereWithSpikes->z%10 == 0) {
			int num = rand()%3;
			if(num == 0 && (destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10] != 1 &&
							destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10] != 2)){
				sphereWithSpikes->z -= 1;
			}
			else if(num == 1 && (destroy_block[(int)sphereWithSpikes->x/10][(int)sphereWithSpikes->z/10 + 1] != 1 &&
								destroy_block[(int)sphereWithSpikes->x/10][(int)sphereWithSpikes->z/10 + 1] != 2)){
				sphereWithSpikes->direction = 2;
			}
			else if(num == 2 && (destroy_block[(int)sphereWithSpikes->x/10 + 2][(int)sphereWithSpikes->z/10 + 1] != 1 &&
								destroy_block[(int)sphereWithSpikes->x/10 + 2][(int)sphereWithSpikes->z/10 + 1] != 2)){
				sphereWithSpikes->direction = 3;
			}
			else{
				sphereWithSpikes->direction = 1;
			}
		}
		else {
			sphereWithSpikes->z -= 1;
		}
	}
	if(sphereWithSpikes->direction == 1) {
		if((int)sphereWithSpikes->z%10 == 0){
			int num = rand()%3;
			if(num == 0 && (destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10 + 2] != 1 && 
							destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10 + 2] != 2)){
				sphereWithSpikes->z += 1;
			}
			else if(num == 1 && (destroy_block[(int)sphereWithSpikes->x/10][(int)sphereWithSpikes->z/10 + 1] != 1 &&
								destroy_block[(int)sphereWithSpikes->x/10][(int)sphereWithSpikes->z/10 + 1] != 2)){
				sphereWithSpikes->direction = 2;
			}
			else if(num == 2 && (destroy_block[(int)sphereWithSpikes->x/10 + 2][(int)sphereWithSpikes->z/10 + 1] != 1 &&
								destroy_block[(int)sphereWithSpikes->x/10 + 2][(int)sphereWithSpikes->z/10 + 1] != 2)){
				sphereWithSpikes->direction = 3;
			}
			else{
				sphereWithSpikes->direction = 0;
			}
		}
		else {
			sphereWithSpikes->z += 1;
		}
	}
	if(sphereWithSpikes->direction == 2) {
		if((int)sphereWithSpikes->x%10 == 0) {
			int num = rand()%3;
			if(num == 0 && (destroy_block[(int)sphereWithSpikes->x/10][(int)sphereWithSpikes->z/10 + 1] != 1 &&
							destroy_block[(int)sphereWithSpikes->x/10][(int)sphereWithSpikes->z/10 + 1] != 2)){
				sphereWithSpikes->x -= 1;
			}
			else if(num == 1 && (destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10] != 1 &&
								destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10] != 2)){
				sphereWithSpikes->direction = 0;
			}
			else if(num == 2 && (destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10 + 2] != 1 &&
								destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10 + 2] != 2)){
				sphereWithSpikes->direction = 1;
			}
			else{
				sphereWithSpikes->direction = 3;
			}
		}
		else {
			sphereWithSpikes->x -= 1;
		}
	}
	if(sphereWithSpikes->direction == 3) {
		if((int)sphereWithSpikes->x%10 == 0) {
			int num = rand()%3;
			if(num == 0 && (destroy_block[(int)sphereWithSpikes->x/10 + 2][(int)sphereWithSpikes->z/10 + 1] != 1 &&
							destroy_block[(int)sphereWithSpikes->x/10 + 2][(int)sphereWithSpikes->z/10 + 1] != 2)){
				sphereWithSpikes->x += 1;
			}
			else if(num == 1 && (destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10] != 1 &&
								destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10] != 2)){
				sphereWithSpikes->direction = 0;
			}
			else if(num == 2 && (destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10 + 2] != 1 &&
								destroy_block[(int)sphereWithSpikes->x/10 + 1][(int)sphereWithSpikes->z/10 + 2] != 2)){
				sphereWithSpikes->direction = 1;
			}
			else{
				sphereWithSpikes->direction = 2;
			}
		}
		else {
			sphereWithSpikes->x += 1;
		}
	}
}

void Timer(int id){
	id = ID;
	
	if(id != 0){
		if(move == 45)
			indicador = -1;
		else if(move == -45)
			indicador = 1;
		if(indicador == 1)
			move+=5;
		else if(indicador == -1)
			move-=5;
			
		if(bomberman.old_direction != bomberman.new_direction){
			if(bomberman.old_direction < bomberman.new_direction){
				if(bomberman.new_direction - bomberman.old_direction <= 0.2){
				bomberman.old_direction = bomberman.new_direction;
			} else
				bomberman.old_direction += 0.3;
			}
			else {
				if(bomberman.old_direction - bomberman.new_direction <= 0.2){
					bomberman.old_direction = bomberman.new_direction;
				}
				else
					bomberman.old_direction -= 0.3;
			}
		}
	}

	if(id == 1){
		bomberman.z--;
        if((int)bomberman.z%10 == 0)
		{
			if(KEY_DOWN_W == 0 || destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10] == 1 || 
					destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10] == 2){
				ID = 0;
			}
		}
	}

	if(id == 2){
		bomberman.z++;
        if((int)bomberman.z%10 == 0){
			if(KEY_DOWN_S == 0 || destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10 + 2] == 1 ||
						destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10 + 2] == 2){
					ID = 0;
			}
		}
	}

	if(id == 3){
		bomberman.x--;
		if((int)bomberman.x%10 == 0){
			if(KEY_DOWN_A == 0 || destroy_block[(int)bomberman.x/10][(int)bomberman.z/10 + 1] == 1 ||
						destroy_block[(int)bomberman.x/10][(int)bomberman.z/10 + 1] == 2){
					ID = 0;
			}
		}
	}

	if(id == 4){
		bomberman.x++;
		if((int)bomberman.x%10 == 0){
			if(KEY_DOWN_D == 0 || destroy_block[(int)bomberman.x/10 + 2][(int)bomberman.z/10 + 1] == 1 || 
                destroy_block[(int)bomberman.x/10 + 2][(int)bomberman.z/10 + 1] == 2){
                ID = 0;
			}
		}
	}

	if(ID == 0){
		if(KEY_DOWN_W && destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10] != 1 && 
		    destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10] != 2){
			ID = 1;
			bomberman.new_direction = 2.0;
		}						
		else if(KEY_DOWN_S  && destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10 + 2] != 1 &&
			    destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10 + 2] != 2){
			ID = 2;
			bomberman.new_direction = 0.0;
		}						
		else if(KEY_DOWN_A && destroy_block[(int)bomberman.x/10][(int)bomberman.z/10 + 1] != 1 &&
			    destroy_block[(int)bomberman.x/10][(int)bomberman.z/10 + 1] != 2){
			ID = 3;
			bomberman.new_direction = -1.0;
		}				
		else if(KEY_DOWN_D && destroy_block[(int)bomberman.x/10 + 2][(int)bomberman.z/10 + 1] != 1 && 
			    destroy_block[(int)bomberman.x/10 + 2][(int)bomberman.z/10 + 1] != 2){
			ID = 4;
			bomberman.new_direction = 1.0;
		}
	}

    sphereWithSpikes_count++;
	sphereWithSpikes_param = PI*sin(sphereWithSpikes_count);
	
	if(sphereWithSpikes1.alive) {
		EnemyWalk(&sphereWithSpikes1);
		if(abs(sphereWithSpikes1.x - bomberman.x) <= 5 && abs(sphereWithSpikes1.z - bomberman.z) <= 5) {
			// game_over = 2;
			animation_ongoing = 0;
		}
	}

	if(sphereWithSpikes2.alive){
		EnemyWalk(&sphereWithSpikes2);
		if(abs(sphereWithSpikes2.x - bomberman.x) <= 5 && abs(sphereWithSpikes2.z - bomberman.z) <= 5) {
			// game_over = 2;
			animation_ongoing = 0;
		}
	}
	
	if(animation_ongoing)
	    glutTimerFunc(TIMER_INTERVAL, Timer, id); 

	glutPostRedisplay();
}

void DrawMap(){
	glPushMatrix();
		glTranslatef(-1, -0.5, -1);

		glPushMatrix();
			glColor3f(.1, .3, .1);
			glTranslatef(8, .2, 5.5);
			glScalef(17, .5, 12);
			glutSolidCube(1);
		glPopMatrix();

		int i;
		for(i = 0; i < 13; i++){
			int j;
			for(j = 0; j < 17; j++){
				if(i == 0 || i == 12 || j == 0 || j == 16 || (j % 2 == 0 && i % 2 == 0)){
					glPushMatrix();
						glScalef(1, .7, 1);
						glTranslatef(0, 1, 0);
						ApplyTexture(1.0, names[2]);
					glPopMatrix();
				}
				else {
					glColor3f(.1, .5, .1);
					glutSolidCube(.97);
					if(destroy_block[j][i] == 2)
					{
						glPushMatrix();
							glScalef(.9, .6, .9);
							glTranslatef(0, 1.5, 0);
							ApplyTexture(1.0, names[1]);
						glPopMatrix();
					}
				}
				glTranslatef(1, 0, 0);
			}
			glTranslatef(-17, 0, 1);
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
        case 13:
		//ENTER = Start ou Pause
			if(animation_ongoing == 0){
                animation_ongoing = 1;
                glutTimerFunc(TIMER_INTERVAL, Timer, 0);
			}
			else if(animation_ongoing == 1){
				animation_ongoing = 0;
			}
			break;
        case 'R':
        case 'r':
        //Restart
            animation_ongoing = 0;
            ID = 0;
            bomberman.x = 0;
            bomberman.z = 0;
            indicador = 1;
            move = 0.0;
            sphereWithSpikes2.alive = 1;
            sphereWithSpikes1.alive = 1;
            RandomWalls();
			glutPostRedisplay();
            break;
        case 'W':
        case 'w':
            KEY_DOWN_W = 1;
			KEY_DOWN++;
			if(animation_ongoing == 1 && (destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10] != 1 && 
			    destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10] != 2) && (int)bomberman.x % 10 == 0 && (int)bomberman.z % 10 ==0){
				bomberman.new_direction = 2.0;
				ID = 1;
			}
            break;
        case 'S':
        case 's':
            KEY_DOWN_S = 1;
			KEY_DOWN++;
			if(animation_ongoing == 1 && (destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10 + 2] != 1 &&
			    destroy_block[(int)bomberman.x/10 + 1][(int)bomberman.z/10 + 2] != 2) && (int)bomberman.x % 10 == 0 && (int)bomberman.z % 10 ==0){
				bomberman.new_direction = 0.0;
				ID = 2;
			}
            break;
        case 'A':
        case 'a':
            KEY_DOWN_A = 1;
			KEY_DOWN++;
			if(animation_ongoing == 1 && (destroy_block[(int)bomberman.x/10][(int)bomberman.z/10 + 1] != 1 &&
			    destroy_block[(int)bomberman.x/10][(int)bomberman.z/10 + 1] != 2) && (int)bomberman.x % 10 == 0 && (int)bomberman.z % 10 ==0){
				bomberman.new_direction = -1.0;
				ID = 3;
			}
            break;
        case 'D':
        case 'd':
            KEY_DOWN_D = 1;
			KEY_DOWN++;
			if(animation_ongoing == 1 && (destroy_block[(int)bomberman.x/10 + 2][(int)bomberman.z/10 + 1] != 1 &&
			    destroy_block[(int)bomberman.x/10 + 2][(int)bomberman.z/10 + 1] != 2) && (int)bomberman.x % 10 == 0 && (int)bomberman.z % 10 ==0){
				bomberman.new_direction = 1.0;
				ID = 4;
			}

            break;
        case 32:
            //SPACE = Arma Bomba

            break;
    }
}

void keyboardUp(unsigned char key, int x, int y){
    switch(key){
        case 'W':
        case 'w':
            KEY_DOWN_W = 0;
            KEY_DOWN--;
            break;
        case 'S':
        case 's':
            KEY_DOWN_S = 0;
            KEY_DOWN--;
            break;
        case 'A':
        case 'a':
            KEY_DOWN_A = 0;
            KEY_DOWN--;
            break;
        case 'D':
        case 'd':
            KEY_DOWN_D = 0;
            KEY_DOWN--;
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
	
	gluLookAt(7, 15, 10, 7, 0, 5, 0, 1, 0);

    //Base do Mapa
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
		DrawMap();
	glPopMatrix();

    glPushMatrix();
        Bomberman(0.1, 0.1, 0.1);
    glPopMatrix();

    glPushMatrix();
		if(sphereWithSpikes1.alive){
			glPushMatrix();
				drawSphereWithSpikes(sphereWithSpikes1.x/10, sphereWithSpikes1.z/10);
			glPopMatrix();
		}
		if(sphereWithSpikes2.alive){
			glPushMatrix();
				drawSphereWithSpikes(sphereWithSpikes2.x/10, sphereWithSpikes2.z/10);
			glPopMatrix();
		}
	glPopMatrix();
		
	glutSwapBuffers();
}

void init(){
    // glClearColor(0.07, 0.14, 0.20, 0);
    glClearColor(0.0, 0., 0.0, 0);
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

    quadratic = gluNewQuadric();

    srand(time(0));

    //Adicionando os inimigos
    sphereWithSpikes1.direction = rand() % 4;
    sphereWithSpikes2.direction = rand() % 4;

    //Inicializando os inimigos
	sphereWithSpikes1.alive = 1;
	sphereWithSpikes2.alive = 1;

    //Bomb
    
    // glShadeModel(GL_SMOOTH);
    RandomWalls();
    InitializeTexture();	
    loadTexture(); 

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
    glutKeyboardUpFunc(keyboardUp);
	// glutKeyboardUpFunc(on_keyboardUp);
	
    init();

	glutMainLoop();
	
	return 0;
}

// =================================== FIM =================================== //