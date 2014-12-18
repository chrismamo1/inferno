struct ilinked_t;

#ifndef _IUTILS_H
#define _IUTILS_H

//#include "frontend.h"
#include "icoordinates.h"
#include "itypes.h"

struct ilinked_t {
        struct ilinked_t *previous;
        void *data;
        struct ilinked_t *next;
} ;

//float ipercent_error(float rough, float expected);

#define ifabs(x) ((float)((x) < 0.0 ? (0.0 - (x)) : (x)))

#define ipercent_error(r, e) ((e) == 0.f ? ((r) == 0 ? 0 : 100) : ifabs(100.f * ifabs((e)-(r))/(e)))
#define ifast_epsilon_cmp(f1, f2, e) ((ifabs((f1) - (f2)) < e) ? 0 : ((f1) < (f2) ? -1 : 1))
#define inabs(x) (((x) + ((x) >> (sizeof(int) * 8 - 1))) ^ ((x) >> (sizeof(int) * 8 - 1)))

float ifast_bigpow(float base, float power);

float ifast_smallpow(float base, float power);

float ifast_rsqrt(float x);

float ifast_sqrt(float x);

float iget_distance(struct ipoint_t *p1, struct ipoint_t *p2);

struct ipoint_t* igetclosest_point_linked(struct ipoint_t *p, struct ilinked_t *list, float epsilon);

int inmin(int *arr, unsigned int len);

int inmax(int *arr, unsigned int len);

int* infastsort(int *arr, unsigned int len);

struct ilinked_t* inew_linked(void *data);

struct ilinked_t* iget_linked(struct ilinked_t *list, int index);

struct ilinked_t* iset_linked(struct ilinked_t *list, int index, void *data);

struct ilinked_t* iadd_linked(struct ilinked_t *list, struct ilinked_t *n);

struct ilinked_t* igetfirst_linked(struct ilinked_t *list);

struct ilinked_t* igetlast_linked(struct ilinked_t *list);

struct ilinked_t* igetnext_linked(struct ilinked_t *list);

struct ilinked_t* iremove_linked(struct ilinked_t *list);

int icontains_linked(struct ilinked_t *list, void *data);

struct ilinked_t* iaddunique_linked(struct ilinked_t *list, struct ilinked_t *n);

void ifree_linked(struct ilinked_t *list, char free_contents);

#endif
