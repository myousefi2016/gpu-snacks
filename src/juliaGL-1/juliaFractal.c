#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>

#include "timer.h"
#include "juliaColor.h"

#define FPS_TIME_WINDOW	 1

#define PI  3.141592653589793

#define WIDTH   512
#define HEIGHT  512

#define JULIA_WIDTH   256
#define JULIA_HEIGHT  256

/* eye */
GLfloat eye_z = 512.0;

/* translation */
GLfloat translate_z = 0.0;

/* rotation */
GLfloat rotate_x = 0.0, rotate_y = 0.0;

/* mouse Controls */
int mouse_old_x, mouse_old_y;
int mouse_buttons = 0;

/* julia Variables */
float theta = 0.0;

float xmin = -2.0,
      ymin = -2.0,
      xmax =  2.0,
      ymax =  2.0;

void init(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.25, 0.25, 0.25, 1.0);
	glShadeModel(GL_SMOOTH);
}

void display(void)
{
	int i, j;
	int m, n;
	int color;
	float deltax = (xmax-xmin)/JULIA_WIDTH,
	      deltay = (ymax-ymin)/JULIA_HEIGHT;

	static double t0 /*= currentSeconds()*/;
	       double t1, elapsedTime;
	static float fps;
	static unsigned N = 0;
	char cfps[256];

	t0 = currentSeconds();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glTranslatef(0.0, 0.0, translate_z);
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);

	/* draw the x-z plane */
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	for (i = 0, n = JULIA_WIDTH/2; i <= JULIA_WIDTH; i += 32) {
		glVertex3i(i-n, 0, 0);
		glVertex3i(i-n, 0, -512);
	}
	for (j = 0; j <= 512; j += 32)
	{
		glVertex3i(-n, 0, -j);
		glVertex3i( n, 0, -j);
	}
	glEnd();

	/* draw Julia */
	glBegin(GL_POINTS);
	n = JULIA_WIDTH/2, m = JULIA_HEIGHT/2;
	for (i = 0; i < JULIA_WIDTH; ++i) {
		for (j = 0; j < JULIA_HEIGHT; ++j) {
			color = genColorJulia(xmin + (float)(i) * deltax,
			                      ymin + (float)(j) * deltay,
			                      theta);
			glColor3f((float)( color&0xff    )     /128,
			          (float)((color&0xff00  )>>8) /128,
			          (float)((color&0xff0000)>>16)/128);
			glVertex3i(i-n, j-m, 0);

			if (j >= 0 && j < 64) glVertex3i(i-n, j-m, -128);
			else if (j >= 64 && j < 128) glVertex3i(i-n, j-m, -256);
			else if (j >= 128 && j < 192) glVertex3i(i-n, j-m, -384);
			else glVertex3i(i-n, j-m, -512);
		}
	}
	glEnd();

	/* compute FPS */
	N++;
	t1 = currentSeconds();
	elapsedTime = t1 - t0;
	if (elapsedTime > FPS_TIME_WINDOW) {
		fps = (float) N / elapsedTime;
		N = 0;
		t0 = t1;
		sprintf(cfps, "Julia Fractal (%d x %d): %3.1f fps", WIDTH, HEIGHT, fps);
		printf("%s\n", cfps);
		glutSetWindowTitle(cfps);
	}
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-512.0, 512.0, -512.0, 512.0, -1024.0, 1024.0);
	gluPerspective(60.0, 1.0, 1.0, 1024.0);

	glutPostRedisplay();
}

void mouse (int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
		mouse_buttons |= 1<<button;
	else if (state == GLUT_UP)
		mouse_buttons = 0;

	mouse_old_x = x, mouse_old_y = y;

	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case (27): exit(0);
	case ('w'):
		if (eye_z >= 256.0) eye_z -= 1.0;
		break;
	case ('s'):
		if (eye_z <= 512.0) eye_z += 1.0;
		break;
	}
}

void motion (int x, int y)
{
	float dx, dy;

	dx = x - mouse_old_x;
	dy = y - mouse_old_y;

	if (mouse_buttons & 1) {
		rotate_x += dy * 0.2;
		rotate_y += dx * 0.2;
	} else if (mouse_buttons & 4) {
		translate_z += dy * 0.01;
	}

	mouse_old_x = x;
	mouse_old_y = y;
}

void spinDisplay(void)
{
	theta += PI / 180;
	if (theta > 2*PI)
		theta -= 2*PI;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(128, 128);
	glutCreateWindow("Julia Fractal GL");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutIdleFunc(spinDisplay);

	glutMainLoop ();

	return 0;
}

