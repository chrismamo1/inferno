#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "frontend.h"
#include "icoordinates.h"
#include "frontend_opengl_linux.h"
#include "iprimitives.h"
#include "icoordinates.h"


void DrawAQuad() {
        glBegin(GL_QUADS);
        /*glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
        glColor3f(0., 1., 0.); glVertex3f( 75, -.75, 0.);
        glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
        glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);*/
        glColor3f(0., 0., 0.);
        glVertex2f(0.0, 0.); glVertex2f(1., 0.); glVertex2f(1., 1.); glVertex2f(0., 1.);
        glEnd();
} 

void frontend_initialize()
{
        //att = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        att[0] = GLX_RGBA;
        att[1] = GLX_DEPTH_SIZE;
        att[2] = 24;
        att[3] = GLX_DOUBLEBUFFER;
        att[4] = None;
        dpy = XOpenDisplay(NULL);
        
        if(dpy == NULL) {
                printf("\n\tcannot connect to X server\n\n");
                exit(0);
        }

        root = DefaultRootWindow(dpy);

        vi = glXChooseVisual(dpy, 0, att);

        if(vi == NULL) {
                printf("\n\tno appropriate visual found\n\n");
                exit(0);
        } 
        else {
                printf("\n\tvisual %p selected\n", (void *)vi->visualid);
        }

        cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

        swa.colormap = cmap;
        swa.event_mask = ExposureMask | KeyPressMask;

        win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0,
                                vi->depth, InputOutput, vi->visual,
                                CWColormap | CWEventMask, &swa);

        XMapWindow(dpy, win);

        XStoreName(dpy, win, "VERY SIMPLE APPLICATION");

        glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
        glXMakeCurrent(dpy, win, glc);

        glDisable(GL_DEPTH_TEST);
}

/**
 * @return a bitmap with the 0 byte being the number of "near" z-levels and the 1 byte being the number of "far" z-levels
 */
void frontend_initialize_viewspace(float x1, float x2, float y1, float y2, int num_z_levels, struct ifrontendstate_t *state)
{
        state->background_color = malloc(sizeof(struct icolor_t));
        state->drawing_color = malloc(sizeof(struct icolor_t));
        state->left = x1;
        state->right = x2;
        state->bottom = y1;
        state->top = y2;
        state->z_levels = num_z_levels;
        state->num_regions = 0;
        state->regions = NULL;
        icreate_region(0, 0, 100, 100, state);
        int i;
        for (i = 0; i < 256; i++) {
                state->onkeyup_handlers[i]   = inew_eventhandler(NULL, NULL, i);
                state->onkeydown_handlers[i] = inew_eventhandler(NULL, NULL, i);
        }
}

void frontend_reset_viewspace(struct ifrontendstate_t *state)
{
        glClearColor((float)red(*state->background_color)/256.,
                     (float)green(*state->background_color)/256.,
                     (float)blue(*state->background_color)/256., 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(state->left, state->right, state->bottom, state->top, state->z_levels, 0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
}


void frontend_free()
{
        printf("Freeing frontend resources...\n");
        glXMakeCurrent(dpy, None, NULL);
        glXDestroyContext(dpy, glc);
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
}

void frontend_update(struct ifrontendstate_t *state)
{
        XGetWindowAttributes(dpy, win, &gwa);
        glViewport(0, 0, gwa.width, gwa.height);
        //DrawAQuad(); 
        glXSwapBuffers(dpy, win);
}

void frontend_draw_rectangle(struct ipoint_t *p1, struct ipoint_t *p2, struct icolor_t color)
{
        float z = (float) p1->z;
        glBegin(GL_QUADS);
                glColor3f(color.r, color.g, color.b);
                glVertex3f(p1->x, p1->y, z);
                glVertex3f(p2->x, p1->y, z);
                glVertex3f(p2->x, p2->y, z);
                glVertex3f(p1->x, p2->y, z);
        glEnd();
}

void frontend_draw_point(struct ipoint_t *point, struct icolor_t color)
{
        glBegin(GL_POINTS);
                glColor3f((float)color.r, (float)color.g, (float)color.b);
                glVertex3f(point->x, point->y, (float) point->z);
        glEnd();
}

void frontend_draw_coloredtriplet(struct icoloredtriplet_t *triplet)
{
        glBegin(GL_TRIANGLES);
                glColor3ub((triplet->colors[0]->r),
                          (triplet->colors[0]->g),
                          (triplet->colors[0]->b));
                glVertex3f(triplet->vertices[0]->x, triplet->vertices[0]->y,
                           (float)triplet->vertices[0]->z);
                glColor3ub(triplet->colors[1]->r,
                          triplet->colors[1]->g,
                          triplet->colors[1]->b);
                glVertex3f(triplet->vertices[1]->x, triplet->vertices[1]->y,
                           (float)triplet->vertices[1]->z);
                glColor3ub(triplet->colors[2]->r,
                          triplet->colors[2]->g,
                          triplet->colors[2]->b);
                glVertex3f(triplet->vertices[2]->x, triplet->vertices[2]->y,
                           (float)triplet->vertices[2]->z);
        glEnd();
}

int step(struct ifrontendstate_t *state)
{
        XNextEvent(dpy, &xev);
        if (xev.type == Expose)
                frontend_update(state);
        else if(xev.type == KeyPress) {
                struct ievent_handler *h = state->onkeydown_handlers[xev.xkey.keycode];
                *(h->handler)(h->args, (void*)&(xev.xkey));
        }
        else if (xev.type == KeyRelease) { 
                struct ievent_handler *h = state->onkeyup_handlers[xev.xkey.keycode];
                *(h->handler)(h->args, (void*)&(xev.xkey));
        }
        else if (xev.type == ClientMessage) {
                frontend_free();
                return 1;
        }
        frontend_update(state);
        frontend_reset_viewspace(state);
        return 0;
}
