#include "frontend.h"
#include "frontend_opengl_linux.h"


#ifndef _ICOORDINATES_H
#define _ICOORDINATES_H

typedef struct _iregion_t {
        unsigned short id;
        int x;
        int y;
        int width;
        int height;
} iregion_t;

typedef struct _ipoint_t {
        float x;
        float y;
        unsigned char z;
        unsigned short region;
} ipoint_t;

typedef struct _ilinkedpoint_t ilinkedpoint_t;

struct _ilinkedpoint_t {
        ipoint_t *point;
        ilinkedpoint_t *next;
};

ilinkedpoint_t* inew_linkedpoint(ipoint_t *point);

ilinkedpoint_t* iadd_linkedpoint(ilinkedpoint_t *head, ilinkedpoint_t *point);

ilinkedpoint_t* icontains_linkedpoint(ilinkedpoint_t *head, ilinkedpoint_t *lookfor);

void ifree_linkedpoints(ilinkedpoint_t *head, char contents);

typedef struct _ivector_t {
        double x;
        double y;
} ivector_t;

ipoint_t* inew_point(double x, double y, int z/*, ifrontendstate_t *state*/);

ivector_t* inew_vector(double x, double y);

ivector_t* inormalize_vector(ivector_t *vector);

ivector_t* ipush_vector(ivector_t *vector, ivector_t *direction, float magnitude);

ivector_t* iadd_vector(ivector_t *v1, ivector_t *v2);

void iprint_point(ipoint_t *point);

void iprint_vector(ivector_t *vec);

char* iprints_point(ipoint_t *point);

#endif
