#include <stdlib.h>
#include <math.h>

#include "juliaColor.h"

int genRGB(int r, int g, int b)
{
	return (r&0xff) | ((g&0xff)<<8) | ((b&0xff)<<16);
}

int genColor(float color, float dif)
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

int genColorJulia(float x, float y, float theta)
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
		//dif = newx * newx + newy * newy - M;
		if (dif > 0.f)
			return genColor((float)(t)/K*2*255, dif);
	}

	return genColor(2*255, dif);
}

