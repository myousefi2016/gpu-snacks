#ifndef _JULIA_3D_H_
#	define _JULIA_3D_H_

#define FPS_TIME_WINDOW 1

#define PI 3.141592653589793

#define WIDTH  640
#define HEIGHT 640

#define JULIA_WIDTH  256
#define JULIA_HEIGHT 256
#define JULIA_DEPTH  240

#define SPACE_WIDTH  256
#define SPACE_HEIGHT 256
#define SPACE_DEPTH  320

#define STRIDE 32

#define XMIN -2.
#define YMIN -2.
#define XMAX  2.
#define YMAX  2.

enum DrawMode { DrawAll, DrawXOY, DrawXOZ, DrawYOZ };

void initVBO(void);
void initJuliaColors(void);

void drawJulia(enum DrawMode dm);
void drawSpace(void);

void resetCount(void);
void cleanJulia(void);

#endif  /* _JULIA_3D_H_ */

