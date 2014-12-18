#include <stdlib.h>

#include "frontend.h"

/*struct ifrontendstate_t {
        icolor_t background_color;
        icolor_t drawing_color;
        char z_levels;
        float left;
        float right;
        float bottom;
        float top;

        ievent_handler *onkeydown_handlers[256];
        ievent_handler *onkeyup_handlers[256];

        iregion_t **regions; //a dynamic array of pointers to regions
        unsigned short num_regions;
};*/


ievent_handler* inew_eventhandler(void* (*handler)(void *args, void *event), void *args, unsigned short key_code)
{
        ievent_handler *rval = malloc(sizeof(ievent_handler));
        rval->handler = handler;
        rval->args = args;
        rval->key_code = key_code;
        return rval;
}

void frontend_set_eventhandler(unsigned short i, char down, ievent_handler *h, struct ifrontendstate_t *state)
{
        if (down)
                state->onkeydown_handlers[i] = h;
        else
                state->onkeyup_handlers[i] = h;
}
