#include "iprimitives.h"
#include "iutils.h"
#include "iprettyconsole.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

ibytepair_t make_bytepair(char zero, char one)
{
        return (short)one + ((short)zero << 8);
}

unsigned char red(icolor_t color)
{
        return color.r;
}

unsigned char green(icolor_t color)
{
        return color.g;
}

unsigned char blue(icolor_t color)
{
        return color.b;
}

float redf(icolorf_t color)
{
        return color.data[0];
}

float greenf(icolorf_t color)
{
        return color.data[1];
}

float bluef(icolorf_t color)
{
        return color.data[2];
}

icolor_t* inew_color(icolor_t *rval, unsigned char r, unsigned char g, unsigned char b)
{
        free(rval);
        rval = malloc(sizeof(icolor_t));
        rval->r = r; rval->g = g; rval->b = b;
        return rval;
}

icoloredtriplet_t* inew_coloredtriplet(ipoint_t *p1, icolor_t *c1, ipoint_t *p2, icolor_t *c2, ipoint_t *p3, icolor_t *c3)
{
        icoloredtriplet_t *rval = malloc(sizeof(icoloredtriplet_t));
        rval->vertices[0] = p1; rval->vertices[1] = p2; rval->vertices[2] = p3;
        rval->colors[0] = c1; rval->colors[1] = c2; rval->colors[2] = c3;
        return rval;
}

void iobject_step(iobject_t *obj)
{
        ilinkedpoint_t *point = obj->points;
        for ( ; point != NULL; point = point->next) {
                ipoint_t *data = point->point;
                data->x = (double)(data->x) + obj->motion->x;
                data->y = (double)(data->y) + obj->motion->y;
        }
        return;
}

iobject_t* init_iobject(iobject_t *object)
{
        object = malloc(sizeof(iobject_t));
        object->triplets = NULL;
        object->num_triplets = 0;
        object->points   = NULL;
        object->num_points   = 0;
        object->motion = inew_vector(0.0f, 0.0f);
        return object;
}

/** add a triplet to an iobject, and rewire all of its contained points to the iobject's internal point list
 * using runtime redundant vertex elimination for speed and a reduced memory footprint
 * @param *object a pointer to an iobject
 * @param *triplet a pointer to an inferno colored triplet
 */
iobject_t* iobject_add_triplet(iobject_t *object, icoloredtriplet_t *triplet)
{
        float epsilon = 0.001;
        object->num_triplets = object->num_triplets + 1;///Tell the data structure that
                                                        ///there is one more triplet in the house.
        if (object->triplets == NULL)
                object->triplets = iadd_linked(NULL, inew_linked(triplet));
        else
                iadd_linked( object->triplets, inew_linked(triplet) );

        if (object->points == NULL) {
                ipoint_t **points = ((icoloredtriplet_t*)(igetlast_linked(object->triplets)->data))->vertices;
                object->points = inew_linkedpoint(points[0]);
                iadd_linkedpoint(object->points,
                                        inew_linkedpoint(points[1]));
                iadd_linkedpoint(object->points,
                                        inew_linkedpoint(points[2]));
                object->num_points = 3;
        } else {
                ipoint_t **points = ((icoloredtriplet_t*)(igetlast_linked(object->triplets)->data))->vertices;
                int i;
                for ( i = 0; i < 3; i++) {
                        ilinkedpoint_t *iterator = object->points;
                        for ( ; iterator != NULL; iterator = iterator->next) {
                                float distance = iget_distance(points[i], iterator->point);
                                if (distance <= epsilon
                                        && points[i] != iterator->point) { /* if the distance betweed the current
                                                                              candidate point and the current
                                                                              incumbent point is within epsilon of
                                                                              zero */
                                        printf(KWHT "INFO: found incumbent vertex within EPSILON, swapping reference.\n" RESET);
                                        printf(KWHT "INFO: incumbent @%p, new @%p.\n" RESET, iterator->point, points[i]);
                                        ipoint_t *incumbent = iterator->point; /* fetch the address of the current incumbent
                                                                                  point */
                                        incumbent->x = (float)(incumbent->x + points[i]->x)/2.; /* average the two points to minimize
                                                                                           visual distortion */
                                        incumbent->y = (float)(incumbent->y + points[i]->y)/2.;
                                        incumbent->z = ((float)(incumbent->z + points[i]->z))/2;
                                        free(points[i]); /* free the point which is about to be discarded */
                                        points[i] = iterator->point; /* reset the address stored in the current slot in the
                                                                        triplet's array to the address of the incumbent */
                                        assert(points[i]->z==incumbent->z);
                                        assert(points[i]->y==incumbent->y);
                                        assert(points[i]->x==incumbent->x);
                                        continue;
                                } // else {
                                ilinkedpoint_t *test_point = inew_linkedpoint(points[i]);
                                if (!icontains_linkedpoint(object->points, test_point)) {
                                        iadd_linkedpoint(object->points, test_point);
                                        object->num_points++;
                                }
                                //if (iaddunique_linked(igetlast_linked(object->points), test_point) == NULL)
                                //        free(test_point);
                        }
                }
        }
        return object;
}

void iprint_object(iobject_t *object)
{
        printf("object " KYEL "@%p" RESET ".\n", object);
        printf("Motion vector = ");
        iprint_vector(object->motion);
        printf("%d triplets:\n", object->num_triplets);
        ilinked_t *trp = igetfirst_linked(object->triplets);
        for ( ; trp != NULL; trp=trp->next) {
                printf("\ttriplet at address %p.\n", trp->data);
                printf("\t L > points:\n");
                icoloredtriplet_t *colored = (icoloredtriplet_t*)(trp->data);
                ipoint_t *pnt = colored->vertices[0];
                icolor_t **colors = colored->colors;
                printf("\t      | > @%p(%.3f, %.3f, %d)", pnt, pnt->x, pnt->y, pnt->z);
                 printf(" rgb(%d, %d, %d) @%p\n", colors[0]->r, colors[0]->g, colors[0]->b, colors[0]);
                pnt = colored->vertices[1];
                printf("\t      | > @%p(%.3f, %.3f, %d)", pnt, pnt->x, pnt->y, pnt->z);
                 printf(" rgb(%d, %d, %d) @%p\n", colors[1]->r, colors[1]->g, colors[1]->b, colors[1]);
                pnt = colored->vertices[2];
                printf("\t      L > @%p(%.3f, %.3f, %d)", pnt, pnt->x, pnt->y, pnt->z);
                 printf(" rgb(%d, %d, %d) @%p\n", colors[2]->r, colors[2]->g, colors[2]->b, colors[2]);
        }
        printf("%d points are in the global points register:\n", object->num_points);
        ilinkedpoint_t *pnt = object->points;
        for ( ; pnt != NULL; pnt=pnt->next) {
                printf("\tpoint at address %p.\n", pnt->point);
        }
}

void idelete_object(iobject_t **object)
{
        ifree_linked((*object)->triplets, 'y');
        ifree_linkedpoints((*object)->points, 'y');
        free(*object);
        *object = NULL;
        free((*object)->motion);
}

void idelete_coloredtriplet(icoloredtriplet_t *triplet)
{
        free(triplet);
}
