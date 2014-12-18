#include "frontend.h"
#include "icoordinates.h"
#include "error.h"
#include "iutils.h"
#include "itypes.h"
#include <stdlib.h>
#include <stdio.h>

/*struct _ilinkedpoint_t {
        ipoint_t *point;
        ilinkedpoint_t *next;
};*/

struct ilinkedpoint_t* inew_linkedpoint(struct ipoint_t *point)
{
        if (point) {
                struct ilinkedpoint_t *rval = malloc(sizeof(struct ilinkedpoint_t));
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

struct ilinkedpoint_t* iadd_linkedpoint(struct ilinkedpoint_t *head, struct ilinkedpoint_t *point)
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
                        struct ilinkedpoint_t *tmp = head->next;
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

struct ilinkedpoint_t* icontains_linkedpoint(struct ilinkedpoint_t *head, struct ilinkedpoint_t *lookfor)
{
        struct ipoint_t *point = lookfor->point;
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

void ifree_linkedpoints(struct ilinkedpoint_t *head, char contents)
{
        struct ilinkedpoint_t *tmp = head->next;
        while (head) {
                if (contents == 'y') {
                        free(head->point);
                }
                free(head);
                head = tmp;
                tmp = head->next;
        }
}

struct ipoint_t* inew_point(long double x, long double y, uint8 z, struct ifrontendstate_t *state)
{
        struct ipoint_t *rval = malloc(sizeof(struct ipoint_t));
        if (state == NULL) {
                rval->x = x;
                rval->y = y;
                rval->z = z;
                rval->region = -1;
                return rval;
        }
        struct ilinked_t *list = igetfirst_linked(state->regions);
        struct iregion_t *region;

        long int _x = (long int)x, _y = (long int)y;

        if (list == NULL)
                goto HANDLE_NO_VALID_REGIONS;

        for ( ; list != NULL; list = list->next) {
                region = (struct iregion_t*)(list->data);
                if (x > region->x && x < region->x + region->width)
                        if (y > region->y && y < region->y + region->height) {
                                rval->region = region->id;
                                break;
                        }
        }
        if (list == NULL) {
HANDLE_NO_VALID_REGIONS:
                if (x > 0)
                        _x = _x - _x % DEFAULT_REGION_WIDTH;
                else if (x < 0)
                        _x = _x - (DEFAULT_REGION_WIDTH + (_x % DEFAULT_REGION_WIDTH));
                else
                        _x = 0;

                if (y > 0)
                        _y = _y - _y % DEFAULT_REGION_HEIGHT;
                else if (y < 0)
                        _y = _y - (DEFAULT_REGION_HEIGHT + (_y % DEFAULT_REGION_HEIGHT));
                else
                        _y = 0;
                
                struct iregion_t *newregion = NULL;
                newregion = icreate_region(_x, _y, DEFAULT_REGION_WIDTH,
                                                   DEFAULT_REGION_HEIGHT, state);

                region = newregion;
        }
        rval->region = region->id;
        rval->x = x - (long double)(region->x);
        rval->y = y - (long double)(region->y);
        rval->z = z;
        return rval;
}

struct iabsolutepoint_t* iget_absolutepoint(struct ipoint_t *rel, struct ifrontendstate_t *state)
{
        struct iabsolutepoint_t *rval = malloc(sizeof(struct iabsolutepoint_t));
        rval->x = rel->x;
        rval->y = rel->y;
        rval->x += (long double)
}

struct ivector_t* inew_vector(double x, double y)
{
        struct ivector_t *rval = malloc(sizeof(struct ivector_t));
        rval->x = x;
        rval->y = y;
        return rval;
}

struct ivector_t* inormalize_vector(struct ivector_t *vector)
{
        float invlength = ifast_rsqrt(vector->x * vector->x + vector->y * vector->y);
        vector->x *= invlength;
        vector->y *= invlength;
        return vector;
}

struct ivector_t* ipush_vector(struct ivector_t *vector, struct ivector_t *direction, float magnitude)
{
        
        return vector;
}

struct ivector_t* iadd_vector(struct ivector_t *v1, struct ivector_t *v2)
{
        v1->x += v2->x;
        v1->y += v2->y;
        return v1;
}

void iprint_point(struct ipoint_t *point)
{
        printf("ipoint_t at %p, coordinates(%f, %f, %d)", point,
                        point->x,
                        point->y,
                        point->z
              );
        return;
}

struct iregion_t* icreate_region(int x, int y, uint32 width, uint32 height, struct ifrontendstate_t *state)
{
        struct iregion_t *region = malloc(sizeof(struct iregion_t));
        region->x = x;
        region->y = y;
        region->width = width;
        region->height = height;

        if (state->regions == NULL) {
                region->id = 0;
                state->regions = inew_linked(region);
                state->num_regions = 1;
        } else {
                uint16 id;
                struct ilinked_t *cur = igetfirst_linked(state->regions);
                for (id = 0; cur != NULL; cur = cur->next)
                        if (((struct iregion_t*)(cur->data))->id > id)
                                id = ((struct iregion_t*)(cur->data))->id;
                id = id + 1;
                region->id = id;
                iadd_linked(state->regions, inew_linked(region));
                state->num_regions++;
        }
        return region;
}

void iprint_vector(struct ivector_t *vec)
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

char* iprints_point(struct ipoint_t *point)
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
