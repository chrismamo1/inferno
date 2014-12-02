#ifndef _FRONTEND_H
#define _FRONTEND_H

#include "iprimitives.h"
#include "icoordinates.h"

#include <X11/X.h>
#include <X11/Xlib.h>

struct _ievent_handler {
        void* (*handler)(void *args, void *event);
        void *args;
        unsigned short key_code;
};

typedef struct _ievent_handler ievent_handler;

typedef struct _frontend_state_t {
        icolor_t background_color;
        icolor_t drawing_color;
        char z_levels;
        float left;
        float right;
        float bottom;
        float top;

        ievent_handler *onkeydown_handlers[256];
        ievent_handler *onkeyup_handlers[256];

        iregion_t *regions; /* a dynamic array of pointers to regions */
        unsigned short num_regions;
} ifrontendstate_t;

ievent_handler* inew_eventhandler(void* (*handler)(void *args, void *event), void *args, unsigned short key_code);

void frontend_initialize();

void frontend_set_eventhandler(unsigned short i, char down, ievent_handler *h, ifrontendstate_t *state);

void frontend_initialize_viewspace(float x1, float x2, float y1, float y2, int num_z_levels, ifrontendstate_t *state);

void frontend_reset_viewspace(ifrontendstate_t *state);

void frontend_free();

void frontend_update(ifrontendstate_t *state);

void frontend_draw_line(float x1, float y1, float x2, float y2, icolor_t color, int z_level);

void frontend_draw_rectangle(ipoint_t *p1, ipoint_t *p2, icolor_t color);

void frontend_draw_point(ipoint_t *point, icolor_t color);

void frontend_draw_coloredtriplet(icoloredtriplet_t *triplet);

int step(); /* Redraw frames, fetch the next event and handle it, etc. */

#endif
