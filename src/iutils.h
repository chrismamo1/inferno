#ifndef _IUTILS_H
#define _IUTILS_H

#include "icoordinates.h"

struct _ilinked_t {
        void *previous;
        void *data;
        void *next;
} ;

typedef struct _ilinked_t ilinked_t;

float ipercent_error(float rough, float expected);

char ifast_epsilon_cmp(float f1, float f2, float epsilon);

float ifast_bigpow(float base, float power);

float ifast_smallpow(float base, float power);

float ifast_rsqrt(float x);

float ifast_sqrt(float x);

float iget_distance(ipoint_t *p1, ipoint_t *p2);

ipoint_t* igetclosest_point_linked(ipoint_t *p, ilinked_t *list, float epsilon);

int inabs(int x);

float ifabs(float x);

int inmin(int *arr, unsigned int len);

int inmax(int *arr, unsigned int len);

int* infastsort(int *arr, unsigned int len);

ilinked_t* inew_linked(void *data);

ilinked_t* iget_linked(ilinked_t *list, int index);

ilinked_t* iset_linked(ilinked_t *list, int index, void *data);

ilinked_t* iadd_linked(ilinked_t *list, ilinked_t *n);

ilinked_t* igetfirst_linked(ilinked_t *list);

ilinked_t* igetlast_linked(ilinked_t *list);

ilinked_t* igetnext_linked(ilinked_t *list);

ilinked_t* iremove_linked(ilinked_t *list);

int icontains_linked(ilinked_t *list, void *data);

ilinked_t* iaddunique_linked(ilinked_t *list, ilinked_t *n);

void ifree_linked(ilinked_t *list, char free_contents);

#endif
