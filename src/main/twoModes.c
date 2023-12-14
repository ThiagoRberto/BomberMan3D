// =================================== IMAGE ===================================
// //
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TIMER_INTERVAL 25
#define PI 3.14159265358979323846

#define FLOOR "./src/main/Texture/grass.bmp"
#define BOX "./src/main/Texture/crate.bmp"
#define WALL "./src/main/Texture/graybrick.bmp"
#define EXPLOSION "./src/main/Texture/explosion.bmp"

int game_over = 0;

float sphereWithSpikes_count = 0;
float animation_ongoing = 0;

static GLuint names[4];
static int destroy_block[17][13];

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

  image = (Image *)malloc(sizeof(Image));
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

// =================================== END ===================================
// //

// =================================== TEXTURE
// =================================== //

void InitializeTexture() {
  Image *image;

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

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image->pixels);

  image_read(image, BOX);

  glBindTexture(GL_TEXTURE_2D, names[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image->pixels);

  image_read(image, WALL);

  glBindTexture(GL_TEXTURE_2D, names[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image->pixels);

  image_read(image, EXPLOSION);

  glBindTexture(GL_TEXTURE_2D, names[3]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image->pixels);

  glBindTexture(GL_TEXTURE_2D, 0);

  image_done(image);
}

static void ApplyTexture(double edge, int texture) {
  int coef_of_mapping = 1;
  double offset = 0.01;

  glBindTexture(GL_TEXTURE_2D, texture);
  glPushMatrix();
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(-edge / 2, -edge / 2, edge / 2);
  glTexCoord2f(0, edge);
  glVertex3f(-edge / 2, edge / 2, edge / 2);
  glTexCoord2f(edge * coef_of_mapping, edge);
  glVertex3f(edge / 2, edge / 2, edge / 2);
  glTexCoord2f(edge * coef_of_mapping, 0);
  glVertex3f(edge / 2, -edge / 2, edge / 2);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(-edge / 2, -edge / 2, -edge / 2);
  glTexCoord2f(0, edge);
  glVertex3f(-edge / 2, edge / 2, -edge / 2);
  glTexCoord2f(edge * coef_of_mapping, edge);
  glVertex3f(edge / 2, edge / 2, -edge / 2);
  glTexCoord2f(edge * coef_of_mapping, 0);
  glVertex3f(edge / 2, -edge / 2, -edge / 2);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(-edge / 2, edge / 2, edge / 2);
  glTexCoord2f(0, edge);
  glVertex3f(-edge / 2, edge / 2, -edge / 2);
  glTexCoord2f(edge * coef_of_mapping, edge);
  glVertex3f(edge / 2, edge / 2, -edge / 2);
  glTexCoord2f(edge * coef_of_mapping, 0);
  glVertex3f(edge / 2, edge / 2, edge / 2);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(-edge / 2 - offset, -edge / 2, edge / 2);
  glTexCoord2f(0, edge);
  glVertex3f(-edge / 2 - offset, edge / 2, edge / 2);
  glTexCoord2f(edge * coef_of_mapping, edge);
  glVertex3f(-edge / 2 - offset, edge / 2, -edge / 2);
  glTexCoord2f(edge * coef_of_mapping, 0);
  glVertex3f(-edge / 2 - offset, -edge / 2, -edge / 2);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(edge / 2 + offset, -edge / 2, edge / 2);
  glTexCoord2f(0, edge);
  glVertex3f(edge / 2 + offset, edge / 2, edge / 2);
  glTexCoord2f(edge * coef_of_mapping, edge);
  glVertex3f(edge / 2 + offset, edge / 2, -edge / 2);
  glTexCoord2f(edge * coef_of_mapping, 0);
  glVertex3f(edge / 2 + offset, -edge / 2, -edge / 2);
  glEnd();
  glPopMatrix();

  glBindTexture(GL_TEXTURE_2D, 0);
}

// =================================== FIM ===================================
// //

// =================================== ENEMY ===================================
// //
typedef struct {
  float x;
  float z;
  int direction;
  int alive;
} SphereWithSpikes;

GLuint textureID;

void loadTexture() {
  textureID =
      SOIL_load_OGL_texture("./src/main/Texture/spike.jpg", SOIL_LOAD_AUTO,
                            SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

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

// =================================== FIM ===================================
// //

// =================================== BOMBERMAN
// =================================== //

typedef struct {
  float x;
  float z;
  float old_direction;
  float new_direction;
} Bomberman_t;

int KEY_DOWN_W = 0;
int KEY_DOWN_S = 0;
int KEY_DOWN_A = 0;
int KEY_DOWN_D = 0;
int KEY_DOWN = 0;

int ID;

Bomberman_t bomberman;

int indicador = 1;
float move = 0.0;

GLUquadricObj *quadratic;
float theta = 0.0, thetaDead = 0.0;
int stop = 1, inclination = 0, died = 0;
int runAuxiliar = 0, heightAuxiliar = 0;
float height = 0.0;
float red, green, blue;

void running(int value) {
  if (stop && inclination == 0) {
    theta = 0;
    height = 0;
    runAuxiliar = 0;
    heightAuxiliar = 0;
    inclination = 0;
    glutPostRedisplay();
    return;
  } else if (stop && inclination >= 0)
    inclination--;
  else {
    if (theta >= 20) {
      runAuxiliar = 1;
    } else if (theta <= -20) {
      runAuxiliar = 0;
    }
    if (runAuxiliar) {
      value = value * -1;
    }

    theta += value * 0.3;

    if (!heightAuxiliar) {
      height = height + 0.001;
      if (height >= 0.05)
        heightAuxiliar = 1;
    } else {
      height = height - 0.001;
      if (height <= 0)
        heightAuxiliar = 0;
    }
    if (inclination <= 15)
      inclination++;
  }

  glutPostRedisplay();
  glutTimerFunc(10, running, 1);
}

int count = 0;
int diedSpike;

void dying(int value) {
  count++;
  thetaDead += 6;

  if (count >= 100 && diedSpike) {
    game_over = 1;
    animation_ongoing = 0;
  } else if (count >= 1000 && !diedSpike) {
    game_over = 1;
    animation_ongoing = 0;
  }

  if (thetaDead >= 120)
    thetaDead = 120;

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
  if (!died)
    eyes();
  else
    deadEyes();
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

  if (died)
    glRotatef(thetaDead, 0.0, 0.0, 1.0);
  else
    glRotatef(theta * 5, 1, 0.0, 0.0);
  shoulder();

  glRotatef(20, 0.0, 0.0, 1.0);
  glRotatef(90, 1, 0.0, 0.0);

  glScalef(1.1, 1.0, 1.13);
  gluCylinder(quadratic, 0.05, 0.03, 0.16, 20, 10);
  glPopMatrix();

  // Antebraço
  glPushMatrix();
  glTranslatef(0.14, 0.4, 0.0);

  if (died)
    glRotatef(thetaDead, 0.0, 0.0, 1.0);
  else
    glRotatef(theta * 5, 1.0, 0.0, 0.0);
  glRotatef(20, -0.55, -0.5, 0.5);
  elbow();

  glTranslatef(0.02, -0.17, -0.05);
  int theta2 = theta * 5;
  if (theta2 >= 0)
    theta2 = 0;
  if (!died)
    glRotatef(theta2, 1.0, 0.0, 0.0);
  else
    glRotatef(thetaDead * 0.2, 0.0, 0.0, 1.0);

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
  if (!died)
    theta = -theta;
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
  // glTranslatef(0.3, 0.35, 0.0);
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
  if (thetaLegsDead >= 35)
    thetaLegsDead = 35;
  if (thetaLegs <= -15)
    thetaLegs = -15;
  glPushMatrix();
  glColor3f(red, green, blue);
  glTranslatef(0.07, 0.18, 0.0);

  if (died)
    glRotatef(thetaLegsDead, 0.0, 0.0, 1.0);
  else
    glRotatef(-thetaLegs * 1.5, 1.0, 0.0, 0.0);
  glRotatef(7.5, 0, 0, 1);

  thigh();
  knee();

  glRotatef(90, 1, 0.0, 0.0);
  glScalef(0.1, 0.1, 0.1);
  gluCylinder(quadratic, 0.5, 0.35, 1.5, 20, 10);
  glPopMatrix();

  glPushMatrix();

  glTranslatef(0.07, 0.18, 0.0);
  if (died)
    glRotatef(thetaLegsDead, 0.0, 0.0, 1.0);
  else
    glRotatef(-thetaLegs * 1.5, 1.0, 0.0, 0.0);
  glRotatef(7.5, 0, 0, 1);

  glTranslatef(0.0, -0.15, 0.0);

  int thetaLegs2 = thetaLegs;
  if (thetaLegs2 >= 0)
    thetaLegs = 0;
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

void Bomberman(float r, float g, float b) {
  red = r;
  green = g;
  blue = b;

  glPushMatrix();
  glTranslatef(0.0, 0.2 + height, 0.0);
  glTranslatef(bomberman.x / 10, 0, bomberman.z / 10);
  glScalef(2, 2, 2);
  glRotatef(90 * bomberman.old_direction, 0, 1, 0);
  if (!stop)
    glRotatef(-inclination, 1.0, 0.0, 0.0);

  head();
  body();

  glPushMatrix();
  glTranslatef(.00125, 0.0025, 0);
  glTranslatef(0, 0.002, 0);
  glRotatef(move / 2, 1, 0, 0);
  glTranslatef(.0, -0.002, 0.0);
  rightArm();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-.00125, .0025, 0);
  glTranslatef(0, 0.002, 0);
  glRotatef(-move / 2, 1, 0, 0);
  glTranslatef(0, -0.002, 0);
  leftArm();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(.0031, 0.0081, 0);
  glTranslatef(0, 0.002, 0);
  glRotatef(0.75 * (-move), 1, 0, 0);
  glTranslatef(0, -0.002, 0);
  rightLeg();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-.0031, 0.0081, 0);
  glTranslatef(0, 0.002, 0);
  glRotatef(0.75 * (move), 1, 0, 0);
  glTranslatef(0, -0.002, 0);
  leftLeg();
  glPopMatrix();

  glPopMatrix();
}

// =================================== FIM ===================================

// =================================== BOMBA
// ===================================

typedef struct {
  int x;
  int z;
  int ind;
  float bomb_pump;
} Bomba_t;

Bomba_t bomba;

float redBomb = 0.1;

void draw_explosion(int x, int y) {
  GLUquadric *quadric = gluNewQuadric();

  glPushMatrix();
  glTranslatef(x, 0.5, y);
  glColor3f(1, 0.5, 0);
  float scaleFactor = (bomba.bomb_pump - 100) / 30.0f;
  glScalef(scaleFactor, scaleFactor, scaleFactor);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, names[3]);
  gluQuadricTexture(quadric, GL_TRUE);

  gluSphere(quadric, 1.0, 20, 20);

  glBindTexture(GL_TEXTURE_2D, 0);
  glPopMatrix();
}

void explosion(int x, int y) {
  draw_explosion(x, y);
  if (fabsf(x * 10 - bomberman.x) <= 5 && fabsf(y * 10 - bomberman.z) <= 5) {
    died = 1;
    diedSpike = 0;
    glutTimerFunc(10, dying, 1);
  }
  destroy_block[x + 1][y + 1] = 0;
  if (destroy_block[x + 2][y + 1] != 1) {
    draw_explosion(x + 1, y);
    if (fabsf((x + 1) * 10 - bomberman.x) <= 5 &&
        fabsf(y * 10 - bomberman.z) <= 5) {
      died = 1;
      diedSpike = 0;
      glutTimerFunc(10, dying, 1);
    }
    if (fabsf((x + 1) * 10 - sphereWithSpikes1.x) <= 5 &&
        fabsf(y * 10 - sphereWithSpikes1.z) <= 5) {
      sphereWithSpikes1.alive = 0;
    }
    if (fabsf((x + 1) * 10 - sphereWithSpikes2.x) <= 5 &&
        fabsf(y * 10 - sphereWithSpikes2.z) <= 5) {
      sphereWithSpikes2.alive = 0;
    }
    destroy_block[x + 2][y + 1] = 0;
  }
  if (destroy_block[x][y + 1] != 1) {
    draw_explosion(x - 1, y);
    if (fabsf((x - 1) * 10 - bomberman.x) <= 5 &&
        fabsf(y * 10 - bomberman.z) <= 5) {
      died = 1;
      diedSpike = 0;
      glutTimerFunc(10, dying, 1);
    }
    if (fabsf((x - 1) * 10 - sphereWithSpikes1.x) <= 5 &&
        fabsf(y * 10 - sphereWithSpikes1.z) <= 5) {
      sphereWithSpikes1.alive = 0;
    }
    if (fabsf((x - 1) * 10 - sphereWithSpikes2.x) <= 5 &&
        fabsf(y * 10 - sphereWithSpikes2.z) <= 5) {
      sphereWithSpikes2.alive = 0;
    }
    destroy_block[x][y + 1] = 0;
  }
  if (destroy_block[x + 1][y + 2] != 1) {
    draw_explosion(x, y + 1);
    if (fabsf(x * 10 - bomberman.x) <= 5 &&
        fabsf((y + 1) * 10 - bomberman.z) <= 5) {
      died = 1;
      diedSpike = 0;
      glutTimerFunc(10, dying, 1);
    }
    if (fabsf(x * 10 - sphereWithSpikes1.x) <= 5 &&
        fabsf((y + 1) * 10 - sphereWithSpikes1.z) <= 5) {
      sphereWithSpikes1.alive = 0;
    }
    if (fabsf(x * 10 - sphereWithSpikes2.x) <= 5 &&
        fabsf((y + 1) * 10 - sphereWithSpikes2.z) <= 5) {
      sphereWithSpikes2.alive = 0;
    }
    destroy_block[x + 1][y + 2] = 0;
  }
  if (destroy_block[x + 1][y] != 1) {
    draw_explosion(x, y - 1);
    if (fabsf(x * 10 - bomberman.x) <= 5 &&
        fabsf((y - 1) * 10 - bomberman.z) <= 5) {
      died = 1;
      diedSpike = 0;
      glutTimerFunc(10, dying, 1);
    }
    if (fabsf(x * 10 - sphereWithSpikes1.x) <= 5 &&
        fabsf((y - 1) * 10 - sphereWithSpikes1.z) <= 5) {
      sphereWithSpikes1.alive = 0;
    }
    if (fabsf(x * 10 - sphereWithSpikes2.x) <= 5 &&
        fabsf((y - 1) * 10 - sphereWithSpikes2.z) <= 5) {
      sphereWithSpikes2.alive = 0;
    }
    destroy_block[x + 1][y] = 0;
  }
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

void bodyBomb() {
  glPushMatrix();
  glColor3f(redBomb, 0.1, 0.1);
  glutSolidSphere(0.5, 30, 30);
  glPopMatrix();
  bodyDetail();
}

int exploding = 0;

void Exploding() {
  if (exploding) {
    redBomb = 0.4;
    exploding = 0;
  } else {
    redBomb = 0.1;
    exploding = 1;
  }
  glutTimerFunc(50, Exploding, 1);
  glutPostRedisplay();
}

void draw_bomb() {
  glPushMatrix();
  if (bomba.bomb_pump > 100) {
    explosion(bomba.x, bomba.z);
    if (bomba.bomb_pump >= 130) {
      bomba.bomb_pump = 0;
      bomba.ind = 0;
      bomba.x = -1;
      bomba.z = -1;
    }
  } else {
    glTranslatef(bomba.x, 0.5, bomba.z);
    glColor3f(red, 0, 0);
    glScalef(1.0 + cos((bomba.bomb_pump) / 5) * 0.05,
             1.0 + cos((bomba.bomb_pump) / 5) * 0.05,
             1.0 + cos((bomba.bomb_pump) / 5) * 0.05);
    bodyBomb();
    glutTimerFunc(50, Exploding, 1);
  }
  glPopMatrix();
}

// =================================== FIM ===================================
// //

// =================================== GAME OVER
// =================================== //

static void draw_game_over(int pob) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
  int x = -350;
  int y = 0;
  int z = 0;

  char *over_game = malloc(30);
  if (pob == 1) {
    sprintf(over_game, "Game over!");
  } else {
    sprintf(over_game, "Victory!");
  }
  glPushMatrix();
  glScalef(0.06, 0.06, 5);
  glPushAttrib(GL_LINE_BIT);
  glLineWidth(4);
  glDisable(GL_TEXTURE_2D);
  glColor3f(0, 0, 1);
  glScalef(0.01, 0.01, 5);
  glTranslatef(x, y, z);
  int len = strlen(over_game);
  for (int i = 0; i < len; i++) {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, over_game[i]);
  }
  free(over_game);
  glEnable(GL_LIGHTING);
  glPopAttrib();
  glPopMatrix();
}

// =================================== FIM ===================================
// //

// =================================== MAP ===================================
// //

void RandomWalls(void) {
  srand(time(0));
  int i, j;

  // Alocando os blocos destrutíveis e indestrutíveis
  for (i = 0; i < 17; i++) {
    for (j = 0; j < 13; j++) {
      if (i == 0 || j == 0 || j == 12 || i == 16 || (j % 2 == 0 && i % 2 == 0))
        destroy_block[i][j] = 1;
      else if (((int)rand()) % 3 == 0)
        destroy_block[i][j] = 2;
      else
        destroy_block[i][j] = 0;
    }
  }

  // Não permite que o Bomberman e os inimigos nasçam em blocos destrutíveis
  bomberman.x = 0;
  bomberman.z = 0;
  bomberman.old_direction = 0;
  bomberman.new_direction = 0;
  destroy_block[1][1] = 0;
  destroy_block[1][2] = 0;
  destroy_block[2][1] = 0;

  // Desterminando a posição dos inimigos
  int n = 0;

  while (n != 2) {
    i = ((int)rand()) % 6 + 6;
    j = ((int)rand()) % 4 + 4;

    if (destroy_block[i][j] == 0) {
      if (n == 0) {
        sphereWithSpikes1.x = (i - 1) * 10;
        sphereWithSpikes1.z = (j - 1) * 10;
        destroy_block[i][j] = 0;
      } else if (n == 1) {
        sphereWithSpikes2.x = (i - 1) * 10;
        sphereWithSpikes2.z = (j - 1) * 10;
        destroy_block[i][j] = 0;
      }
      n++;
    }
  }
}

void EnemyWalk(SphereWithSpikes *sphereWithSpikes) {
  if (sphereWithSpikes->direction == 0) {
    if ((int)sphereWithSpikes->z % 10 == 0) {
      int num = rand() % 3;
      if (num == 0 && (destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                                    [(int)sphereWithSpikes->z / 10] != 1 &&
                       destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                                    [(int)sphereWithSpikes->z / 10] != 2)) {
        sphereWithSpikes->z -= 1;
      } else if (num == 1 &&
                 (destroy_block[(int)sphereWithSpikes->x / 10]
                               [(int)sphereWithSpikes->z / 10 + 1] != 1 &&
                  destroy_block[(int)sphereWithSpikes->x / 10]
                               [(int)sphereWithSpikes->z / 10 + 1] != 2)) {
        sphereWithSpikes->direction = 2;
      } else if (num == 2 &&
                 (destroy_block[(int)sphereWithSpikes->x / 10 + 2]
                               [(int)sphereWithSpikes->z / 10 + 1] != 1 &&
                  destroy_block[(int)sphereWithSpikes->x / 10 + 2]
                               [(int)sphereWithSpikes->z / 10 + 1] != 2)) {
        sphereWithSpikes->direction = 3;
      } else {
        sphereWithSpikes->direction = 1;
      }
    } else {
      sphereWithSpikes->z -= 1;
    }
  }
  if (sphereWithSpikes->direction == 1) {
    if ((int)sphereWithSpikes->z % 10 == 0) {
      int num = rand() % 3;
      if (num == 0 && (destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                                    [(int)sphereWithSpikes->z / 10 + 2] != 1 &&
                       destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                                    [(int)sphereWithSpikes->z / 10 + 2] != 2)) {
        sphereWithSpikes->z += 1;
      } else if (num == 1 &&
                 (destroy_block[(int)sphereWithSpikes->x / 10]
                               [(int)sphereWithSpikes->z / 10 + 1] != 1 &&
                  destroy_block[(int)sphereWithSpikes->x / 10]
                               [(int)sphereWithSpikes->z / 10 + 1] != 2)) {
        sphereWithSpikes->direction = 2;
      } else if (num == 2 &&
                 (destroy_block[(int)sphereWithSpikes->x / 10 + 2]
                               [(int)sphereWithSpikes->z / 10 + 1] != 1 &&
                  destroy_block[(int)sphereWithSpikes->x / 10 + 2]
                               [(int)sphereWithSpikes->z / 10 + 1] != 2)) {
        sphereWithSpikes->direction = 3;
      } else {
        sphereWithSpikes->direction = 0;
      }
    } else {
      sphereWithSpikes->z += 1;
    }
  }
  if (sphereWithSpikes->direction == 2) {
    if ((int)sphereWithSpikes->x % 10 == 0) {
      int num = rand() % 3;
      if (num == 0 && (destroy_block[(int)sphereWithSpikes->x / 10]
                                    [(int)sphereWithSpikes->z / 10 + 1] != 1 &&
                       destroy_block[(int)sphereWithSpikes->x / 10]
                                    [(int)sphereWithSpikes->z / 10 + 1] != 2)) {
        sphereWithSpikes->x -= 1;
      } else if (num == 1 &&
                 (destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                               [(int)sphereWithSpikes->z / 10] != 1 &&
                  destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                               [(int)sphereWithSpikes->z / 10] != 2)) {
        sphereWithSpikes->direction = 0;
      } else if (num == 2 &&
                 (destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                               [(int)sphereWithSpikes->z / 10 + 2] != 1 &&
                  destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                               [(int)sphereWithSpikes->z / 10 + 2] != 2)) {
        sphereWithSpikes->direction = 1;
      } else {
        sphereWithSpikes->direction = 3;
      }
    } else {
      sphereWithSpikes->x -= 1;
    }
  }
  if (sphereWithSpikes->direction == 3) {
    if ((int)sphereWithSpikes->x % 10 == 0) {
      int num = rand() % 3;
      if (num == 0 && (destroy_block[(int)sphereWithSpikes->x / 10 + 2]
                                    [(int)sphereWithSpikes->z / 10 + 1] != 1 &&
                       destroy_block[(int)sphereWithSpikes->x / 10 + 2]
                                    [(int)sphereWithSpikes->z / 10 + 1] != 2)) {
        sphereWithSpikes->x += 1;
      } else if (num == 1 &&
                 (destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                               [(int)sphereWithSpikes->z / 10] != 1 &&
                  destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                               [(int)sphereWithSpikes->z / 10] != 2)) {
        sphereWithSpikes->direction = 0;
      } else if (num == 2 &&
                 (destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                               [(int)sphereWithSpikes->z / 10 + 2] != 1 &&
                  destroy_block[(int)sphereWithSpikes->x / 10 + 1]
                               [(int)sphereWithSpikes->z / 10 + 2] != 2)) {
        sphereWithSpikes->direction = 1;
      } else {
        sphereWithSpikes->direction = 2;
      }
    } else {
      sphereWithSpikes->x += 1;
    }
  }
}

void Timer(int id) {
  id = ID;

  if (sphereWithSpikes1.alive == 0 && sphereWithSpikes2.alive == 0) {
    game_over = 2;
    animation_ongoing = 0;
  }

  if (id != 0) {
    if (move == 45)
      indicador = -1;
    else if (move == -45)
      indicador = 1;
    if (indicador == 1)
      move += 5;
    else if (indicador == -1)
      move -= 5;

    if (bomberman.old_direction != bomberman.new_direction) {
      if (bomberman.old_direction < bomberman.new_direction) {
        if (bomberman.new_direction - bomberman.old_direction <= 0.2) {
          bomberman.old_direction = bomberman.new_direction;
        } else
          bomberman.old_direction += 0.3;
      } else {
        if (bomberman.old_direction - bomberman.new_direction <= 0.2) {
          bomberman.old_direction = bomberman.new_direction;
        } else
          bomberman.old_direction -= 0.3;
      }
    }
  }

  if (id == 1) {
    bomberman.z--;
    if ((int)bomberman.z % 10 == 0) {
      if (KEY_DOWN_W == 0 ||
          destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10] ==
              1 ||
          destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10] ==
              2) {
        ID = 0;
      }
    }
  }

  if (id == 2) {
    bomberman.z++;
    if ((int)bomberman.z % 10 == 0) {
      if (KEY_DOWN_S == 0 ||
          destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10 + 2] ==
              1 ||
          destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10 + 2] ==
              2) {
        ID = 0;
      }
    }
  }

  if (id == 3) {
    bomberman.x--;
    if ((int)bomberman.x % 10 == 0) {
      if (KEY_DOWN_A == 0 ||
          destroy_block[(int)bomberman.x / 10][(int)bomberman.z / 10 + 1] ==
              1 ||
          destroy_block[(int)bomberman.x / 10][(int)bomberman.z / 10 + 1] ==
              2) {
        ID = 0;
      }
    }
  }

  if (id == 4) {
    bomberman.x++;
    if ((int)bomberman.x % 10 == 0) {
      if (KEY_DOWN_D == 0 ||
          destroy_block[(int)bomberman.x / 10 + 2][(int)bomberman.z / 10 + 1] ==
              1 ||
          destroy_block[(int)bomberman.x / 10 + 2][(int)bomberman.z / 10 + 1] ==
              2) {
        ID = 0;
      }
    }
  }

  if (ID == 0) {
    if (KEY_DOWN_W &&
        destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10] != 1 &&
        destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10] != 2) {
      ID = 1;
      bomberman.new_direction = 2.0;
    } else if (KEY_DOWN_S &&
               destroy_block[(int)bomberman.x / 10 + 1]
                            [(int)bomberman.z / 10 + 2] != 1 &&
               destroy_block[(int)bomberman.x / 10 + 1]
                            [(int)bomberman.z / 10 + 2] != 2) {
      ID = 2;
      bomberman.new_direction = 0.0;
    } else if (KEY_DOWN_A &&
               destroy_block[(int)bomberman.x / 10]
                            [(int)bomberman.z / 10 + 1] != 1 &&
               destroy_block[(int)bomberman.x / 10]
                            [(int)bomberman.z / 10 + 1] != 2) {
      ID = 3;
      bomberman.new_direction = -1.0;
    } else if (KEY_DOWN_D &&
               destroy_block[(int)bomberman.x / 10 + 2]
                            [(int)bomberman.z / 10 + 1] != 1 &&
               destroy_block[(int)bomberman.x / 10 + 2]
                            [(int)bomberman.z / 10 + 1] != 2) {
      ID = 4;
      bomberman.new_direction = 1.0;
    }
  }

  if (bomba.ind == 1) {
    bomba.bomb_pump++;
  }

  sphereWithSpikes_count++;
  sphereWithSpikes_param = PI * sin(sphereWithSpikes_count);

  if (sphereWithSpikes1.alive) {
    EnemyWalk(&sphereWithSpikes1);
    if (fabsf(sphereWithSpikes1.x - bomberman.x) <= 5 &&
        fabsf(sphereWithSpikes1.z - bomberman.z) <= 5) {
      died = 1;
      diedSpike = 1;
      glutTimerFunc(10, dying, 1);
    }
  }

  if (sphereWithSpikes2.alive) {
    EnemyWalk(&sphereWithSpikes2);
    if (fabsf(sphereWithSpikes2.x - bomberman.x) <= 5 &&
        fabsf(sphereWithSpikes2.z - bomberman.z) <= 5) {
      died = 1;
      diedSpike = 1;
      glutTimerFunc(10, dying, 1);
    }
  }

  if (animation_ongoing)
    glutTimerFunc(TIMER_INTERVAL, Timer, id);

  glutPostRedisplay();
}

void DrawMap() {
  glPushMatrix();
  glTranslatef(-1, -0.5, -1);

  glPushMatrix();
  glColor3f(.1, .3, .1);
  glTranslatef(8, .2, 5.5);
  glScalef(17, .5, 12);
  glutSolidCube(1);
  glPopMatrix();

  int i;
  for (i = 0; i < 13; i++) {
    int j;
    for (j = 0; j < 17; j++) {
      if (i == 0 || i == 12 || j == 0 || j == 16 ||
          (j % 2 == 0 && i % 2 == 0)) {
        glPushMatrix();
        glScalef(1, .7, 1);
        glTranslatef(0, 1, 0);
        ApplyTexture(1.0, names[2]);
        glPopMatrix();
      } else {
        glColor3f(.1, .5, .1);
        glutSolidCube(.97);
        if (destroy_block[j][i] == 2) {
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

float v_vertical = -1.95;
float v_lados = 0.0;
int gameModeView = 0;
int controleInterativo = 0;

void configCamera(void);

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    // ESC = Exit
    glDeleteTextures(3, names);
    exit(0);
    break;
  case 13:
    // ENTER = Start ou Pause
    if (animation_ongoing == 0) {
      animation_ongoing = 1;
      glutTimerFunc(TIMER_INTERVAL, Timer, 0);
    } else if (animation_ongoing == 1) {
      animation_ongoing = 0;
    }
    break;
  case 'R':
  case 'r':
    // Restart
    animation_ongoing = 0;
    ID = 0;
    bomberman.x = 0;
    bomberman.z = 0;
    indicador = 1;
    move = 0.0;
    sphereWithSpikes2.alive = 1;
    sphereWithSpikes1.alive = 1;
    bomba.bomb_pump = 0;
    bomba.ind = 0;
    bomba.x = -1;
    bomba.z = -1;
    v_lados = 0;
    RandomWalls();
    glutPostRedisplay();
    break;
    case 'W':
    case 'w':
        KEY_DOWN_W = 1;
        KEY_DOWN++;
        if (animation_ongoing == 1 &&
            (destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10] != 1 &&
             destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10] !=
                 2) &&
            (int)bomberman.x % 10 == 0 && (int)bomberman.z % 10 == 0) {
          bomberman.new_direction = 2.0;
          glutTimerFunc(10, running, 1);
          ID = 1;
        }
        stop = 0;
        glutTimerFunc(10, running, 1);
        break;
  case 'S':
  case 's':
    KEY_DOWN_S = 1;
    KEY_DOWN++;
    if (animation_ongoing == 1 &&
        (destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10 + 2] !=
             1 &&
         destroy_block[(int)bomberman.x / 10 + 1][(int)bomberman.z / 10 + 2] !=
             2) &&
        (int)bomberman.x % 10 == 0 && (int)bomberman.z % 10 == 0) {
      bomberman.new_direction = 0.0;
      glutTimerFunc(10, running, 1);
      ID = 2;
    }
    stop = 0;
    glutTimerFunc(10, running, 1);
    break;
  case 'A':
  case 'a':
    KEY_DOWN_A = 1;
    KEY_DOWN++;
    if (animation_ongoing == 1 &&
        (destroy_block[(int)bomberman.x / 10][(int)bomberman.z / 10 + 1] != 1 &&
         destroy_block[(int)bomberman.x / 10][(int)bomberman.z / 10 + 1] !=
             2) &&
        (int)bomberman.x % 10 == 0 && (int)bomberman.z % 10 == 0) {
      bomberman.new_direction = -1.0;
      glutTimerFunc(10, running, 1);
      ID = 3;
    }
    stop = 0;
    glutTimerFunc(10, running, 1);
    break;
  case 'D':
  case 'd':
    KEY_DOWN_D = 1;
    KEY_DOWN++;
    if (animation_ongoing == 1 &&
        (destroy_block[(int)bomberman.x / 10 + 2][(int)bomberman.z / 10 + 1] !=
             1 &&
         destroy_block[(int)bomberman.x / 10 + 2][(int)bomberman.z / 10 + 1] !=
             2) &&
        (int)bomberman.x % 10 == 0 && (int)bomberman.z % 10 == 0) {
      bomberman.new_direction = 1.0;

      ID = 4;
    }
    stop = 0;
    glutTimerFunc(10, running, 1);
    break;
  case 32:
    // SPACE = Arma Bomba
    if (animation_ongoing == 1 && bomba.ind == 0) {
      bomba.ind = 1;
      bomba.x = round(bomberman.x / 10);
      bomba.z = round(bomberman.z / 10);
      destroy_block[bomba.x + 1][bomba.z + 1] = 1;
    }
    break;
case  '6':
    if (gameModeView == 1) {
        v_lados += 0.03;
        configCamera();
        break;
    }
case  '4':
    if (gameModeView == 1) {
        v_lados -= 0.03;
        configCamera();
        break;
    }
case  '8':
    if (gameModeView == 1) {
        v_vertical += 0.1;
        configCamera();
    }
    break;
case  '2':
    if (gameModeView == 1) {
        v_vertical -= 0.1;
        configCamera();
    }
    break;
case  '0':
    gameModeView = 0;
    glutPostRedisplay();
    break;
case  '5':
    gameModeView = 1;
    glutPostRedisplay();
    break;
  }
}

void keyboardUp(unsigned char key, int x, int y) {
  switch (key) {
  case 'W':
  case 'w':
    KEY_DOWN_W = 0;
    KEY_DOWN--;
    stop = 1;
    break;
  case 'S':
  case 's':
    KEY_DOWN_S = 0;
    KEY_DOWN--;
    stop = 1;
    break;
  case 'A':
  case 'a':
    KEY_DOWN_A = 0;
    KEY_DOWN--;
    stop = 1;
    break;
  case 'D':
  case 'd':
    KEY_DOWN_D = 0;
    KEY_DOWN--;
    stop = 1;
    break;
  }
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (float)width / height, 1, 100);
  glutFullScreen();
}
void configCamera(void) {
    glMatrixMode(GL_MODELVIEW); //define que a matrix é a model view
    glLoadIdentity(); //carrega a matrix de identidade

    float xp0, yp0, zp0;
    float xN, yN, zN;
    yp0 = 0.0;

    xN = bomberman.x / 10;
    yN = v_vertical;
    zN = bomberman.z / 10;

    //rotacao da camera sobre o eixo do personagem
    xp0 = ((-2.5) * sin(bomberman.old_direction) + (-2.5) * cos(bomberman.old_direction) + xN);
    zp0 = ((-2.5) * cos(bomberman.old_direction) - (-2.5) * sin(bomberman.old_direction) + zN);

    if (v_lados != 0) {// leve rotacao na propria camera
        xN = ((1.5) * sin(bomberman.old_direction - v_lados) + (1.5) * cos(bomberman.old_direction- v_lados) + xp0);
        zN = ((1.5) * cos(bomberman.old_direction- v_lados) - (1.5) * sin(bomberman.old_direction- v_lados) + zN);
    }

    gluLookAt(xp0, yp0, zp0, //P0 - posição da câmera
        xN, yN, zN, //para onde a câmera aponta
        0.0, 1.0, 0.0); //vetor view-up
}

void display() {
  if (game_over != 0) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    animation_ongoing = 0;
    switch (game_over) {
    case 1:
      draw_game_over(1);
      break;
    case 2:
      draw_game_over(2);
      break;
    }
  } else {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gameModeView == 0) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(7, 15, 10, 7, 0, 5, 0, 1, 0);

     }
        else {
            configCamera();
            glPushMatrix(); //Permitir movimento da camera
            glTranslatef(0.0, -3.5, 0.0);
        }

    // Base do Mapa
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
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    Bomberman(1.0, 1.0, 1.0);
    glPopMatrix();

    if (bomba.ind == 1) {
      glPushMatrix();
      glColor3f(0.7, 0, 0);
      draw_bomb();
      glPopMatrix();
    }

    glPushMatrix();
    if (sphereWithSpikes1.alive) {
      glPushMatrix();
      drawSphereWithSpikes(sphereWithSpikes1.x / 10, sphereWithSpikes1.z / 10);
      glPopMatrix();
    }
    if (sphereWithSpikes2.alive) {
      glPushMatrix();
      drawSphereWithSpikes(sphereWithSpikes2.x / 10, sphereWithSpikes2.z / 10);
      glPopMatrix();
    }
    glPopMatrix();
  }
  if (gameModeView == 1) glPopMatrix();
  glutSwapBuffers();
}

void init() {
  glClearColor(0.0, 0., 0.0, 0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

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

  // Adicionando os inimigos
  sphereWithSpikes1.direction = rand() % 4;
  sphereWithSpikes2.direction = rand() % 4;

  // Inicializando os inimigos
  sphereWithSpikes1.alive = 1;
  sphereWithSpikes2.alive = 1;

  // Inicializando a bomba
  bomba.x = -1;
  bomba.z = -1;
  bomba.ind = 0;
  bomba.bomb_pump = 0;

  RandomWalls();
  InitializeTexture();
  loadTexture();

  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowSize(800, 600);
  glutInitWindowPosition(0, 0);
  glutCreateWindow(argv[0]);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);

  init();

  glutMainLoop();

  return 0;
}

// =================================== FIM ===================================
// //
