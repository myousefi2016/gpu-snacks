#include <stdio.h>

#include <GL/glut.h>

#include "juliaColor.h"
#include "timer.h"

#define FPS_TIME_WINDOW 1

#define PI 3.141592653589793

#define WIDTH       1280
#define HEIGHT       640
#define JULIA_WIDTH  256
#define JULIA_HEIGHT 256

#define XMIN -2.
#define YMIN -2.
#define XMAX  2.
#define YMAX  2.

#define EDGE  320
#define STRIDE 32

GLfloat angle = 0.0;

GLint *xzPlane;
GLint *xyVertices, *yzVertices;
GLfloat *colors;

static float theta;
static float deltax, deltay;

void init(void)
{
	int i, j, n;
	int index, color;

	deltax = (XMAX - XMIN) / JULIA_WIDTH,
	deltay = (YMAX - YMIN) / JULIA_HEIGHT;

	xzPlane    = (GLint   *) malloc(2 * 2 * (EDGE/STRIDE + 1) * 3  * sizeof(GLint));
	xyVertices = (GLint   *) malloc(3 * JULIA_WIDTH * JULIA_HEIGHT * sizeof(GLint));
	yzVertices = (GLint   *) malloc(3 * JULIA_WIDTH * JULIA_HEIGHT * sizeof(GLint));
	colors     = (GLfloat *) malloc(3 * JULIA_WIDTH * JULIA_HEIGHT * sizeof(GLfloat));

	/* initial X-Z plane array */
	n = EDGE / (STRIDE * 2);
	for (i = -n, index = 0; i <= n; i++, index += 6) {
		j = i * STRIDE;
		xzPlane[index  ] = j;
		xzPlane[index+1] = 0;
		xzPlane[index+2] = 160;
		xzPlane[index+3] = j;
		xzPlane[index+4] = 0;
		xzPlane[index+5] = -160;
	}
	for (i = -n; i <= n; i++, index += 6) {
		j = i * STRIDE;
		xzPlane[index  ] = 160;
		xzPlane[index+1] = 0;
		xzPlane[index+2] = j;
		xzPlane[index+3] = -160;
		xzPlane[index+4] = 0;
		xzPlane[index+5] = j;
	}

	/* initial julia vertex array */
	for (i = 0, index = 0; i < JULIA_WIDTH; i++) {
		for (j = 0; j < JULIA_HEIGHT; j++, index += 3) {
			xyVertices[index  ] = i-128;
			xyVertices[index+1] = j-128;
			xyVertices[index+2] = 128;
			yzVertices[index  ] = 128;
			yzVertices[index+1] = j-128;
			yzVertices[index+2] = i-128;
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

void drawPlane(void)
{
	glColor3f(1.0, 1.0, 1.0);
	glVertexPointer(3, GL_INT, 0, xzPlane);

	glEnableClientState (GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINES, 0, 4*(EDGE/STRIDE+1));
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawJulia(void)
{
	glVertexPointer(3, GL_INT,   0, xyVertices);
	glColorPointer (3, GL_FLOAT, 0, colors);

	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY );
	glDrawArrays(GL_POINTS, 0, JULIA_WIDTH*JULIA_HEIGHT);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY );

	glVertexPointer(3, GL_INT,   0, yzVertices);
	glColorPointer (3, GL_FLOAT, 0, colors);

	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY );
	glDrawArrays(GL_POINTS, 0, JULIA_WIDTH*JULIA_HEIGHT);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY );
}

void spinJulia(void)
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

	angle += 0.5;
}

void display(void)
{
	static unsigned N;
	static double t0;
	double t1, elapsedTime;
	float fps;
	char cfps[256];

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	/* left viewport */
	glViewport(0, 0, WIDTH/2, HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1.0, 16.0, 512.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(160.0, 64.0, 160.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(angle, 0.0, 1.0, 0.0);

	drawPlane();
	drawJulia();

	glPopMatrix();

	/* right viewport */
	glViewport(WIDTH/2, 0, WIDTH/2, HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-256.0, 256.0, -256.0, 256.0, 0.0, 512.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(160.0, 64.0, 160.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(angle, 0.0, 1.0, 0.0);

	drawPlane();
	drawJulia();

	glPopMatrix();

	/* calculate and show FPS */
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
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case (27):
		free(xzPlane);
		free(xyVertices); free(yzVertices);
		free(colors);
		exit(0);
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) glutIdleFunc(spinJulia);
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
	glutCreateWindow("Julia Fractal GL");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();

	return 0;
}

