#include <stdio.h>

#include <GL/glut.h>

#include "julia3D.h"
#include "timer.h"

GLfloat eye_x = 512.;
GLfloat eye_y =  64.;
GLfloat eye_z = 512.;

GLfloat rotate_x_angle = 0.;
GLfloat rotate_y_angle = 0.;
GLfloat rotate_z_angle = 0.;

enum DrawMode dm = DrawXOY;

void init(void)
{
	initVBO();
	initJuliaColors();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.25, 0.25, 0.25, 1.0);
	glShadeModel(GL_FLAT);
}

void display(void)
{
	static unsigned n;
	static double t0;
	double t1, elapsedTime;
	float fps;
	char cfps[256];

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(rotate_y_angle, 0.0, 1.0, 0.0);
	glRotatef(rotate_x_angle, 1.0, 0.0, 0.0);
	glRotatef(rotate_z_angle, 0.0, 0.0, 1.0);

	drawSpace();
	drawJulia(dm);

	n++;
	t1 = currentSeconds();
	elapsedTime = t1 - t0;
	if (elapsedTime > FPS_TIME_WINDOW) {
		fps = (float) n / elapsedTime;
		n = 0;
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1.0, 16.0, 2048.0);

	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case (27):
		cleanJulia();
		exit(0);
	case ('w'):
		if (eye_z >= 256.) {
			eye_x -= 1.0;
			eye_z -= 1.0;
		}
		break;
	case ('x'):
		if (eye_z <= 512.) {
			eye_x += 1.0;
			eye_z += 1.0;
		}
		break;
	case ('a'):
		rotate_y_angle += 0.5;
		if (rotate_y_angle > 360.) {
			rotate_y_angle -= 360.0;
		}
		break;
	case ('d'):
		rotate_y_angle -= 0.5;
		if (rotate_y_angle < 0.) {
			rotate_y_angle += 360.0;
		}
		break;
	case ('q'):
		rotate_x_angle += 0.5;
		if (rotate_x_angle > 360.) {
			rotate_x_angle -= 360.0;
		}
		break;
	case ('e'):
		rotate_x_angle -= 0.5;
		if (rotate_x_angle < 0.) {
			rotate_x_angle += 360.;
		}
		break;
	case ('z'):
		rotate_z_angle += 0.5;
		if (rotate_z_angle > 360.) {
			rotate_z_angle -= 360.;
		}
		break;
	case ('c'):
		rotate_z_angle -= 0.5;
		if (rotate_z_angle < 0.0) {
			rotate_z_angle += 360.0;
		}
		break;
	case ('s'):
		resetCount();
		dm = DrawAll;
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			resetCount();
			dm = DrawXOY;

			glutPostRedisplay();
			glutSwapBuffers();
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			resetCount();
			dm = DrawXOZ;

			glutPostRedisplay();
			glutSwapBuffers();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			resetCount();
			dm = DrawYOZ;

			glutPostRedisplay ( );
			glutSwapBuffers ( );
		}
		break;
	default:
		break;
	}
}

//void idle(void) { }

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Preparing data ...");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	//glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

