#ifndef _FRONTEND_H
#define _FRONTEND_H

#include "iprimitives.h"
#include "icoordinates.h"

#include <X11/X.h>
#include <X11/Xlib.h>

typedef struct _frontend_state_t {
        icolor_t background_color;
        icolor_t drawing_color;
        char z_levels;
        float left;
        float right;
        float bottom;
        float top;

        iregion_t *regions; /* an array of regions */
        unsigned short num_regions;
} ifrontendstate_t;

void frontend_initialize();

void frontend_initialize_viewspace(float x1, float x2, float y1, float y2, int num_z_levels, ifrontendstate_t *state);

void frontend_reset_viewspace(ifrontendstate_t *state);

void frontend_free();

void frontend_update(ifrontendstate_t *state);

void frontend_draw_line(float x1, float y1, float x2, float y2, icolor_t color, int z_level);

void frontend_draw_rectangle(ipoint_t *p1, ipoint_t *p2, icolor_t color);

void frontend_draw_point(ipoint_t *point, icolor_t color);

void frontend_draw_coloredtriplet(icoloredtriplet_t *triplet);

int step(); /// Redraw frames, fetch the next event and handle it, etc.

void* (*ionkeydown[256])(void *args, void *ev);
void* (*ionkeyup[256])  (void *args, void *ev);

#endif
