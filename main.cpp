// Projeto Final
// ST765, 1sem23, Matheus Roberto Mariano 246029 e Gabriel Marino 247455
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include "image.h"
#define TEXTURA_DO_PLANO "condominio.sgi"

GLfloat mat_1_difusa[] = {1.0, 0.0, 0.0, 1.0}; // placa condominio textura
GLfloat mat_1_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_1_brilho[] = {50.0};

GLfloat mat_2_difusa[] = {0.8, 0.8, 0.8, 1.0}; // placa do condom?nio
GLfloat mat_2_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_2_brilho[] = {50.0};

GLfloat mat_3_especular[] = {0.0, 0.0, 0.0, 1.0};
GLfloat mat_3_difusa[] = {0.0, 0.5, 0.0, 1.0}; // chao
GLfloat mat_3_brilho[] = {100.0};

GLfloat mat_4_difusa[] = {1.0, 1.0, 1.0, 1.0}; // casa
GLfloat mat_4_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_4_brilho[] = {50.0};

GLfloat mat_5_difusa[] = {0.5, 0.0, 0.0, 1.0}; // telhado
GLfloat mat_5_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_5_brilho[] = {50.0};

GLfloat mat_6_difusa[] = {0.8, 0.8, 1.0, 1.0}; // janela da casa
GLfloat mat_6_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_6_brilho[] = {50.0};

GLfloat mat_7_difusa[] = {1.0, 0.0, 0.0, 1.0}; // cancela
GLfloat mat_7_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_7_brilho[] = {50.0};

GLfloat mat_8_difusa[] = {1.0, 1.0, 0.0, 1.0}; // base da cancela
GLfloat mat_8_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_8_brilho[] = {50.0};

GLfloat mat_9_difusa[] = {0.1216, 0.6118, 0.5294, 1.0}; // carro e cerca
GLfloat mat_9_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_9_brilho[] = {50.0};

GLfloat mat_10_difusa[] = {0.1, 0.1, 0.1, 1.0}; // roda
GLfloat mat_10_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_10_brilho[] = {50.0};

GLfloat mat_11_difusa[] = {0.7, 0.7, 0.7}; // meteoro
GLfloat mat_11_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_11_brilho[] = {50.0};

GLfloat mat_12_difusa[] = {1.0, 1.0, 1.0, 0.5}; // explosao
GLfloat mat_12_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_12_brilho[] = {50.0};

GLfloat mat_13_difusa[] = {1.0, 0.8, 0.4, 1.0}; // porteira
GLfloat mat_13_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_13_brilho[] = {50.0};

GLfloat posicao_luz[] = {0.0, 9.0, 0.0, 1.0};
GLfloat cor_luz[] = {1.0, 1.0, 1.0, 1.0};
GLfloat cor_luz_amb[] = {0.3, 0.3, 0.3, 1.0};

GLfloat sphere_position[] = {13.3, 15.0, -5.0};

static int rot = 0;
static float port = 0;
float zoom_factor = 1.0;
float andar = 0;   // carro
float cair = 0;    // meteoro
float aumenta = 1; // explosao
float rotMeteoro = 0;
static float portFechar = 0;

static bool animationRunning = false;

GLuint textura_plano;

GLfloat planotext[4][2] = {
    {0, 0},
    {+1, 0},
    {+1, +1},
    {0, +1}};

void carregar_texturas(void)
{
  IMAGE *img;
  GLenum gluerr;

  /* textura do plano */
  glGenTextures(1, &textura_plano);
  glBindTexture(GL_TEXTURE_2D, textura_plano);

  if (!(img = ImageLoad(TEXTURA_DO_PLANO)))
  {
    fprintf(stderr, "Error reading a texture.\n");
    exit(-1);
  }

  gluerr = gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
                             img->sizeX, img->sizeY,
                             GL_RGB, GL_UNSIGNED_BYTE,
                             (GLvoid *)(img->data));
  if (gluerr)
  {
    fprintf(stderr, "GLULib%s\n", gluErrorString(gluerr));
    exit(-1);
  }

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void init(void)
{
  glClearColor(0.6, 0.8, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
  glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
  glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz_amb);
  glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);

  carregar_texturas();
  glShadeModel(GL_FLAT);
}

void ambiente(void)
{
  /* placa condominio - textura*/
  if (cair <= 13)
  {
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2fv(planotext[0]);
    glVertex3f(-2.5, 0.51, 2.52);
    glTexCoord2fv(planotext[1]);
    glVertex3f(-1.48, 0.51, 2.52);
    glTexCoord2fv(planotext[2]);
    glVertex3f(-1.48, 1.12, 2.52);
    glTexCoord2fv(planotext[3]);
    glVertex3f(-2.5, 1.12, 2.52);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0, 1.0, 0.0);
  }

  /* propriedades do material 2 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_2_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_2_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_2_brilho);
  // placa do condominio
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(-15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, aumenta);
  }
  glTranslatef(-2.0, 0.8, 2.51);
  glScalef(1, 0.5, 0);
  glutSolidCube(1.0);
  glPopMatrix();

  /* propriedades do material 4 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_4_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_4_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_4_brilho);
  /*casa*/
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(15 * aumenta, 0, 1, 1);
    glTranslatef(0, aumenta, aumenta);
  }
  glTranslatef(-2.0, 1.5, 1.5);
  glScalef(2, 3, 2);
  glutSolidCube(1.0);
  glPopMatrix();

  // telhado
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(-2.0, 4, 1.5); // move a piramide para a nova posicao
  glBegin(GL_TRIANGLES);
  
  /* propriedades do material 5 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_5_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_5_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_5_brilho);
  glNormal3f(0.0, 5.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0);   // topo
  glVertex3f(-1.0, -1.0, 1.0); // base esquerda
  glVertex3f(1.0, -1.0, 1.0);  // base direita

  glVertex3f(0.0, 1.0, 0.0);   // topo
  glVertex3f(1.0, -1.0, 1.0);  // base direita
  glVertex3f(1.0, -1.0, -1.0); // base esquerda

  glVertex3f(0.0, 1.0, 0.0);    // topo
  glVertex3f(1.0, -1.0, -1.0);  // base esquerda
  glVertex3f(-1.0, -1.0, -1.0); // base direita
  glEnd();
  glPopMatrix();

  /* propriedades do material 6 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_6_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_6_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_6_brilho);
  //janela da casa
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(-15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(-2.0, 1.7, 2.51);
  glScalef(1.5, 1, 0.01);
  glutSolidCube(1.0);
  glPopMatrix();

  /* propriedades do material 7 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_7_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_7_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_7_brilho);
  //cancela
  glPushMatrix();
  glTranslatef(1.3, 1.0, 2.0);
  if (cair > 13)
  {
    glRotatef(10 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  
  //rotacao da cancela como se estivesse fixa no suporte
  glTranslatef(-1.7, 0.0, -0.0);
  glRotatef(port, 0.0f, 0.0f, 1.0f);
  glTranslatef(1.7, 0.0, 0.0);
  glScalef(3.4, 0.08, 0.08);
  glutSolidCube(1.0);
  glPopMatrix();

  /* propriedades do material 8 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_8_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_8_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_8_brilho);
  //base cancela
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(-0.4, 0.6, 2.0);
  glScalef(0.1, 1.0, 0.1);
  glutSolidCube(1.0);
  glPopMatrix();
}

void roda(float x, float y, float z)
{
  //roda do carro
  float th;
  float j;
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(-15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  for (j = 0; j < 0.2; j += 0.001)
  {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
      th = i * 3.142 / 180;
      glVertex3f(x + j, y + 0.2 * sin(th), z + 0.2 * cos(th));
    }
    glEnd();
  }
  glPopMatrix();
}

void carro(void)
{
  /* propriedades do material 8 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_9_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_9_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_9_brilho);

  // esquerda
  glPushMatrix();
  glTranslatef(0.25, 0.6, 7 - andar);
  if (cair > 13)
  {
    glRotatef(-15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glScalef(0.01, 0.5, 3.0);
  glutSolidCube(1);
  glPopMatrix();

  // traseira
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(-15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(1, 0.6, 5.5 - andar);
  glScalef(1.5, 0.5, 0.01);
  glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(-15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(1, 1.02, 5.68 - andar);
  glRotatef(45, 1, 0, 0);
  glScalef(1.5, 0.5, 0.01);
  glutWireCube(1);
  glPopMatrix();

  // frente
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(1, 0.6, 8.5 - andar);
  glScalef(1.5, 0.5, 0.01);
  glutSolidCube(1);
  glPopMatrix();

  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(-15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(1, 1.02, 8.33 - andar);
  glRotatef(-45, 1, 0, 0);
  glScalef(1.5, 0.5, 0.01);
  glutWireCube(1);
  glPopMatrix();

  // direita
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(5 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(1.75, 0.6, 7 - andar);
  glScalef(0.01, 0.5, 3.0);
  glutSolidCube(1);
  glPopMatrix();

  // topo
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(1, 1.20, 7 - andar);
  glScalef(1.5, 0.01, 2.3);
  glutSolidCube(1);
  glPopMatrix();

  // chao
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(15 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(1, 0.35, 7 - andar);
  glScalef(1.5, 0.01, 3);
  glutSolidCube(1);
  glPopMatrix();

  /* propriedades do material 10 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_10_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_10_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_10_brilho);
  roda(0.2, 0.3, 6.3 - andar);
  roda(0.2, 0.3, 7.8 - andar);
  roda(1.6, 0.3, 6.3 - andar);
  roda(1.6, 0.3, 7.8 - andar);
}

void porteira(void)
{
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_13_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_13_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_13_brilho);

  // estaca lado esquerdo frente/
  glColor3f(0.0, 0.0, 1.0);
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(-4.0, 0.9, 2.5);
  glScalef(0.1, 1.6, 0.1);
  glutSolidCube(1.0);
  glPopMatrix();

  // estaca lado direito frente/
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(4.0, 0.9, 2.5);
  glScalef(0.1, 1.6, 0.1);
  glutSolidCube(1.0);
  glPopMatrix();

  // estaca lado esquerdo frente2/
  glColor3f(0.0, 0.0, 1.0);
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, 1, 1, -1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(-6.75, 0.9, 2.5);
  glScalef(0.1, 1.6, 0.1);
  glutSolidCube(1.0);
  glPopMatrix();

  // estaca lado direito frente2/
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, -1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(6.75, 0.9, 2.5);
  glScalef(0.1, 1.6, 0.1);
  glutSolidCube(1.0);
  glPopMatrix();

  // direita
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, 1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(5.5, 0.5, 2.7);
  glScalef(5, 0.1, 0.1);
  glutSolidCube(1.0);
  glPopMatrix();

  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, 1, -1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(5.5, 1.0, 2.7);
  glScalef(5, 0.1, 0.1);
  glutSolidCube(1.0);
  glPopMatrix();

  // esquerda
  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, 1, 1, -1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(-5.5, 0.5, 2.7);
  glScalef(5, 0.1, 0.1);
  glutSolidCube(1.0);
  glPopMatrix();

  glPushMatrix();
  if (cair > 13)
  {
    glRotatef(10 * aumenta, -1, 1, 1);
    glTranslatef(0, aumenta, 0);
  }
  glTranslatef(-5.5, 1.0, 2.7);
  glScalef(5, 0.1, 0.1);
  glutSolidCube(1.0);
  glPopMatrix();
}

void meteoro(void)
{

  GLfloat light_position[] = {sphere_position[0] - cair, sphere_position[1] - 1.75 - cair, sphere_position[2], 1.0};
  GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

  /* propriedades do material 11 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_11_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_11_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_11_brilho);
  
  // Desenha a esfera
  glPushMatrix();
  glTranslatef(sphere_position[0] - cair, sphere_position[1] - cair, sphere_position[2]);
  glRotatef(rotMeteoro, 1.0, 0.0, 0.0); // Rotacao em torno do eixo X
  glRotatef(rotMeteoro, 0.0, 1.0, 0.0); // Rotacao em torno do eixo Y
  glutSolidSphere(1.5, 80, 20);         // Raio = 1.0, detalhes = 20x20
  glPopMatrix();
}

void explosao(void)
{
  if (cair > 13)
  {
    glEnable(GL_BLEND);                                // Habilitar blending (mistura de cores)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Configurar a funcaoo de blending

    // Configurar material para transparencia
    GLfloat corEsfera[] = {1.0, 1.0, 1.0, 0.2};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, corEsfera);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0); // Desabilitar brilho especular

    // Habilitar transparencia
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0);
    glPushMatrix();
    glTranslatef(0.26, 1.96, -5.0);
    glutSolidSphere(2.5 * aumenta, 50, 20);

    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
  }
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_LIGHTING);
  GLUquadricObj *quadric;

  glPushMatrix();
  glRotatef((GLfloat)rot, 0.0, 1.0, 0.0);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glBindTexture(GL_TEXTURE_2D, textura_plano);

  if (aumenta < 25)
  {
    ambiente();
    carro();
    porteira();
    if (andar > 12 && cair < 13)
    {
      meteoro();
    }
  }

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_3_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_3_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_3_brilho);
  /*chao*/
  glPushMatrix();
  glScalef(16.0, 0.2, 20.0);
  glutSolidCube(1.0);

  glPopMatrix();
  explosao();

  glPopMatrix();
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 40.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0.0, -2.0, -13.0);
}

void stopAnimation()
{
  animationRunning = !animationRunning;
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'w':
    if (zoom_factor < 1)
    {
      zoom_factor = 1;
    }
    zoom_factor += 0.1; // aumenta o fator de zoom
    glScalef(zoom_factor, zoom_factor, zoom_factor);
    glutPostRedisplay();
    break;
  case 'W':
    if (zoom_factor < 1)
    {
      zoom_factor = 1;
    }
    zoom_factor += 0.1; // aumenta o fator de zoom
    glScalef(zoom_factor, zoom_factor, zoom_factor);
    glutPostRedisplay();
    break;
  case 's':
    if (zoom_factor > 1)
    {
      zoom_factor = 1;
    }
    zoom_factor -= 0.1; // diminui o fator de zoom
    glScalef(zoom_factor, zoom_factor, zoom_factor);
    glutPostRedisplay();
    break;
  case 'S':
    if (zoom_factor > 1)
    {
      zoom_factor = 1;
    }
    zoom_factor -= 0.1; // diminui o fator de zoom
    glScalef(zoom_factor, zoom_factor, zoom_factor);
    glutPostRedisplay();
    break;
  case 'a':
    rot = (rot + 5) % 360;
    glutPostRedisplay();
    break;
  case 'A':
    rot = (rot + 5) % 360;
    glutPostRedisplay();
    break;
  case 'd':
    rot = (rot - 5) % 360;
    glutPostRedisplay();
    break;
  case 'D':
    rot = (rot - 5) % 360;
    glutPostRedisplay();
    break;
  case 'q':
    glTranslatef(0, -0.5, 0);
    glutPostRedisplay();
    break;
  case 'Q':
    glTranslatef(0, -0.5, 0);
    glutPostRedisplay();
    break;
  case 'e':
    glTranslatef(0, 0.5, 0);
    glutPostRedisplay();
    break;
  case 'E':
    glTranslatef(0, 0.5, 0);
    glutPostRedisplay();
    break;
  case 32:
    stopAnimation();
    glutPostRedisplay();
    break;
  case 27: // tecla Esc (encerra o programa)
    exit(0);
    break;
  }
}

void specialKeys(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    if (zoom_factor < 1)
    {
      zoom_factor = 1;
    }
    zoom_factor += 0.1; // aumenta o fator de zoom
    glScalef(zoom_factor, zoom_factor, zoom_factor);
    glutPostRedisplay();
    break;
  case GLUT_KEY_DOWN:
    if (zoom_factor > 1)
    {
      zoom_factor = 1;
    }
    zoom_factor -= 0.1; // diminui o fator de zoom
    glScalef(zoom_factor, zoom_factor, zoom_factor);
    glutPostRedisplay();
    break;
  case GLUT_KEY_LEFT:
    rot = (rot + 5) % 360;
    glutPostRedisplay();
    break;
  case GLUT_KEY_RIGHT:
    rot = (rot - 5) % 360;
    glutPostRedisplay();
    break;
  }
}

void updateAnimation(int t)
{
  if (animationRunning)
  {
    if (andar > 3)
    {
      if (port > 77)
      {
        if (andar > 12)
        {
          if (cair > 13)
          {
            aumenta += 0.1;
          }
          else
          {
            cair += 0.13;
            rotMeteoro += 1.5;
          }
        }
        else
        {
          andar += 0.05;
        }
      }
      else
      {
        port += 0.5;
      }
    }
    else
    {
      andar += 0.05;
    }
  }
  glutTimerFunc(10, updateAnimation, 0);
  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  //  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Projeto Final");
  init();
  glutDisplayFunc(display);
  glutTimerFunc(10, updateAnimation, 0);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeys);
  glutMainLoop();
  return 0;
}

