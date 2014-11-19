#include <X11/X.h>
#include <X11/Xlib.h>

#include "frontend.h"
#include "frontend_opengl_linux.h"
#include "iutils.h"
#include "icoordinates.h"
#include "igraphics.h"
#include "error.h"
#include "isimulation.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * w = 25
 * a = 38
 * s = 39
 * d = 40
 */
void* random_function(void *args, void *ev)
{
        XKeyEvent *event = ev;
        printf("keycode = %d", event->keycode);
        ierror(I_UNDEFINED_EVENT_HANDLER_ERROR);
        return NULL;
}

void* push_object(void *args, void *ev)
{
        iobject_t *object = args;
        XKeyEvent *event = ev;
        switch (event->keycode) {
        case 25:
                iadd_vector(object->motion, inew_vector(0.0, 0.035));
                break;
        case 38:
                iadd_vector(object->motion, inew_vector(-0.035, 0.0));
                break;
        case 39:
                iadd_vector(object->motion, inew_vector(0.0, -0.035));
                break;
        case 40:
                iadd_vector(object->motion, inew_vector(0.035, 0.0));
                break;
        }
        return NULL;
}

int main(int argc, char *argv[])
{
        /*iobject_t *obj = NULL;
        obj = init_iobject(obj);*/

        icolor_t *neon_green = NULL;
        neon_green = inew_color(neon_green, 100, 255, 100);

        /*iobject_add_triplet(obj, inew_coloredtriplet( inew_point(0., 0., 0), neon_green,
                                                       inew_point(1., 0., 0), neon_green,
                                                       inew_point(1., 1., 0), neon_green));

        iobject_add_triplet(obj, inew_coloredtriplet( inew_point(0., 0., 1.), neon_green,
                                                       inew_point(5., 0., 3.), neon_green,
                                                       inew_point(1., 1., 0.01), neon_green));*/

        icolor_t *black = NULL;
        black = inew_color(black, 0, 0, 0);

        icolor_t *purple = NULL;
        purple = inew_color(purple, 153, 0, 153);

        /*iobject_t *corners = NULL;
        corners = init_iobject(corners);
        iobject_add_triplet(corners, inew_coloredtriplet( inew_point(0., 1.9, 0), black,
                                                           inew_point(0., 2.0, 0), black,
                                                           inew_point(0.1, 2.0, 0), black));

        iobject_add_triplet(corners, inew_coloredtriplet( inew_point(2., 1.9, 0), black,
                                                           inew_point(2., 2.0, 0), black,
                                                           inew_point(1.9, 2.0, 0), black));

        iobject_add_triplet(corners, inew_coloredtriplet( inew_point(0., .1, 0), black,
                                                           inew_point(0., 0.0, 0), black,
                                                           inew_point(.1, 0., 0), black));

        iobject_add_triplet(corners, inew_coloredtriplet( inew_point(1.9, 0., 0), black,
                                                           inew_point(2., 0., 0), black,
                                                           inew_point(2., .1, 0), black));*/

        /*iobject_t *triangle = NULL;
        triangle = init_iobject(triangle);
        iobject_add_triplet(triangle, inew_coloredtriplet( inew_point(0.75, 0.8, 0), black,
                                                           inew_point(1.1, 1.5, 0), black,
                                                           inew_point(1.25, 0.7, 0), black));*/

        iobject_t *box = NULL;
        box = init_iobject(box);
        iobject_add_triplet(box, inew_coloredtriplet( inew_point(0.25, 1.25, 0), black,
                                                           inew_point(0.25, 1.75, 0), black,
                                                           inew_point(0.75, 1.75, 0), black));
        iobject_add_triplet(box, inew_coloredtriplet( inew_point(0.25, 1.25, 0), purple,
                                                           inew_point(0.75, 1.25, 0), purple,
                                                           inew_point(0.75, 1.75, 0), purple));

        int i;
        for (i = 8; i < 256; i++) {
                ionkeydown[i] = random_function;
        }

        ionkeydown[25] = &push_object;
        ionkeydown[38] = &push_object;
        ionkeydown[39] = &push_object;
        ionkeydown[40] = &push_object;

        frontend_initialize();
        ifrontendstate_t state;
        frontend_initialize_viewspace(0, 2, 0, 2, 11, &state);
        state.background_color.r = 255;
        state.background_color.g = 255;
        state.background_color.b = 255;
        frontend_reset_viewspace(&state);
        iprint_object(box);
        
        //int coll = idetectcollision_object(triangle, box);
        //printf("Result of collision between triangle and box: %d\n", coll);
        
        box->motion->x = 0.025;

        int val = 0;

        while (!val) {
                XNextEvent(dpy, &xev);
                if (xev.type == Expose)
                        frontend_update(&state);
                else if(xev.type == KeyPress) {
                        (*ionkeydown[xev.xkey.keycode])(box, &(xev.xkey));
                }
                else if (xev.type == KeyRelease) { 
                        (*ionkeyup[xev.xkey.keycode])(NULL, &(xev.xkey));
                }
                else if (xev.type == ClientMessage) {
                        frontend_free();
                        val = 1;
                }
                frontend_update(&state);
                frontend_reset_viewspace(&state);

                val = 0;

                //draw_object(corners);
                //draw_object(triangle);
                //iobject_step(box);
                //iprint_object(box);
                draw_object(box);
        }

        //idelete_object(&corners);
        //idelete_object(&triangle);
        idelete_object(&box);
        //idelete_object(&obj);

        free(neon_green);
        free(black);

        frontend_free();
        printf("Exiting.\n");
        return 0;
}