#include "igraphics.h"

#include "frontend.h"
#include "iutils.h"
#include "iprimitives.h"

void draw_object(struct iobject_t *object)
{
        struct ilinked_t *li = /*igetfirst_linked(*/object->triplets/*)*/;
        for ( ; li != NULL; li = li->next) {
                frontend_draw_coloredtriplet(li->data);
        }
        return;
        do {
                frontend_draw_coloredtriplet(li->data);
        } while ( (li = igetnext_linked(li)) );
}
