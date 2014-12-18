struct iregion_t;
struct ipoint_t;
struct iabsolutepoint_t;
struct ilinkedpoint_t;
struct ivector_t;

#ifndef _ICOORDINATES_H
#define _ICOORDINATES_H

#define DEFAULT_REGION_WIDTH 128
#define DEFAULT_REGION_HEIGHT 128

#include "frontend.h"
#include "itypes.h"

//typedef struct _ifrontendstate_t ifrontendstate_t;

struct iregion_t {
        unsigned short id;
        int x;
        int y;
        uint32 width;
        uint32 height;
} ;

struct ipoint_t {
        float x;
        float y;
        uint8 z;
        unsigned short region;
} ;

struct iabsolutepoint_t {
        long double x;
        long double y;
        uint8 z;
} ;

struct ilinkedpoint_t {
        struct ipoint_t *point;
        struct ilinkedpoint_t *next;
};

struct ilinkedpoint_t* inew_linkedpoint(struct ipoint_t *point);

struct ilinkedpoint_t* iadd_linkedpoint(struct ilinkedpoint_t *head, struct ilinkedpoint_t *point);

struct ilinkedpoint_t* icontains_linkedpoint(struct ilinkedpoint_t *head, struct ilinkedpoint_t *lookfor);

void ifree_linkedpoints(struct ilinkedpoint_t *head, char contents);

struct ivector_t {
        double x;
        double y;
} ;

struct ipoint_t* inew_point(long double x, long double y, uint8 z, struct ifrontendstate_t *state);

struct iabsolutepoint_t* iget_absolutepoint(struct ipoint_t *rel, struct ifrontendstate_t *state);

struct ivector_t* inew_vector(double x, double y);

struct ivector_t* inormalize_vector(struct ivector_t *vector);

struct ivector_t* ipush_vector(struct ivector_t *vector, struct ivector_t *direction, float magnitude);

struct ivector_t* iadd_vector(struct ivector_t *v1, struct ivector_t *v2);

struct iregion_t* icreate_region(int x, int y, uint32 width, uint32 height, struct ifrontendstate_t *state);

struct iregion_t* iget_region(

void iprint_point(struct ipoint_t *point);

void iprint_vector(struct ivector_t *vec);

char* iprints_point(struct ipoint_t *point);

#endif
