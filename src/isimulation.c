#include <stdlib.h>
#include <stdio.h>
#include "isimulation.h"
#include "iutils.h"
#include "icoordinates.h"
#include "iprimitives.h"

int idetectcollision_aabb(aabb_t hb1, aabb_t hb2)
{
        int rval = 0;

        if ((hb2.x >= hb1.x && hb2.x <= hb1.x + hb1.width) || (hb2.x < hb1.x && hb2.x + hb2.width >= hb1.x))
                rval++; // because they collide on the x axis
        if ((hb2.y >= hb1.y && hb2.y <= hb1.y + hb1.height) || (hb2.y < hb1.y && hb2.y + hb2.height >= hb1.y))
                rval++; // because they collide on the y axis

        return rval / 2; // because of integer division, this will return
                         // 1 only if they collide on both axes

}

int idetectcollision_object(iobject_t *o1, iobject_t *o2)
{
        if (((ipoint_t*)(o1->points->point))->z != ((ipoint_t*)(o2->points->point))->z)
                return 0;
        aabb_t hb_o1 = icomputeaabb_object(o1);
        aabb_t hb_o2 = icomputeaabb_object(o2);
        
        if (idetectcollision_aabb(hb_o1, hb_o2)) {
                ilinked_t *triplets = o1->triplets; /// TODO: Finish implementing this.
                aabb_t *hbs_o1 = malloc(o1->num_triplets * sizeof(aabb_t));
                int i;
                for (i = 0; triplets != NULL; triplets = triplets->next, ++i) { /// generate recursive AABB's for
                                                                                /// object 1
                        icoloredtriplet_t *t = triplets->data;
                        hbs_o1[i] = icomputeaabb_coloredtriplet(t);
                }

                triplets = o2->triplets;
                aabb_t *hbs_o2 = malloc(o2->num_triplets * sizeof(aabb_t));
                for (i = 0; triplets != NULL; triplets = triplets->next, ++i) { /// generate recursive AABB's for
                                                                                /// object 2
                        icoloredtriplet_t *t = triplets->data;
                        hbs_o2[i] = icomputeaabb_coloredtriplet(t);
                        
                        int j;
                        for (j = 0; j < o1->num_triplets; ++j) { /// walk through the already-generated list for
                                                                 /// recursive AABB's for object 1 and look for
                                                                 /// collisions
                                if (idetectcollision_aabb(hbs_o2[i], hbs_o1[j]))
                                        return 1;
                        }
                }
        }
        return 0;
}

aabb_t icomputeaabb_object(iobject_t *o)
{
        aabb_t hb;

        ilinkedpoint_t *pts = o->points;
        ipoint_t *pnt = pts->point;
        hb.x = pnt->x;
        hb.y = pnt->y;
        hb.width = pnt->x;
        hb.height = pnt->y;
        for ( ; pts != NULL; pts = pts->next) { // find the AABB for o1
                pnt = pts->point;
                if (pnt->x < hb.x)
                        hb.x = pnt->x;
                if (pnt->y < hb.y)
                        hb.y = pnt->y;
                if (pnt->x > hb.width)
                        hb.width = pnt->x;
                if (pnt->y > hb.height)
                        hb.height = pnt->y;
        }
        hb.height -= hb.y;
        hb.width  -= hb.x;

        return hb;
}

aabb_t icomputeaabb_coloredtriplet(icoloredtriplet_t *t)
{
        aabb_t hb;

        ipoint_t **pts = t->vertices;
        ipoint_t *pnt = pts[0];
        hb.x = pnt->x;
        hb.y = pnt->y;
        hb.width = pnt->x;
        hb.height = pnt->y;
        int i;
        for (i = 1; i < 3; ++i) {
                pnt = pts[i];
                if (pnt->x < hb.x)
                        hb.x = pnt->x;
                if (pnt->y < hb.y)
                        hb.y = pnt->y;
                if (pnt->x > hb.width)
                        hb.width = pnt->x;
                if (pnt->y > hb.height)
                        hb.height = pnt->y;
        }
        hb.height -= hb.y;
        hb.width  -= hb.x;

        return hb;
}


void iprint_aabb(aabb_t aabb)
{
        printf("Axis Aligned Bounding Box, lower left-hand corner at (%f, %f),",
                aabb.x, aabb.y);
        printf(" upper right-hand corner at (%f, %f). width = %f, height = %f.\n",
                aabb.x + aabb.width, aabb.y + aabb.height, aabb.width, aabb.height);
}
