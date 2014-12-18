#ifndef _FRONTEND_OPENGL_LINUX_H
#define _FRONTEND_OPENGL_LINUX_H 1

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "frontend.h"
#include "icoordinates.h"

Display                 *dpy;
Window                  root;
GLint                   att[5];
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;

void DrawAQuad();

#endif
