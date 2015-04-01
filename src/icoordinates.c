#include "frontend.h"
#include "icoordinates.h"
#include "error.h"
#include "iutils.h"
#include "itypes.h"
#include "iprettyconsole.h"
#include <stdlib.h>
#include <stdio.h>

/*struct _ilinkedpoint_t {
        ipoint_t *point;
        ilinkedpoint_t *next;
};*/

/** allocate and initialize a new ilinkedpoint structure, a type of singly
 * linked list specialized for holding ipoints
 * @param *point the ipoint structure the be "contained" the the link
 * @return the new ilinkedpoint, or NULL on failure
 */
struct ilinkedpoint_t* inew_linkedpoint(struct ipoint_t *point)
{
        if (point) {
                struct ilinkedpoint_t *rval = malloc(sizeof(struct ilinkedpoint_t));
                if (rval) {
                        rval->next = NULL;
                        rval->point = point;
                        return rval;
                }
                ierror(I_INITIALIZATION_ERROR);
                return NULL;
        }
        ierror(I_NULL_LINKED_ERROR);
        return NULL;
}

/** add a new point to a linked points structure
 * @param *head the structure we want to append to
 * @param *point the point to be appended
 * @return a pointer to the new head of the list
 */
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

/** See if an ilinkedpoint list contains a point at the same location or with
 * the same value as a given argument.
 * @param *head The presumed "head" of the list
 * @param *lookfor A pointer to the point to search for
 * @return a pointer to the linked list element containing *lookfor, or NULL if
 * *lookfor can't be found in the list
 */
struct ilinkedpoint_t* icontains_linkedpoint(struct ilinkedpoint_t *head, struct ilinkedpoint_t *lookfor)
{
        if (lookfor == NULL || head == NULL)
                return NULL;
        struct ilinkedpoint_t *hd = head;
        struct ipoint_t *point = lookfor->point;
        if (point == NULL)
                return NULL;
        while (hd) {
                if (hd == lookfor)
                        return hd;
                if (hd == NULL)
                        return NULL;
                if (hd->point == NULL)
                        return NULL;
                if (point->x == ((struct ipoint_t*)(hd->point))->x
                        && point->y == hd->point->y
                        && point->z == hd->point->z)
                        return hd;
                hd = hd->next;
        }
        return NULL;
}

/** free a linkedpoint list
 * @param *head The head of the list
 * @param contents determine if the points "contained" in the list should be
 * freed as well. 'y' or 1 mean "yes" and 'n' or '0' mean "no".
 */
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

/** Allocates and initializes a new point using the new region system
 * 
 */
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
        volatile struct ilinked_t *list = igetfirst_linked(state->regions);
        volatile struct iregion_t *region;

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

/*
 * @todo finish this
 */
struct iabsolutepoint_t* iget_absolutepoint(struct ipoint_t *rel, struct ifrontendstate_t *state)
{
        struct iabsolutepoint_t *rval = malloc(sizeof(struct iabsolutepoint_t));
        rval->x = rel->x;
        rval->y = rel->y;
        //rval->x += (long double)
        //
        return rval;
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

/** performs simple vector addition, v1 += v2
 * @param *v1 a pointer to the destination vector
 * @param *v2 a pointer to the source vector
 */
struct ivector_t* iadd_vector(struct ivector_t *v1, struct ivector_t *v2)
{
        v1->x += v2->x;
        v1->y += v2->y;
        return v1;
}

/** prints a point all pretty-like
 * @param *point the point to print
 */
void iprint_point(struct ipoint_t *restrict point)
{
        printf("point " KYEL "@@%p" RESET ", coordinates(%f, %f, %d)\n", (void*)point,
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

/** prints a vector all pretty-like
 * @param *vec the vector to print
 * @todo make it prettier
 */
void iprint_vector(struct ivector_t *restrict vec)
{
        printf("ivector_t @%p, (x, y) = (%lf, %lf)\n", (void*)vec, vec->x, vec->y);
}

/** a static inline homebrew strlen function so that I don't have to include the whole string.h header
 * @param *string the string to find the length of
 * @return length of the string, in number of characters
 */
static inline size_t str_len(char *string)
                             // homebrew strlen function, because
                             // we really don't need the entire
                             // standard library string.h
{
        size_t i;
        for ( i = 0; *(string + i) != '\0'; ++i) ;
        return i;
}

/** prints a point all pretty-like to a new string
 * @param *point a pointer to the point to print
 * @return a string
 */
char* iprints_point(struct ipoint_t *restrict point)
{
        char *buffer = malloc(64 * sizeof(char));
        sprintf(buffer, "ipoint_t at %p, coordinates(%f, %f, %d)",
                        (void*)point,
                        point->x,
                        point->y,
                        point->z
              );
        buffer = realloc(buffer, str_len(buffer));
        return buffer;
}
