struct icolor_t;
struct icoloredtriplet_t;
struct icolorf_t;
struct itriplet_t;
struct iobject_t;

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

struct icolorf_t {
        float data[3];
} ;

struct itriplet_t {
        struct ipoint_t *vertices[3];
} ;

struct icoloredtriplet_t {
        struct ipoint_t *vertices[3];
        struct icolor_t *colors[3];
} ;

unsigned char red(struct icolor_t color);

unsigned char green(struct icolor_t color);

unsigned char blue(struct icolor_t color);

float redf(struct icolorf_t color);

float greenf(struct icolorf_t color);

float bluef(struct icolorf_t color);

struct iobject_t {
        struct ilinked_t *triplets; /* icoloredtriplet_t */
        int num_triplets;
        struct ilinkedpoint_t *points;
        int num_points;
        struct ivector_t *motion;
        int (*step)(void*);
} ;

struct icolor_t* inew_color(struct icolor_t *rval, unsigned char r, unsigned char g, unsigned char b);

struct icoloredtriplet_t* inew_coloredtriplet(struct ipoint_t *p1, struct icolor_t *c1, struct ipoint_t *p2, struct icolor_t *c2, struct ipoint_t *p3, struct icolor_t *c3);

struct iobject_t* init_iobject(struct iobject_t *iobject);

struct iobject_t* iobject_add_triplet(struct iobject_t *object, struct icoloredtriplet_t *triplet);

void iobject_step(struct iobject_t *obj);

void iprint_object(struct iobject_t *object);

void idelete_object(struct iobject_t **object);

#endif
