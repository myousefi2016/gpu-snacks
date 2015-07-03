# gpu-snacks #

my casual codes on cuda acceleration and gl rendering.

# GLUT setup #

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

