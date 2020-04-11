#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<GL/glut.h>
#define FORWARD 1
#define BACKWARD 2

char r1[27][2], r2[27][2], r3[27][2], ref[14][2], plug[27][2]; // rotor and plug variables
char map1[] = "QWERTYUIOPASDFGHJKLZXCVBNM"; // mapping for rotor 1
char map2[] = "MLPOKIJNBHUYGVCFTRDXSEWQAZ"; //mapping for rotor 2
char map3[] = "LKJHGFDSAMPOIUNBYTVCREXZWQ"; // mapping for rotor 3
char plugmap[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //plugboard mapping
char refmap[] = "NOPQRSTUVWXYZ"; // reflector mapping

int index1, index2, index3; // index variables for rotors

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
	gluOrtho2D(0, 1000, 0, 1000);
	glClearColor(0, 0, 0, 0);
}

void encrypt() {
	char text[200];
	init();
	printf("Enter string to be encoded\n");
	gets_s(text, 50);
	int len = strlen(text);
	char ch;
	for (int i = 0; i < len; i++) {
		ch = text[i];
		ch = toupper(ch);
		ch = passThroughPlugBoard(ch, FORWARD);
		ch = passThroughRotors(ch, FORWARD);
		ch = passThroughReflectors(ch);
		ch = passThroughRotors(ch, BACKWARD);
		ch = passThroughPlugBoard(ch, FORWARD);
		printf("%c", ch);
		rotateRotors();
	}
	getch();
}

void displayMenu(int n) {
	switch (n)
	{
	case 1: encrypt();
		break;
	}
}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void main() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Enigma");
	myinit();
	glutDisplayFunc(display);
	glutCreateMenu(displayMenu);
	glutAddMenuEntry("Simulate", 1);
	glutAddMenuEntry("Inspect wheel", 2);
	glutAddMenuEntry("Visualize", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
}