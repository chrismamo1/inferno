/**
 * @author John Christopher McAlpine
 * @todo Let this be a warning to you: I have abused the ever loving SHIT out
 * of Hungarian notation up in 'hyah.
 */

#include <stdio.h>
#include <stdlib.h>

#include "icoordinates.h"
#include "iutils.h"
#include "error.h"

float ipercent_error(float rough, float expected)
{
        if (expected == 0.f) return rough == 0 ? 0 : 100;
        return ifabs(100. * ifabs(expected - rough)/expected);
}

char ifast_epsilon_cmp(float f1, float f2, float epsilon)
{
        if (ifabs(f1 - f2) < epsilon)
                return 0;
        if (f1 < f2)
                return -1;
        else
                return 1;
}

float ifast_bigpow(float base, float power)
{
        if (power == 0.) return 1.;
        if (base == 0) return 0;
        if (power < 0) return (1. / ifast_bigpow(base, ifabs((int)power)))
                * ifast_smallpow(base, power - (float)((int)power));
        if (power == 1.) return base;
        float big_part = base * ifast_smallpow(base, power - (float)( (int) power )); // because base**power
                                                                                      // is equal to the small
                                                                                      // part times the big
                                                                                      // part
        int big_exponent = (int)power; // casting + truncation + ??? = profit
        for ( ; big_exponent > 1; --big_exponent) // using prefix decrement because fuck you that's why
                big_part *= base;
        return big_part;
}

float ifast_smallpow(float base, float power)
{
        if (power == 0) return 1;
        if (power < 0) {
                if (power == -0.5) return ifast_rsqrt(base);
                return 1/ifast_smallpow(base, ifabs(power));
        }
        union {
                int i;
                float f;
        } data = { .f = base }; /* why do it in one line when you can do it in four? */
        data.i = 0x3f7a3bea + power * (data.i - 0x3f7a3bea); // evil bit-level hack, it was hard to write so it should
                                                             // be hard to read too, bitch :)
        return data.f;
}

float ifast_rsqrt(float x) /// All credit the the authors of the Quake engine
{
        float xhalf = 0.5f * x;
        union {
                int i;
                float f;
        } data = { .f = x };
        //int i = *(int*)&x;         // evil floating point bit level hacking
        data.i = 0x5f3759df - (data.i >> 1);  // what the fuck?
        //x = *(float*)&i;
        data.f = data.f*(1.5f-(xhalf*data.f*data.f));
        return data.f;
}

/**
 * @todo get rid of all those gorram iterations
 * of Newton's method, except we need them for
 * now to get good levels of precision.
 * @todo why is it more accurate to just take the
 * magic number hack for rsqrt and inverse it?!?
 * @todo I'm sure that rearranging the terms in
 * my Newton's method approximation could kill at
 * least one operation
 */
float ifast_sqrt(float x)
{
        if (x == 0) return x;           // this is necessary to keep it from breaking when x is 0
        float xt2 = 2*x;                // convenience constant for our Newton's Method calculations later
        union {
                int i;
                float f;
        } data = { .f = x };
        //int i = *(int*)&x;              // interpret bits as an int
        data.i = 0x1fbd1df5 + (data.i >> 1);      // wtf magic number hack, no helpful comment for this, it was hard
                                        // to write so it should be hard to read
        //float y = *(float*)&i;        // reinterpret as a single precision float
        ///Fuck, I was using the wrong Newton's method, I think...
        data.f = data.f - ((data.f * data.f) - x)/xt2;      // 1st iteration of Newton's method for additional precision
        data.f = data.f - ((data.f * data.f) - x)/xt2;      // 2nd iteration of Newton's method, can this be removed?
        return data.f;
}

int inabs(int x)
{
        int mask = x >> (sizeof(int) * 8 - 1); // what the fuck?
        return (x + mask) ^ mask; // seriously, this is almost as bad as the exponentiation functions
}

float ifabs(float x)
{
        union {
                int i;
                float f;
        } data = { .f = x };
        //int i = *(int*)&x;  // interpret the bits as an integer
        data.i = data.i & 0x7fffffff; // because FUCK you, that's why
        return data.f; // reinterpret as a single-precision float for the return
}

int inmin(int *arr, unsigned int length) // pretty much the only optimization here is the hard coded loop.
                                         // this is only efficient for very large arrays
{
        int len = length;
        int rval = arr[--len];
        while (len > 8) {
                if ( arr[len] < rval)
                        rval = arr[len];
                if ( arr[len - 1] < rval) // not using either decrement operator here because a decrement
                                          // is actually a retrieval, subtraction, and an assignment, while
                                          // this is just a retrieval and a subtraction.
                        rval = arr[len - 1];
                if ( arr[len - 2] < rval)
                        rval = arr[len - 2];
                if ( arr[len - 3] < rval)
                        rval = arr[len - 3];
                if ( arr[len - 4] < rval)
                        rval = arr[len - 4];
                if ( arr[len - 5] < rval)
                        rval = arr[len - 5];
                if ( arr[len - 6] < rval)
                        rval = arr[len - 6];
                if ( arr[len - 7] < rval)
                        rval = arr[len - 7];
                len = len - 8;
        }

        for (; len >= 0; --len)
                if ( arr[len] < rval)
                        rval = arr[len];
        return rval;
}

int inmax(int *arr, unsigned int length)
{
        int len = length;
        int rval = arr[--len];
        while (len > 8) {
                if ( arr[len] > rval)
                        rval = arr[len];
                if ( arr[len - 1] > rval)
                        rval = arr[len - 1];
                if ( arr[len - 2] > rval)
                        rval = arr[len - 2];
                if ( arr[len - 3] > rval)
                        rval = arr[len - 3];
                if ( arr[len - 4] > rval)
                        rval = arr[len - 4];
                if ( arr[len - 5] > rval)
                        rval = arr[len - 5];
                if ( arr[len - 6] > rval)
                        rval = arr[len - 6];
                if ( arr[len - 7] > rval)
                        rval = arr[len - 7];
                len = len - 8;
        }

        for (; len >= 0; --len)
                if ( arr[len] > rval)
                        rval = arr[len];
        return rval;
}

/// LOCAL
void nquicksort(int *arr, unsigned int low, unsigned int high)
{
        if (low < high) {
                int pivot = (high + low) / 2;
                int pvalue = arr[pivot];
                int tmp = arr[high];
                arr[high] = arr[pivot];
                arr[pivot] = tmp;
                int i, j;
                for (i = low, j = 0; i < high; ++i) {
                        if (arr[i] < pvalue) {
                                tmp = arr[j];
                                arr[j] = arr[i];
                                arr[i] = tmp;
                                ++j;
                        }
                }
                tmp = arr[high];
                arr[high] = pvalue;
                arr[pivot] = tmp;
        }
        return;
}

int* infastsort(int *arr, unsigned int len)
{
        if (len > 3) {
                /// do a sane sorting algorithm
                return arr;
        }
        /// God, please forgive me for this sin which I am about to commit
        int tmp;
        switch (len) {
        case 1: break;
        case 2:
                if (arr[0] > arr[1]) {
                        tmp = arr[0];
                        arr[0] = arr[1];
                        arr[1] = tmp;
                }
                break;
        case 3:
                if (arr[0] > arr[1]) { /// (2), 1, (2), 0, (2)
                        if (arr[2] > arr[1]) {  /// 1, (2), 0, (2)
                                if (arr[2] > arr[0]) { /// 1, 0, 2
                                        tmp = arr[0];
                                        arr[0] = arr[1];
                                        arr[1] = tmp;
                                        break;
                                }
                                /// 1, 2, 0
                                tmp = arr[0];
                                arr[0] = arr[1];
                                arr[1] = arr[2];
                                arr[2] = tmp;
                                break;
                        } /// 2, 1, 0
                        tmp = arr[0];
                        arr[0] = arr[2];
                        arr[2] = tmp;
                        break;
                }
                /// (2), 0, (2), 1, (2)
                if (arr[2] > arr[0]) { /// 0, (2), 1, (2)
                        if (arr[2] > arr[1]) break; /// 0, 1, 2
                        /// 0, 2, 1
                        tmp = arr[1];
                        arr[1] = arr[2];
                        arr[2] = tmp;
                        break;
                }
                /// 2, 0, 1
                tmp = arr[0];
                int tmp2 = arr[1];
                arr[0] = arr[2]; // 2, 1, 2
                arr[1] = tmp;    // 2, 0, 2
                arr[2] = tmp2;   // 2, 0, 1
                break;
        /*case 4:
                int tmp;
                if (arr[0] < arr[1] && arr[0] < arr[2]) { /// 0, (1,2,3), (1,2,3), (1,2,3)
                        
                }*/
        default:
                ierror(I_IMPOSSIBLE_ERROR);
                break;
        }
        return arr;
}

float iget_distance(ipoint_t *p1, ipoint_t *p2)
{
        float dx = p1->x - p2->x;
        float dy = p1->y - p2->y;
        int dz   = p1->z - p2->z;
        return ifast_sqrt(dx*dx + dy*dy + (float) (dz*dz));
}

ipoint_t* igetclosest_point_linked(ipoint_t *p, ilinked_t *list, float epsilon)
{
        float cdist = epsilon + 1;
        list = igetfirst_linked(list);
        ipoint_t *rval = NULL;
        do {
                if (list->data == NULL)
                        continue;
                
                float dist = iget_distance(p, list->data);

                if (dist < cdist) {
                        rval = list->data;
                        cdist = dist;
                }
        } while ( (list = list->next) != NULL);

        if (cdist < epsilon) return rval;
        return NULL;
}

ilinked_t* inew_linked(void *data)
{
        ilinked_t *rval = malloc(sizeof(ilinked_t));
        if (rval == NULL) {
                ierror(I_INITIALIZATION_ERROR);
        }
        rval->data = data;
        rval->previous = NULL;
        rval->next = NULL;
        return rval;
}

ilinked_t* iget_linked(ilinked_t *list, int index)
{
        for ( ; list->previous != NULL; list = list->previous) ;
        int i;
        for (i = 0; i < index; i++) {
                if (list->next==NULL) return NULL;
                list = list->next;
        }
        return list;
}

ilinked_t* iset_linked(ilinked_t *list, int index, void *data)
{
        if (list == NULL) {
                ierror(I_NULL_LINKED_ERROR);
                return NULL;
        }
        ilinked_t *get = iget_linked(list, index);
        get->data = data;
        return get;
}

ilinked_t* iadd_linked(ilinked_t *list, ilinked_t *n)
{
        ilinked_t *rval = list;
        if (n == NULL) {
                ierror(I_NULL_LINKED_ERROR);
                return list;
        }
        if (n->previous != NULL) {
                ierror(I_NULL_LINKED_ERROR);
                return NULL;
        }
        if (list == NULL) {
                n->previous = NULL;
                n->next = NULL;
                return n;
        }
        for ( ; list->next != NULL; list = list->next) ;
        list->next = n;
        n->previous = list;
        return rval;
}

ilinked_t* igetfirst_linked(ilinked_t *list)
{
        if (list == NULL) {
                ierror(I_NULL_LINKED_ERROR);
                return NULL;
        }
        for ( ; list->previous != NULL; list = list->previous) ;
        return list;
}

ilinked_t* igetlast_linked(ilinked_t *list)
{
        if (list == NULL) {
                ierror(I_NULL_LINKED_ERROR);
                return NULL;
        }
        for ( ; list->next != NULL; list = list->next) ;
        return list;
}

ilinked_t* igetnext_linked(ilinked_t *list)
{
        if (list == NULL) {
                ierror(I_NULL_LINKED_ERROR);
                return NULL;
        }
        return list->next;
}

ilinked_t* iremove_linked(ilinked_t *list)
{
        if (list == NULL) {
                ierror(I_NULL_LINKED_ERROR);
                return NULL;
        }
        if (list->previous != NULL)
                ((ilinked_t*)(list->previous))->next = list->next;
        if (list->next != NULL)
                ((ilinked_t*)(list->next))->previous = list->previous;
        ilinked_t *rval = list->next;
        free(list);
        return rval;
}

int icontains_linked(ilinked_t *list, void *data)
{
        list = igetfirst_linked(list);
        for ( ; list != NULL; list=list->next)
                if (list->data == data)
                        return 1;
        return 0;
}

ilinked_t* iaddunique_linked(ilinked_t *list, ilinked_t *n)
{
        if (!icontains_linked(list, n->data))
                iadd_linked(list, n);
        else
                return NULL;
        return list;
}

void ifree_linked(ilinked_t *list, char free_contents)
{
        ilinked_t *tmp = NULL;
        do {
                tmp = list->next;
                if (free_contents == 1 || free_contents == 'y')
                switch (free_contents) {
                        case 1: case 'y': case 't':
                                free(list->data);
                                break;
                        case 0: case 'n': case 'f': default:
                                break;
                }
                free(list);
                list = tmp;
        } while(tmp != NULL);
}
