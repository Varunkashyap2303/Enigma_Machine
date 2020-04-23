

#include <math.h>
#include <stdlib.h>
#include<string.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.14159265
#endif

/**


  Input:  inner_radius - radius of hole at center
          outer_radius - radius at center of teeth
          width - width of gear
          teeth - number of teeth
          tooth_depth - depth of tooth

 **/
char str[28]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
static void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
  GLint teeth, GLfloat tooth_depth)
{
  GLint i;
  GLfloat r0, r1, r2;
  GLfloat angle, da;
  GLfloat u, v, len;

  r0 = inner_radius;
  r1 = outer_radius - tooth_depth / 2.0;
  r2 = outer_radius + tooth_depth / 2.0;

  da = 2.0 * M_PI / teeth / 4.0;

  glShadeModel(GL_FLAT);

  glNormal3f(0.0, 0.0, 1.0);

  /* draw front face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
  }
  glEnd();

  /* draw front sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * M_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
  }
  glEnd();

  glNormal3f(0.0, 0.0, -1.0);

  /* draw back face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
  }
  glEnd();

  /* draw back sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * M_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
  }
  glEnd();

  /* draw outward faces of teeth */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    u = r2 * cos(angle + da) - r1 * cos(angle);
    v = r2 * sin(angle + da) - r1 * sin(angle);
    len = sqrt(u * u + v * v);
    u /= len;
    v /= len;


    glNormal3f(v, -u, 0.0);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
    u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
    v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
    glNormal3f(v, -u, 0.0);

    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
  }

  glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
  glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

  glEnd();

  glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  glBegin(GL_QUAD_STRIP);

    angle = i * 2.0 * M_PI / teeth;

    glNormal3f(-cos(angle), -sin(angle), 0.0);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);

  glEnd();
}

static GLfloat view_rotx = 20.0, view_roty = -50.0, view_rotz = 0.0;
static GLint gear1, gear2, gear3,gear4,gear5,gear6,gear7,gear8;
static GLfloat angle = 0.0;

static GLuint limit;
static GLuint count = 1;

static void draw()
{


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef(view_rotx, 1.0, 0.0, 0.0);
  glRotatef(view_roty, 0.0, 1.0, 0.0);
  glRotatef(view_rotz, 0.0, 0.0, 1.0);

  glPushMatrix();
  glTranslatef(6.2, 3.7, 6.0);
  glRotatef(angle, 0.0, 0.0, 1.0);
  glCallList(gear1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(6, 3.5, 4.0);
  glRotated( angle , 0.0, 0.0, 1.0);
  glCallList(gear2);
  glPopMatrix();


  glPushMatrix();
  glTranslatef(6.4, 3.9, 8.0);
  glRotatef(angle, 0.0, 0.0, 1.0);
  glCallList(gear3);
  glPopMatrix();

//last one
   glPushMatrix();
  glTranslatef(6.4, 3.9, 8.8);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glCallList(gear4);
    glPopMatrix();

//first one
   glPushMatrix();
  glTranslatef(6, 3.5, 4.8);
  glRotated( angle , 0.0, 0.0, 1.0);

  glCallList(gear5);
  glPopMatrix();

//middle one
   glPushMatrix();
  glTranslatef(6.2, 3.7, 6.8);
  glRotatef(angle, 0.0, 0.0, 1.0);
  glCallList(gear6);
  glPopMatrix();


  glPushMatrix();
  glTranslatef(6, 3.5, 3.5);
  glRotated( angle , 0.0, 0.0, 1.0);
glCallList(gear7);
  glPopMatrix();

   glPushMatrix();
  glTranslatef(6.4, 3.9, 8.8);
  glRotated( angle , 0.0, 0.0, 1.0);
glCallList(gear8);
glPopMatrix();

  glPopMatrix();

  glutSwapBuffers();

  if (count == limit) {
    exit(0);
  }
}

static void idle()
{

  angle += 2.0;

  glutPostRedisplay();
}
static void
key(unsigned char k, int x, int y)
{
  switch (k) {
  case 'z':
    view_rotz += 2.0;
    break;
  case 'Z':
    view_rotz -= 2.0;
    break;

  case 27:  /* Escape */
    exit(0);
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

/* change view angle */
/* ARGSUSED1 */
static void
special(int k, int x, int y)
{
  switch (k) {
  case GLUT_KEY_UP:
    view_rotx += 2.0;
    break;
  case GLUT_KEY_DOWN:
    view_rotx -= 2.0;
    break;
  case GLUT_KEY_LEFT:
    view_roty += 2.0;
    break;
  case GLUT_KEY_RIGHT:
    view_roty -= 2.0;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}
/* new window size or exposure */
static void reshape(int width, int height)
{
  GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -40.0);

}

static void init(void)
{


  static GLfloat pos[4] =
  {5.0, 5.0, 10.0, 0.0};
  static GLfloat color[4] =
  {0.0, 0.5, 0.5, 0.5};
  static GLfloat grey[4] =
  {0.3, .3, 0.3, 0.3};
  static GLfloat yellow[4] =
  {0.8, .8, 0.0, 0.3};



  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  gluOrtho2D(0,500,0,500);


  /* make the gears */
  gear1 = glGenLists(1);
  glNewList(gear1, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  gear(0.5, 2.0, .5, 15, 0.3);
  glEndList();

  gear2 = glGenLists(1);
  glNewList(gear2, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  gear(0.5, 2.0, .5, 15, 0.3);
  glEndList();

  gear3 = glGenLists(1);
  glNewList(gear3, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  gear(0.5, 2.0, .5, 15, 0.3);
  glEndList();

  gear4 = glGenLists(1);
  glNewList(gear4, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
  gear(0.2, 1.5, 1, 26, 0);

  glEndList();

  gear5 = glGenLists(1);
  glNewList(gear5, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
  gear(0.2, 1.5, 1, 26, 0);
  glEndList();

  gear6 = glGenLists(1);
  glNewList(gear6, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);
  gear(0.2, 1.5, 1, 26, 0);
  glEndList();

  gear7 = glGenLists(1);
  glNewList(gear7, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
  gear(0.0, .2, 5, 10, 0);
  glEndList();

  gear8 = glGenLists(1);
  glNewList(gear8, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
  gear(0.0, .2, 4, 10, 0);
  glEndList();

  glEnable(GL_NORMALIZE);
}

void visible(int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc(idle);

  else
    glutIdleFunc(NULL);
}




int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowPosition(100,50);
  glutInitWindowSize(500,500);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutCreateWindow("ENIGMA WHEELS");
  init();

  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutVisibilityFunc(visible);


  glutMainLoop();
  return 0;
}

