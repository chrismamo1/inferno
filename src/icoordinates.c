#include "icoordinates.h"
#include "error.h"
#include "iutils.h"
#include "frontend.h"
#include <stdlib.h>
#include <stdio.h>

/*struct _ilinkedpoint_t {
        ipoint_t *point;
        ilinkedpoint_t *next;
};*/

ilinkedpoint_t* inew_linkedpoint(ipoint_t *point)
{
        if (point) {
                ilinkedpoint_t *rval = malloc(sizeof(ilinkedpoint_t));
                if (rval) {
                        rval->point = point;
                        return rval;
                }
                ierror(I_INITIALIZATION_ERROR);
                return NULL;
        }
        ierror(I_NULL_LINKED_ERROR);
        return NULL;
}

ilinkedpoint_t* iadd_linkedpoint(ilinkedpoint_t *head, ilinkedpoint_t *point)
{
        if (head) {
                if (point) {
#if I_PRESERVE_LINKED_POINTS_ORDER==1
                        ilinkedpoint_t *tmp = head;
                        while (head->next != NULL)
                                head = head->next;
                        head->next = point;
                        return tmp;
#else
                        ilinkedpoint_t *tmp = head->next;
                        head->next = point;
                        point->next = tmp;
                        return head;
#endif
                }
                ierror(I_NULL_LINKED_ERROR);
                return head;
        }
        return point;
}

ilinkedpoint_t* icontains_linkedpoint(ilinkedpoint_t *head, ilinkedpoint_t *lookfor)
{
        ipoint_t *point = lookfor->point;
        while (head) {
                if (head == lookfor)
                        return head;
                if (point->x == head->point->x
                        && point->y == head->point->y
                        && point->z == head->point->z)
                        return head;
                head = head->next;
        }
        return NULL;
}

void ifree_linkedpoints(ilinkedpoint_t *head, char contents)
{
        ilinkedpoint_t *tmp = head->next;
        while (head) {
                if (contents == 'y') {
                        free(head->point);
                }
                free(head);
                head = tmp;
                tmp = head->next;
        }
}

ipoint_t* inew_point(double x, double y, int z/*, ifrontendstate_t *state*/)
{
        ipoint_t *rval = malloc(sizeof(ipoint_t));
        rval->x = x;
        rval->y = y;
        rval->z = z;
        return rval;
}

ivector_t* inew_vector(double x, double y)
{
        ivector_t *rval = malloc(sizeof(ivector_t));
        rval->x = x;
        rval->y = y;
        return rval;
}

ivector_t* inormalize_vector(ivector_t *vector)
{
        float invlength = ifast_rsqrt(vector->x * vector->x + vector->y * vector->y);
        vector->x *= invlength;
        vector->y *= invlength;
        return vector;
}

ivector_t* ipush_vector(ivector_t *vector, ivector_t *direction, float magnitude)
{
        
        return vector;
}

ivector_t* iadd_vector(ivector_t *v1, ivector_t *v2)
{
        v1->x += v2->x;
        v1->y += v2->y;
        return v1;
}

void iprint_point(ipoint_t *point)
{
        printf("ipoint_t at %p, coordinates(%f, %f, %d)", point,
                        point->x,
                        point->y,
                        point->z
              );
        return;
}

void iprint_vector(ivector_t *vec)
{
        printf("ivector_t @%p, (x, y) = (%lf, %lf)\n", vec, vec->x, vec->y);
}

size_t str_len(char *string) // homebrew strlen function, because
                             // we really don't need the entire
                             // standard library string.h
{
        size_t i;
        for ( i = 0; *(string + i) != '\0'; ++i) ;
        return i - 1;
}

char* iprints_point(ipoint_t *point)
{
        char *buffer = malloc(64 * sizeof(char));
        sprintf(buffer, "ipoint_t at %p, coordinates(%f, %f, %d)",
                        point,
                        point->x,
                        point->y,
                        point->z
              );
        buffer = realloc(buffer, str_len(buffer) + 1);
        return buffer;
}
