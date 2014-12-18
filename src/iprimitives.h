struct icolor_t;
struct _icoloredtriplet_t;
typedef struct _icoloredtriplet_t icoloredtriplet_t;

#ifndef _IPRIMITIVES_H
#define _IPRIMITIVES_H

#include "frontend.h"
#include "iutils.h"
#include "icoordinates.h"

struct icolor_t {
        unsigned char r;
        unsigned char g;
        unsigned char b;
} ;

typedef struct _icolorf_t { float data[3]; } icolorf_t;

typedef short ibytepair_t; /**
                            * first  = pair >> 8
                            * second = pair | 0xFF
                            */

ibytepair_t make_bytepair(char zero, char one);

typedef struct _itriplet_t {
        struct ipoint_t *vertices[3];
} itriplet_t;

struct _icoloredtriplet_t {
        struct ipoint_t *vertices[3];
        struct icolor_t *colors[3];
} ;

unsigned char red(struct icolor_t color);

unsigned char green(struct icolor_t color);

unsigned char blue(struct icolor_t color);

float redf(icolorf_t color);

float greenf(icolorf_t color);

float bluef(icolorf_t color);

typedef struct _iobject_t {
        struct ilinked_t *triplets; /* icoloredtriplet_t */
        int num_triplets;
        struct ilinkedpoint_t *points;
        int num_points;
        struct ivector_t *motion;
        int (*step)(void*);
} iobject_t;

struct icolor_t* inew_color(struct icolor_t *rval, unsigned char r, unsigned char g, unsigned char b);

icoloredtriplet_t* inew_coloredtriplet(struct ipoint_t *p1, struct icolor_t *c1, struct ipoint_t *p2, struct icolor_t *c2, struct ipoint_t *p3, struct icolor_t *c3);

iobject_t* init_iobject(iobject_t *iobject);

iobject_t* iobject_add_triplet(iobject_t *object, icoloredtriplet_t *triplet);

void iobject_step(iobject_t *obj);

void iprint_object(iobject_t *object);

void idelete_object(iobject_t **object);

#endif
