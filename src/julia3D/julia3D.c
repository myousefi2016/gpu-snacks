#include <stdio.h>
#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "juliaColor.h"
#include "julia3D.h"

static GLint *vertices;
static GLfloat *colors;
static GLuint vbo;
static int count;

void initVBO(void)
{
	int i, index, msize;
	int nx, ny, nz;
	int width2, height2, depth2;

	msize  = (JULIA_WIDTH * JULIA_HEIGHT * JULIA_DEPTH)
	         * 3 * sizeof(GLfloat);
	colors = (GLfloat *) malloc(msize);

	msize    = ((SPACE_WIDTH/STRIDE+1) + (SPACE_HEIGHT/STRIDE+1) + (SPACE_DEPTH/STRIDE+1))
	           * 3 * 3 * sizeof(GLint);
	vertices = (GLint *) malloc(msize);

	nx = SPACE_WIDTH  / STRIDE / 2;
	ny = SPACE_HEIGHT / STRIDE / 2;
	nz = SPACE_DEPTH  / STRIDE / 2;
	width2  = SPACE_WIDTH  / 2;
	height2 = SPACE_HEIGHT / 2;
	depth2  = SPACE_DEPTH  / 2;

	index = 0;
	for (i = -nx; i <= nx; i++) {
		vertices[index++] = i * STRIDE;
		vertices[index++] = height2;
		vertices[index++] = -depth2;
		vertices[index++] = i * STRIDE;
		vertices[index++] = -height2;
		vertices[index++] = -depth2;
		vertices[index++] = i * STRIDE;
		vertices[index++] = -height2;
		vertices[index++] = depth2;
	}
	for (i = -ny; i <= ny; i++) {
		vertices[index++] = width2;
		vertices[index++] = i * STRIDE;
		vertices[index++] = -depth2;
		vertices[index++] = -width2;
		vertices[index++] = i * STRIDE;
		vertices[index++] = -depth2;
		vertices[index++] = -width2;
		vertices[index++] = i * STRIDE;
		vertices[index++] = depth2;
	}
	for (i = -nz; i <= nz; i++) {
		vertices[index++] = width2;
		vertices[index++] = -height2;
		vertices[index++] = i * STRIDE;
		vertices[index++] = -width2;
		vertices[index++] = -height2;
		vertices[index++] = i * STRIDE;
		vertices[index++] = -width2;
		vertices[index++] = height2;
		vertices[index++] = i * STRIDE;
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, msize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	free(vertices);
}

void initJuliaColors(void)
{
	int i, j, k;
	int index, color;
	float theta, deltax, deltay;

	theta = PI / 3;
	deltax = (XMAX - XMIN) / JULIA_WIDTH ;
	deltay = (YMAX - YMIN) / JULIA_HEIGHT;
	index = 0;
	for (k = 0; k < JULIA_DEPTH; k++) {
		for (j = 0; j < JULIA_HEIGHT; j++) {
			for (i = 0; i < JULIA_WIDTH; i++) {
				color = genColorJulia(XMIN + (float)(i) * deltax,
				                      YMIN + (float)(j) * deltay,
				                      theta);
				colors[index++] = (GLfloat)( color&0xff         )/128;
				colors[index++] = (GLfloat)((color&0xff00  )>> 8)/128;
				colors[index++] = (GLfloat)((color&0xff0000)>>16)/128;
			}
		}
		theta += PI / 180;
	}
}

void drawSpace(void)
{
	int i, n, index;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_INT, 0, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(1.0, 1.0, 1.0);

	index = 0;
	n = (SPACE_WIDTH/STRIDE+1) + (SPACE_HEIGHT/STRIDE+1) + (SPACE_DEPTH/STRIDE+1);
	for (i = 0; i < n; i++) {
		glBegin(GL_LINE_STRIP);
		    glArrayElement(index++);
		    glArrayElement(index++);
		    glArrayElement(index++);
		glEnd();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawJulia(enum DrawMode dm)
{
	int i, j, k;
	int index, stride, offset, msize;

	glEnableClientState ( GL_VERTEX_ARRAY );
	glEnableClientState ( GL_COLOR_ARRAY  );

	switch (dm)
	{
	case DrawAll:
		msize = JULIA_WIDTH * JULIA_HEIGHT * 3 * sizeof(GLint);
		vertices = (GLint *) malloc(msize);
		for (k = 0; k < JULIA_DEPTH; k++) {
			index = 0;
			for (j = 0; j < JULIA_HEIGHT; j++) {
				for (i = 0; i < JULIA_WIDTH; i++) {
					vertices[index++] = i - JULIA_WIDTH /2;
					vertices[index++] = j - JULIA_HEIGHT/2;
					vertices[index++] = k - JULIA_DEPTH /2;
				}
			}
			offset = JULIA_WIDTH * JULIA_HEIGHT * k * 3;
			stride = 0 * sizeof(GLfloat);
			glVertexPointer(3, GL_INT, 0, vertices);
			glColorPointer(3, GL_FLOAT, stride, colors + offset);
			glDrawArrays(GL_POINTS, 0, JULIA_WIDTH * JULIA_HEIGHT);
		}
		break;

	case DrawXOY:
		msize = JULIA_WIDTH * JULIA_HEIGHT * 3 * sizeof(GLint);
		vertices = (GLint *) malloc(msize);
		index = 0;
		k = count;
		for (j = 0; j < JULIA_HEIGHT; j++) {
			for (i = 0; i < JULIA_WIDTH; i++) {
				vertices[index++] = i - JULIA_WIDTH /2;
				vertices[index++] = j - JULIA_HEIGHT/2;
				vertices[index++] = k - JULIA_DEPTH /2;
			}
		}
		offset = JULIA_WIDTH * JULIA_HEIGHT * k * 3;
		stride = 0 * sizeof(GLfloat);
		glVertexPointer(3, GL_INT, 0, vertices);
		glColorPointer(3, GL_FLOAT, stride, colors + offset);
		glDrawArrays(GL_POINTS, 0, JULIA_WIDTH * JULIA_HEIGHT);
		count++;
		if (count == JULIA_DEPTH)
			count -= JULIA_DEPTH;
		break;

	case DrawXOZ:
		msize = JULIA_WIDTH * 3 * sizeof(GLint);
		vertices = (GLint *) malloc(msize);
		j = count;
		for (k = 0; k < JULIA_DEPTH; k++) {
			index = 0;
			for (i = 0; i < JULIA_WIDTH; i++) {
				vertices[index++] = i - JULIA_WIDTH /2;
				vertices[index++] = j - JULIA_HEIGHT/2;
				vertices[index++] = k - JULIA_DEPTH /2;
			}
			offset = (JULIA_WIDTH * j + JULIA_WIDTH * JULIA_HEIGHT * k) * 3;
			stride = 0 * sizeof(GLfloat);
			glVertexPointer(3, GL_INT, 0, vertices);
			glColorPointer(3, GL_FLOAT, 0, colors + offset);
			glDrawArrays(GL_POINTS, 0, JULIA_WIDTH);
		}
		count++;
		if (count == JULIA_HEIGHT)
			count -= JULIA_HEIGHT;
		break;

	case DrawYOZ:
		msize = JULIA_HEIGHT * JULIA_DEPTH * 3 * sizeof(GLint);
		vertices = (GLint *) malloc(msize);
		index = 0;
		i = count;
		for (k = 0; k < JULIA_DEPTH; k++) {
			for (j = 0; j < JULIA_HEIGHT; j++) {
				vertices[index++] = i - JULIA_WIDTH /2;
				vertices[index++] = j - JULIA_HEIGHT/2;
				vertices[index++] = k - JULIA_DEPTH /2;
			}
		}
		offset = i * 3;
		stride = JULIA_WIDTH * 3 * sizeof(GLfloat);
		glVertexPointer(3, GL_INT, 0, vertices);
		glColorPointer(3, GL_FLOAT, stride, colors + offset);
		glDrawArrays(GL_POINTS, 0, JULIA_HEIGHT * JULIA_DEPTH);
		count++;
		if (count == JULIA_WIDTH)
			count -= JULIA_DEPTH;
		break;

	default:
		break;
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	free(vertices);
}

void resetCount(void)
{
	count = 0;
}

void cleanJulia(void)
{
	glBindBuffer(1, vbo);
	glDeleteBuffers(1, &vbo);
	vbo = 0;

	free(colors);
}

