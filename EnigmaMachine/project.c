#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include<GL/glut.h>
#define FORWARD 1
#define BACKWARD 2
#define width 1350
#define height 700
#define true 1
#define false 0

#ifndef M_PI
#define M_PI 3.14159265
#endif

char r1[27][2], r2[27][2], r3[27][2], ref[14][2], plug[27][2]; // rotor and plug variables
char map1[] = "QWERTYUIOPASDFGHJKLZXCVBNM"; // mapping for rotor 1
char map2[] = "MLPOKIJNBHUYGVCFTRDXSEWQAZ"; //mapping for rotor 2
char map3[] = "LKJHGFDSAMPOIUNBYTVCREXZWQ"; // mapping for rotor 3
char plugmap[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //plugboard mapping
char refmap[] = "NOPQRSTUVWXYZ"; // reflector mapping

int index1, index2, index3; // index variables for rotors

char pc1[50][50],pc2[50][50],rc1[50][50],rc2[50][50],rf[50][50];// 2D arrays to store encrypted texts at each functions
int l;//to store the length of string

//Initialize rotors
void init() {
	int i, j;
	for (i = 0; i < 27; i++) {
		r1[i][0] = (char)(((int)'A') + (i));
		r2[i][0] = (char)(((int)'A') + (i));
		r3[i][0] = (char)(((int)'A') + (i));

		r1[i][1] = map1[i];
		r2[i][1] = map2[i];
		r3[i][1] = map3[i];

		if (i < 14) {
			ref[i][0] = (char)(((int)'A') + (i));
			ref[i][1] = refmap[i - 1];
		}

		plug[i][0] = (char)(((int)'A') + (i));
		plug[i][1] = plugmap[i];
	}
	index3 = index2 = index1 = 0;
}

//Function to find character in the array
int findInArray(char c, char arr[], int n) {
	int i;
	for (i = 0; i < n; i++) {
		if (arr[i] == c)
			return i;
	}
	return -1;
}

//Function to send signal through rotors
char passThroughRotors(char ch, int direction) {
	int in, i;
	char c[27];
	if (direction == FORWARD) {
		for (i = 0; i < 27; i++)
			c[i] = r1[i][0];
		in = findInArray(ch, c, 27);
		for (i = 0; i < 27; i++)
			c[i] = r2[i][0];
		in = findInArray(r1[in][1], c, 27);
		for (i = 0; i < 27; i++)
			c[i] = r3[i][0];
		in = findInArray(r2[in][1], c, 27);

		return r3[in][1];
	}
	else {
		for (i = 0; i < 27; i++)
			c[i] = r3[i][1];
		in = findInArray(ch, c, 27);
		for (i = 0; i < 27; i++)
			c[i] = r2[i][1];
		in = findInArray(r1[in][0], c, 27);
		for (i = 0; i < 27; i++)
			c[i] = r1[i][1];
		in = findInArray(r2[in][0], c, 27);

		return r1[in][0];
	}
}

//Function to send signals through reflectors
char passThroughReflectors(char ch) {
	char c[14];
	int i, in;
	for (i = 0; i < 14; i++)
		c[i] = ref[i][0];
	in = findInArray(ch, c, 14);
	if (in == -1) {
		for (i = 0; i < 14; i++)
			c[i] = ref[i][1];
		in = findInArray(ch, c, 14);
		return ref[in][0];
	}
	else {
		return ref[in][1];
	}
}

//Function to send signals through plugboard
char passThroughPlugBoard(char ch, int direction) {
	int i;
	char c[27];
	if (direction == FORWARD) {
		for (i = 0; i < 27; i++)
			c[i] = plug[i][0];
		return(plug[findInArray(ch, c, 27)][1]);
	}
	else {
		for (i = 0; i < 27; i++)
			c[i] = plug[i][1];
		return(plug[findInArray(ch, c, 27)][0]);
	}

}

//Function to rotate rotors once signal has been sent
void rotateRotors() {
	char c0, c1;
	int i;
	c0 = r1[0][0];
	c1 = r1[0][1];
	for (i = 0; i < 25; i++) {
		r1[i][0] = r1[i + 1][0];
	}
	r1[24][0] = c0;
	r1[24][1] = c1;
	index1++;
	if (index1 == 26) {
		index1 = 0;
		c0 = r2[0][0];
		c1 = r2[0][1];
		for (i = 0; i < 25; i++) {
			r2[i][0] = r2[i + 1][0];
		}
		r2[24][0] = c0;
		r2[24][1] = c1;
		index2++;
		if (index2 == 26) {
			index2 = 0;
			c0 = r3[0][0];
			c1 = r3[0][1];
			for (i = 0; i < 25; i++) {
				r3[i][0] = r3[i + 1][0];
			}
			r3[24][0] = c0;
			r3[24][1] = c1;
			index3++;
		}
	}
}

//Graphics initialization
void myinit() {
	gluOrtho2D(0, width, 0, height);
	glClearColor(0, 0, 0, 0);
}

//Function to perform string encryption
void encrypt() {
	char text[200];
	init();
	printf("Enter string to be encoded\n");
	gets(text);
	int len = strlen(text);
	char ch;
	char choice;
	l=len;
	for (int i = 0,j=0; i < len; i++) {
		ch = text[i];
		ch = toupper(ch);
		ch = passThroughPlugBoard(ch, FORWARD);
		pc1[i][j]=ch;
		ch = passThroughRotors(ch, FORWARD);
		rc1[i][j]=ch;
		ch = passThroughReflectors(ch);
		rf[i][j]=ch;
		ch = passThroughRotors(ch, BACKWARD);
		rc2[i][j]=ch;
		ch = passThroughPlugBoard(ch, FORWARD);
		pc2[i][j]=ch;
		printf("%c", ch);

		rotateRotors();

	}
	/*for(int i=0,j=0;i<l;i++){
		printf("\n%c \t",pc1[i][j]);
		printf("%c \t",rc1[i][j]);
		printf("%c \t",rf[i][j]);
		printf("%c \t",rc2[i][j]);
		printf("%c \n",pc2[i][j]);
	}*/

}


//function to display keyboard and lampboard
void displayBoard() {
	char line1[11] = { 'Q','W','E','R','T','Y','U','I','O','P' };
	char line2[10] = { 'A','S','D','F','G','H','J','K','L'};
	char line3[8] = { 'Z','X','C','V','B','N','M',};
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2d(width / 2, 0);
	glVertex2d(width / 2, height);
	glEnd();
	//glColor3f(0, 0, 0);
	for (int i = 0; i < 10; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2d(50 + 60 * i, 500);
		glVertex2d(90 + 60 * i, 500);
		glVertex2d(90 + 60 * i, 460);
		glVertex2d(50 + 60 * i, 460);
		glEnd();
	}
	for (int i = 0; i < 10; i++) {
		glRasterPos2d(60 + 60 * i, 470);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line1[i]);
	}
	for (int i = 0; i < 9; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2d(65 + 60 * i, 440);
		glVertex2d(105 + 60 * i, 440);
		glVertex2d(105 + 60 * i, 400);
		glVertex2d(65 + 60 * i, 400);
		glEnd();
	}
	for (int i = 0; i < 9; i++) {
		glRasterPos2d(75 + 60 * i, 410);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line2[i]);
	}
	for (int i = 0; i < 7; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2d(85 + 60 * i, 380);
		glVertex2d(125 + 60 * i, 380);
		glVertex2d(125 + 60 * i, 340);
		glVertex2d(85 + 60 * i, 340);
		glEnd();
	}
	for (int i = 0; i < 7; i++) {
		glRasterPos2d(95 + 60 * i, 350);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line3[i]);
	}

	for (int i = 0; i < 10; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2d(50 + 60 * i + 675, 500);
		glVertex2d(90 + 60 * i + 675, 500);
		glVertex2d(90 + 60 * i + 675, 460);
		glVertex2d(50 + 60 * i + 675, 460);
		glEnd();
	}
	for (int i = 0; i < 10; i++) {
		glRasterPos2d(60 + 60 * i + 675, 470);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line1[i]);
	}
	for (int i = 0; i < 9; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2d(65 + 60 * i + 675, 440);
		glVertex2d(105 + 60 * i + 675, 440);
		glVertex2d(105 + 60 * i + 675, 400);
		glVertex2d(65 + 60 * i + 675, 400);
		glEnd();
	}
	for (int i = 0; i < 9; i++) {
		glRasterPos2d(75 + 60 * i + 675, 410);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line2[i]);
	}
	for (int i = 0; i < 7; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2d(85 + 60 * i + 675, 380);
		glVertex2d(125 + 60 * i + 675, 380);
		glVertex2d(125 + 60 * i + 675, 340);
		glVertex2d(85 + 60 * i + 675, 340);
		glEnd();
	}
	for (int i = 0; i < 7; i++) {
		glRasterPos2d(95 + 60 * i + 675, 350);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line3[i]);
	}

	glFlush();

}

//Function to display plugboard and rotors
void plugBoard() {
	char line1[11] = { 'Q','W','E','R','T','Y','U','I','O','P' };
	char line2[10] = { 'A','S','D','F','G','H','J','K','L'};
	char line3[8] = { 'Z','X','C','V','B','N','M',};
	char p[]={'P','L','U','G',' ','B','O','A','R','D'};
	char r[]={'R','O','T','O','R','S'};
	char o[]={'O','U','T','P','U','T',':'};
	double angle;
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, .5,.5);

	//glColor3f(0, 0, 0);
	glRasterPos2d(200,600);
	for(int i=0;i<10;i++)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,p[i]);

	glRasterPos2d(900,600);
	for(int i=0;i<6;i++)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,r[i]);

	glRasterPos2d(150,150);
	for(int i=0;i<7;i++)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,o[i]);



	glColor3f(1,1,1);

	for (int i = 0; i < 10; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2d(50 + 60 * i, 500);
		glVertex2d(90 + 60 * i, 500);
		glVertex2d(90 + 60 * i, 460);
		glVertex2d(50 + 60 * i, 460);
		glEnd();
	}
	for (int i = 0; i < 10; i++) {
		glRasterPos2d(60 + 60 * i, 470);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line1[i]);
	}
	for (int i = 0; i < 9; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2d(65 + 60 * i, 440);
		glVertex2d(105 + 60 * i, 440);
		glVertex2d(105 + 60 * i, 400);
		glVertex2d(65 + 60 * i, 400);
		glEnd();
	}
	for (int i = 0; i < 9; i++) {
		glRasterPos2d(75 + 60 * i, 410);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line2[i]);
	}
	for (int i = 0; i < 7; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex2d(85 + 60 * i, 380);
		glVertex2d(125 + 60 * i, 380);
		glVertex2d(125 + 60 * i, 340);
		glVertex2d(85 + 60 * i, 340);
		glEnd();
	}
	for (int i = 0; i < 7; i++) {
		glRasterPos2d(95 + 60 * i, 350);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line3[i]);
	}

	glColor3f(.5,.5,.5);
	 glBegin(GL_QUAD_STRIP);

		for(int i =0;i<=24;i++){

			angle=3.14/12*i;
			glVertex2d(50*cos(angle)+800,50*sin(angle)+400);
			glVertex2d(55*cos(angle)+800,55*sin(angle)+400);
		}
	glEnd();



	glBegin(GL_QUAD_STRIP);

		for(int i =0;i<=24;i++){

			angle=3.14/12*i;
			glVertex2d(50*cos(angle)+1000,50*sin(angle)+400);
			glVertex2d(55*cos(angle)+1000,55*sin(angle)+400);
		}
	glEnd();


	glBegin(GL_QUAD_STRIP);

		for(int i =0;i<=24;i++){

			angle=3.14/12*i;
			glVertex2d(50*cos(angle)+1200,50*sin(angle)+400);
			glVertex2d(55*cos(angle)+1200,55*sin(angle)+400);
		}
	glEnd();
	glRasterPos2d(cos(angle)+790,sin(angle)+395);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'1');

	glRasterPos2d(cos(angle)+990,sin(angle)+395);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'2');

	glRasterPos2d(cos(angle)+1190,sin(angle)+395);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'3');
	glFlush();

}

//Function to display arrows in visluaisation
void movement()
{
	glColor3f(.5,1,.5);
	glEnable(GL_LINE_STIPPLE);
	glLineWidth(3);
	glLineStipple(1,0x0F0F);
	glBegin(GL_LINES);
		glVertex2d(550,370);
		glVertex2d(720,370);

		glVertex2d(870,390);
		glVertex2d(930,390);

		 glVertex2d(1070,390);
		glVertex2d(1130,390);

		glVertex2d(1200,350);
		glVertex2d(1200,280);
		glVertex2d(1200,280);
		glVertex2d(400,280);
		glVertex2d(400,280);
		 glVertex2d(400,320);

		 glVertex2d(350,320);
		 glVertex2d(350,200);




	glEnd();

	glDisable(GL_LINE_STIPPLE);

	glBegin(GL_LINES);
		glVertex2d(710,360);
		glVertex2d(720,370);
		glVertex2d(720,370);
		glVertex2d(710,380);

		glVertex2d(920,380);
		glVertex2d(930,390);
		glVertex2d(930,390);
		glVertex2d(920,400);

		 glVertex2d(1120,380);
		 glVertex2d(1130,390);
		  glVertex2d(1130,390);
		   glVertex2d(1120,400);

		   glVertex2d(390,310);
		   glVertex2d(400,320);
		   glVertex2d(400,320);
		   glVertex2d(410,310);

		   glVertex2d(340,210);
		   glVertex2d(350,200);
		   glVertex2d(350,200);
		   glVertex2d(360,210);

	glEnd();
	glFlush();
}

//Function to print encrypted texts at each function
void encryption_process()
{
	 glColor3f(.5,.7,1);
	glRasterPos2d(620,400);
	for(int i=0,j=0;i<l;i++){

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, pc1[i][j]);
	}

	glRasterPos2d(880,420);
	for(int i=0,j=0;i<l;i++){

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, rc1[i][j]);
	}

	glRasterPos2d(1080,420);
	for(int i=0,j=0;i<l;i++){

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, rf[i][j]);
	}

	 glRasterPos2d(850,300);
	for(int i=0,j=0;i<l;i++){

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, rc2[i][j]);
	}

	 glRasterPos2d(350,150);
	for(int i=0,j=0;i<l;i++){

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, pc2[i][j]);
	}


	glFlush();
}


//Main display function
void display() {
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	char line1[] = "Welcome to the Enigma machine simlator";
	char line2[] = "Press Right mouse button for functionalities";
	glColor3f(1, 1, 0);
	glRasterPos2d(450, 500);
	for (int i = 0; i < strlen(line1); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line1[i]);
	}
	glColor3f(0, 1, 1);
	glRasterPos2d(420, 200);
	for (int i = 0; i < strlen(line2); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line2[i]);
	}
	glFlush();
}


/**


  Input:  inner_radius - radius of hole at center
		  outer_radius - radius at center of teeth
		  width - width of gear
		  teeth - number of teeth
		  tooth_depth - depth of tooth

 **/

//Function to design the wheels
static void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat wid,
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
	glVertex3f(r0 * cos(angle), r0 * sin(angle), wid * 0.5);
	glVertex3f(r1 * cos(angle), r1 * sin(angle), wid * 0.5);
	glVertex3f(r0 * cos(angle), r0 * sin(angle), wid * 0.5);
	glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), wid * 0.5);
  }
  glEnd();

  /* draw front sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * M_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
	angle = i * 2.0 * M_PI / teeth;

	glVertex3f(r1 * cos(angle), r1 * sin(angle), wid * 0.5);
	glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), wid * 0.5);
	glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), wid * 0.5);
	glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), wid * 0.5);
  }
  glEnd();

  glNormal3f(0.0, 0.0, -1.0);

  /* draw back face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
	angle = i * 2.0 * M_PI / teeth;
	glVertex3f(r1 * cos(angle), r1 * sin(angle), -wid * 0.5);
	glVertex3f(r0 * cos(angle), r0 * sin(angle), -wid * 0.5);
	glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -wid * 0.5);
	glVertex3f(r0 * cos(angle), r0 * sin(angle), -wid * 0.5);
  }
  glEnd();

  /* draw back sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * M_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
	angle = i * 2.0 * M_PI / teeth;

	glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -wid * 0.5);
	glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -wid * 0.5);
	glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -wid * 0.5);
	glVertex3f(r1 * cos(angle), r1 * sin(angle), -wid * 0.5);
  }
  glEnd();

  /* draw outward faces of teeth */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i < teeth; i++) {
	angle = i * 2.0 * M_PI / teeth;

	glVertex3f(r1 * cos(angle), r1 * sin(angle), wid * 0.5);
	glVertex3f(r1 * cos(angle), r1 * sin(angle), -wid * 0.5);
	u = r2 * cos(angle + da) - r1 * cos(angle);
	v = r2 * sin(angle + da) - r1 * sin(angle);
	len = sqrt(u * u + v * v);
	u /= len;
	v /= len;


	glNormal3f(v, -u, 0.0);
	glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), wid * 0.5);
	glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -wid * 0.5);
	glNormal3f(cos(angle), sin(angle), 0.0);
	glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), wid * 0.5);
	glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -wid * 0.5);
	u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
	v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
	glNormal3f(v, -u, 0.0);

	glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), wid * 0.5);
	glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -wid * 0.5);
	glNormal3f(cos(angle), sin(angle), 0.0);
  }

  glVertex3f(r1 * cos(0), r1 * sin(0), wid * 0.5);
  glVertex3f(r1 * cos(0), r1 * sin(0), -wid * 0.5);

  glEnd();

  glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  glBegin(GL_QUAD_STRIP);

	angle = i * 2.0 * M_PI / teeth;

	glNormal3f(-cos(angle), -sin(angle), 0.0);
	glVertex3f(r0 * cos(angle), r0 * sin(angle), -wid * 0.5);
	glVertex3f(r0 * cos(angle), r0 * sin(angle), wid * 0.5);

  glEnd();
}

static GLfloat view_rotx = 20.0, view_roty = -50.0, view_rotz = 0.0;//inital position of angles
static GLint gear1, gear2, gear3,gear4,gear5,gear6,gear7,gear8;// wheels 
static GLfloat angle = 0.0;// angle for rotation

static GLuint limit;// used in rotation
static GLuint count = 1;//number of rotations

//Function to  position the each wheels
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

// funtion to spin the angle
static void idle()
{

  angle += 2.0;

  glutPostRedisplay();
}
//function to move the wheels
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

int m;// to store the parent window ID

// Function to  change view angle of wheels

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
static void reshape(int w, int he)
{
  GLfloat h = (GLfloat) he / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) he);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -40.0);

}

//Function to create the wheels
static void minit(void)
{

	// colors of wheels
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


  /* make the wheels */
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

//Fnction to spin the wheels
void visible(int vis)
{
  if (vis == GLUT_VISIBLE)
	glutIdleFunc(idle);

  else
	glutIdleFunc(NULL);
}

int f;//used to store the value of subwindow id

//Mouse event function to come back from sud window
void mpt(int b,int a,int x,int y)
{
	if(b==GLUT_RIGHT_BUTTON && a==GLUT_DOWN){
		glutHideWindow();

	}
}



//Function to handle menu events
void displayMenu(int n) {

	switch (n)
	{
	case 1:	displayBoard();
		encrypt();
		break;

	case 2:
			glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);


		   f= glutCreateSubWindow(m,0,0,width,height);
			minit();

			  glutDisplayFunc(draw);
			  glutReshapeFunc(reshape);
				glutMouseFunc(mpt);
			  glutKeyboardFunc(key);
			  glutSpecialFunc(special);
			  glutVisibilityFunc(visible);

			  break;
	case 3:plugBoard();
			movement();
			encryption_process();
			break;
	case 4:exit(0);
			break;



	}

}


//main function
void main(int argc,char* argv[]) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	m=glutCreateWindow("Enigma");
	myinit();
	glutDisplayFunc(display);
	glutCreateMenu(displayMenu);
	glutAddMenuEntry("Simulate", 1);
	glutAddMenuEntry("Inspect wheel", 2);




	glutAddMenuEntry("Visualize", 3);
	glutAddMenuEntry("Exit",4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
}

