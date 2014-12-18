struct ievent_handler;
struct ifrontendstate_t;

#ifndef _FRONTEND_H
#define _FRONTEND_H

#include "iprimitives.h"
#include "icoordinates.h"

#include <X11/X.h>
#include <X11/Xlib.h>

struct ievent_handler {
        void* (*handler)(void *args, void *event);
        void *args;
        unsigned short key_code;
} ;

struct ifrontendstate_t {
        struct icolor_t *background_color;
        struct icolor_t *drawing_color;
        char z_levels;
        float left;
        float right;
        float bottom;
        float top;

        struct ievent_handler *onkeydown_handlers[256];
        struct ievent_handler *onkeyup_handlers[256];

        struct ilinked_t *regions;
        unsigned short num_regions;
} ;

struct ievent_handler* inew_eventhandler(void* (*handler)(void *args, void *event), void *args, unsigned short key_code);

void frontend_initialize();

void frontend_set_eventhandler(unsigned short i, char down, struct ievent_handler *h, struct ifrontendstate_t *state);

void frontend_initialize_viewspace(float x1, float x2, float y1, float y2, int num_z_levels, struct ifrontendstate_t *state);

void frontend_reset_viewspace(struct ifrontendstate_t *state);

void frontend_free();

void frontend_update(struct ifrontendstate_t *state);

void frontend_draw_line(float x1, float y1, float x2, float y2, struct icolor_t color, int z_level);

void frontend_draw_rectangle(struct ipoint_t *p1, struct ipoint_t *p2, struct icolor_t color);

void frontend_draw_point(struct ipoint_t *point, struct icolor_t color);

void frontend_draw_coloredtriplet(struct icoloredtriplet_t *triplet);

int step(); /* Redraw frames, fetch the next event and handle it, etc. */

#endif
