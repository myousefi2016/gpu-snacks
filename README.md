# gpu-snacks #

my casual codes on cuda acceleration and gl rendering.

# env setup #

## OpenGL/GLUT ##

OpenGL comes with X11; to verify, check the following files:

```
    /usr/lib/libGL*
    /usr/lib/x86_64-linux-gnu/libGL* (Ubuntu 14.04)
    /usr/include/GL/{gl,glu}.h
```

if GLUT is installed, it will be visible as:

```
    /usr/lib/libGL*
    /usr/lib/x86_64-linux-gnu/libglut* (Ubuntu 14.04)
    /usr/include/GL/glut.h
```

under Ubuntu 14.04, to install both (OpenGL and GLUT):

```
    sudo apt-get install freeglut3-dev
```

for RHEL/Fedora, you should try:

```
    sudo yum install freeglut-devel
```

to link with GL and GLUT, use the following options:

```
    -lGL -lGLU -lglut
```

since gcc linker may scan libraries in the order they appear on the command
line, remember to place them last, say:

```
    gcc gl-demo.c -o gl-demo -lGL -lGLU -lglut
```

## cuda ##

to be edited ...

# notes on codes #

## julia gl ##

it is kinda compute-intensive to generate a [Julia set][1], and its 'chaotic'
behavior would give you funny images; instead of images, i render those numbers
as simple no-brain graphics.

![julia-gl-1](http://7xk0df.com1.z0.glb.clouddn.com/juliaGL-1-no-brain-graphics.png)

gl renderings depend on `juliaColor` to generate color values, which is
compiled into a shared library `libjcolor.so`. so it should be prior built,
and make sure it can be located at runtime (set up `LD_LIBRARY_PATH`,
`ldconfig`, or whatever blabla). note that i did have set up an order-only
prerequisite (the `|` in GNU make) and added `$ORIGIN` as `RPATH` to all the
binaries.

juliaGL-2 takes advantage of OpenMP to burn all your CPU cores, it gives you a
direct sense of parallel multi-core acceleration.

![julia-gl-2-omp](http://7xk0df.com1.z0.glb.clouddn.com/juliaGL-2-burn-cpu.png)

[1]: https://en.wikipedia.org/wiki/Julia_set    "Julia set"

