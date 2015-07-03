#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#include "timer.h"

#define FPS_TIME_WINDOW	 1

#define PI  3.141592653589793

#define WIDTH   512
#define HEIGHT  512

#define XMIN  -2.
#define YMIN  -2.
#define XMAX   2.
#define YMAX   2.

/* julia variables */
float theta = 0.;
GLint *vertices;
GLfloat *colors;

inline static int genRGB(int r, int g, int b)
{
	return (r&0xff) | ((g&0xff)<<8) | ((b&0xff)<<16);
}

inline static int genColor(float color, float dif)
{
	static float k1 = 0.5, k2 = 1.9;
	color = abs(color);

	if (color <  255.f * k1) {
		return genRGB((int)(log(color/k1+1.f)/log(255.f)*255.f*2.f), 0, (int)(dif/5.f));
	} else if (color <= 255.f * k2) {
		return genRGB(0, 0, (int)(log(color/k2+1.f)/log(255.f)*255.f));
	} else {
		dif += 1000.f;
		if (dif > 2.0) dif = 2.0;
		return genRGB(0, (int)(log(color-255.f+1.f)/log(255.f)*255.f), 50+(int)(dif*205.f/2.f));
	}
}

inline static int genColorJulia(float x, float y)
{
	static int M = 1000, K = 255 * 3;
	static float r = 0.7745452294733989 /*sqrt(0.65175 * 0.65175 + 0.41850 * 0.41850)*/;
	float p = r * cos(theta), q = r * sin(theta);

	float newx, newy, dif;
	int t;

	for (t = 1; t < K; t++) {
		newx = x * x - y * y + p;
		newy = 2.f * x * y + q;
		x = newx;
		y = newy;
		dif = x * x + y * y - M;
		if (dif > 0.f)
			return genColor((float)(t)/K*2*255, dif);
	}

	return genColor(2*255, dif);
}

inline static void genColorJuliaSet(void)
{
	int i, j;
	int index, color;
	float deltax = (XMAX-XMIN)/WIDTH,
	      deltay = (YMAX-YMIN)/HEIGHT;

	#pragma omp parallel private(i, j, color, index)
	#pragma omp for
	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			color = genColorJulia(XMIN + (float)(i)*deltax, YMIN + (float)(j)*deltay);
			index = (i * WIDTH + j) * 3;
			colors[index+0] = (GLfloat)( color&0xff         )/128;
			colors[index+1] = (GLfloat)((color&0xff00  )>> 8)/128;
			colors[index+2] = (GLfloat)((color&0xff0000)>>16)/128;
		}
	}
}

void initVertCoord(void)
{
	int i, j, index;
	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			index = (WIDTH * i + j) * 2;
			vertices[index  ] = i;
			vertices[index+1] = j;
		}
	}
}

void init(void)
{
	vertices = (GLint   *) malloc(2 * WIDTH * HEIGHT * sizeof(GLint  ));
	colors   = (GLfloat *) malloc(3 * WIDTH * HEIGHT * sizeof(GLfloat));

	//printf("generating data... ");
	initVertCoord();
	genColorJuliaSet();
	//printf("done!\n\n");

	glClearColor(0.25, 0.25, 0.25, 1.0);
	glShadeModel(GL_FLAT);
}

void drawJulia(void)
{
	glVertexPointer(2, GL_INT,   0, vertices);
	glColorPointer (3, GL_FLOAT, 0, colors  );

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY );

	glDrawArrays(GL_POINTS, 0, WIDTH*HEIGHT);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY );
}

void spinDisplay(void)
{
	//printf("theta == %f, regenerating data... ", theta);
	genColorJuliaSet();
	//printf("done!\n");

	theta += PI / 180;
	if (theta > 2 * PI)
		theta -= 2 * PI;
}

void display()
{
	static double t0 /*= currentSeconds()*/;
	       double t1, elapsedTime;
	static unsigned N;
	float fps;
	char cfps[256];

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawJulia();

	/* compute FPS */
	N++;
	t1 = currentSeconds();
	elapsedTime = t1 - t0;
	if (elapsedTime > FPS_TIME_WINDOW) {
		fps = (float)N / elapsedTime;
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLfloat) w, 0.0, (GLfloat) h, 0.0, -1.0);

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) glutIdleFunc(spinDisplay);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case (27):
		free(vertices); free(colors);
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
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

