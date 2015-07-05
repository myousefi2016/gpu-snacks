#include <stdio.h>

#include <GL/glut.h>

#include "juliaColor.h"
#include "timer.h"

#define FPS_TIME_WINDOW 1

#define PI 3.141592653589793

#define WIDTH        640
#define HEIGHT       640
#define JULIA_WIDTH  256
#define JULIA_HEIGHT 256

#define XMIN -2.
#define YMIN -2.
#define XMAX  2.
#define YMAX  2.

/* procedure flag */
int flag = 0;

/* time control */
double tx, ty;

/* ortho */
GLfloat ortho_left   = -512.;
GLfloat ortho_bottom = -512.;
GLfloat ortho_znear  =    0.;
GLfloat ortho_zfar   = 1024.;

/* eye */
GLfloat eye_x =   0.;
GLfloat eye_y = 128.;
GLfloat eye_z = 256.;

/* translation */
GLfloat translate_x = 0.;
GLfloat translate_y = 0.;
GLfloat translate_z = 0.;

/* rotation */
GLfloat angle    = 0.;
GLfloat rotate_x = 0.;
GLfloat rotate_y = 1.;
GLfloat rotate_z = 0.;

/* julia arrays and variables */
GLint *vertices;
GLfloat *colors;

float theta;
float deltax, deltay;

void init(void)
{
	int i, j;
	int index, color;

	theta = 0.;
	deltax = (XMAX - XMIN) / JULIA_WIDTH,
	deltay = (YMAX - YMIN) / JULIA_HEIGHT;

	vertices = (GLint   *) malloc(2 * JULIA_WIDTH * JULIA_HEIGHT * sizeof(GLint  ));
	colors   = (GLfloat *) malloc(3 * JULIA_WIDTH * JULIA_HEIGHT * sizeof(GLfloat));

	/* initial julia vertex array */
	for (i = 0, index = 0; i < JULIA_WIDTH; i++) {
		for (j = 0; j < JULIA_HEIGHT; j++, index += 2) {
			vertices[index  ] = i - JULIA_WIDTH /2;
			vertices[index+1] = j - JULIA_HEIGHT/2;
		}
	}

	/* initial julia color array */
	//#pragma omp parallel private(i, j, color, index)
	//#pragma omp for
	for (i = 0; i < JULIA_WIDTH; i++) {
		for (j = 0; j < JULIA_HEIGHT; j++) {
			color = genColorJulia(XMIN + (float)(i) * deltax,
			                      YMIN + (float)(j) * deltay,
			                      theta);
			index = (i * JULIA_WIDTH + j) * 3;
			colors[index+0] = (GLfloat)( color&0xff         )/128;
			colors[index+1] = (GLfloat)((color&0xff00  )>> 8)/128;
			colors[index+2] = (GLfloat)((color&0xff0000)>>16)/128;
		}
	}

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.25, 0.25, 0.25, 1.0);
	glShadeModel(GL_FLAT);
}

void drawAxes(void)
{
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_LINES);
	    glVertex3i(-256, 0, 0);
	    glVertex3i( 256, 0, 0);

	    glVertex3i(0, -256, 0);
	    glVertex3i(0,  256, 0);

	    glVertex3i(0, 0, -128);
	    glVertex3i(0, 0,  128);
	glEnd();
}

void drawJulia(void)
{
	glVertexPointer(2, GL_INT,   0, vertices);
	glColorPointer (3, GL_FLOAT, 0, colors  );

	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY );
	glDrawArrays(GL_POINTS, 0, JULIA_WIDTH*JULIA_HEIGHT);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY );
}

void idle(void)
{
	int i, j;
	int index, color;

	//#pragma omp parallel private(i, j, color, index)
	//#pragma omp for
	for (i = 0; i < JULIA_WIDTH; i++) {
		for (j = 0; j < JULIA_HEIGHT; j++) {
			color = genColorJulia(XMIN + (float)(i) * deltax,
			                      YMIN + (float)(j) * deltay,
			                      theta);
			index = (i * JULIA_WIDTH + j) * 3;
			colors[index+0] = (GLfloat)( color&0xff         )/128;
			colors[index+1] = (GLfloat)((color&0xff00  )>> 8)/128;
			colors[index+2] = (GLfloat)((color&0xff0000)>>16)/128;
		}
	}

	theta += PI / 180;
	if (theta > 2 * PI)
		theta -= 2 * PI;

	if (flag == 0) {
		angle += 0.5;
		if (angle == 330.0) {
			flag = 1;
			tx = currentMS();
		}
	}

	if (flag == 1) {
		ty = currentMS();
		if (ty - tx > 2000) {
			translate_x += 0.5;
			translate_y += 0.5;
		}
		if (translate_x == 64.0) {
			flag = 2;
			tx = currentMS();
		}
	}

	if (flag == 2) {
		ty = currentMS();
		if (ty - tx > 2000) {
			ortho_left += 0.5;
			ortho_bottom += 0.5;
		}

		if (ortho_left == -128.0) {
			flag = 3;
			tx = currentMS();
		}
	}

	if (flag == 3) {
		ty = currentMS();
		if (ty - tx > 2000) {
			translate_x -= 0.5;
			translate_y -= 0.5;
		}

		if (translate_x == 0.0) {
			flag = 4;
			tx = currentMS();
		}
	}

	if (flag == 4) {
		ty = currentMS();
		if (ty - tx > 2000) {
			ortho_left += 0.5;
			ortho_bottom += 0.5;
		}

		if (ortho_left == -128.0) {
			flag = 5;
			tx = currentMS();
		}
	}

	if (flag == 5) {
		ty = currentMS();
		if (ty - tx > 2000) {
			eye_y -= 0.5;
		}

		if (eye_y == 0.0) {
			flag = 6;
			tx = currentMS();
		}
	}

	if (flag == 6) {
		ty = currentMS();
		if (ty - tx > 2000) {
			angle += 0.5;
		}

		if (angle == 360.0) {
			flag = 7;
			tx = currentMS();
		}
	}

	if (flag == 7) {
		ty = currentMS();
		if (ty - tx > 3000) {
			angle += 0.5;
			if (angle > 360.0) {
				angle -= 360.0;
			}
		}
	}
}

void display(void)
{
	static double t0 /*= currentSeconds()*/;
	       double t1, elapsedTime;
	static unsigned N;
	float fps;
	char cfps[256];

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(ortho_left,   -ortho_left,
		ortho_bottom, -ortho_bottom,
		ortho_znear,   ortho_zfar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(angle, rotate_x, rotate_y, rotate_z);

	drawAxes();

	glPopMatrix();

	glTranslatef(translate_x, translate_y, translate_z);
	glPushMatrix();
	glRotatef(angle, rotate_x, rotate_y, rotate_z);

	drawJulia();

	glPopMatrix();

	/* compute FPS */
	N++;
	t1 = currentSeconds();
	elapsedTime = t1 - t0;
	if (elapsedTime > FPS_TIME_WINDOW) {
		fps = (float) N / elapsedTime;
		N = 0;
		t0 = t1;
		sprintf(cfps, "Julia Fractal GL (%d x %d): %3.1f fps", WIDTH, HEIGHT, fps);
		printf("%s\n", cfps);
		glutSetWindowTitle(cfps);
	}
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case (27):
		free(vertices); free(colors);
		exit (0);
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) glutIdleFunc(idle);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Initializing ...");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	//glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

