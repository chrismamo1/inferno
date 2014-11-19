#include "icoordinates.h"

#ifndef _IPRIMITIVES_H
#define _IPRIMITIVES_H

#include "iutils.h"

typedef struct _icolor_t           /**
                                    * red   = 0
                                    * green = 1
                                    * blue  = 2
                                    */
{ unsigned char r; unsigned char g; unsigned char b;} icolor_t;

typedef struct _icolorf_t { float data[3]; } icolorf_t;

typedef short ibytepair_t; /**
                            * first  = pair >> 8
                            * second = pair | 0xFF
                            */

ibytepair_t make_bytepair(char zero, char one);

typedef struct _itriplet_t {
        ipoint_t *vertices[3];
} itriplet_t;

typedef struct _icoloredtriplet_t {
        ipoint_t *vertices[3];
        icolor_t *colors[3];
} icoloredtriplet_t;

unsigned char red(icolor_t color);

unsigned char green(icolor_t color);

unsigned char blue(icolor_t color);

float redf(icolorf_t color);

float greenf(icolorf_t color);

float bluef(icolorf_t color);

typedef struct _iobject_t {
        struct _ilinked_t *triplets; /* icoloredtriplet_t */
        int num_triplets;
        ilinkedpoint_t *points;
        int num_points;
        ivector_t *motion;
        int (*step)(void*);
} iobject_t;

icolor_t* inew_color(icolor_t *rval, unsigned char r, unsigned char g, unsigned char b);

icoloredtriplet_t* inew_coloredtriplet(ipoint_t *p1, icolor_t *c1, ipoint_t *p2, icolor_t *c2, ipoint_t *p3, icolor_t *c3);

iobject_t* init_iobject(iobject_t *iobject);

iobject_t* iobject_add_triplet(iobject_t *object, icoloredtriplet_t *triplet);

void iobject_step(iobject_t *obj);

void iprint_object(iobject_t *object);

void idelete_object(iobject_t **object);

#endif
