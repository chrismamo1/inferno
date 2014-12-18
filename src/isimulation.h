#ifndef _ISIMULATION_H
#define _ISIMULATION_H

#include "iutils.h"
#include "iprimitives.h"

typedef struct _aabb {
        float x; float y;
        float width; float height;
} aabb_t;

int idetectcollision_aabb(aabb_t hb1, aabb_t hb2);

int idetectcollision_object(struct iobject_t *o1, struct iobject_t *o2);

aabb_t icomputeaabb_object(struct iobject_t *o);

aabb_t icomputeaabb_coloredtriplet(struct icoloredtriplet_t *t);

void iprint_aabb(aabb_t aabb);

#endif
