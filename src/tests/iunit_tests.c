#include "../error.h"
#include "frontend.h"
#include "frontend_opengl_linux.h"
#include "../icoordinates.h"
#include "../igraphics.h"
#include "../iprimitives.h"
#include "../iutils.h"
#include "../iprettyconsole.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*void iassert(int val);

void iassert_setmsg(char *msg);

char *_iassert_msg;

double dabs(double x)
{
        long int y = *(long int*)&x;
        y = y & 0x7fffffffffffffff;
        return *(double*)&y;
}

char depscmp(double a, double b, double eps)
{
        if (dabs(a - b) < eps)
                return 0;
        if (a < b)
                return -1;
        else return 1;
}

double basic_pow(double base, int pow)
{
        int i = 0;
        double rval = 1.;
        for ( ; i < pow; ++i)
                rval = rval * base;
        return rval;
}

double slow_nrt(double x, int root)
{
        double guess = x / 2, gsq = x * 2, step = x / 2.;
        double epsilon = 0.00000001;
        while (depscmp(gsq, x, epsilon) != 0) {
                gsq = basic_pow(guess, root);
                if (gsq > x) 
                        guess = guess - (step = step / 2);
                else if (gsq < x)
                        guess = guess + (step = step / 2);
                else if (gsq == x) return guess;
        }
        return guess;
}

float get_average_accuracy(float start, float limit, int root)
{
        double sum = 0;
        float i = start;
        float exp = 1/(float)root;
        for ( ; (int)i <= (int)limit; i+=1.0f) {
                sum += (double)ipercent_error(ifast_smallpow(i, exp), (float)slow_nrt(i, root));
        }
        if (ifast_epsilon_cmp(limit - (int)limit, 0, 0.01) != 0) {
                sum += (double)ipercent_error(ifast_smallpow(limit, exp), (float)slow_nrt(limit, root));
                sum = sum / (double)((int)i + 1);
                return sum;
        } else {
                sum = sum / (double)((int)i);
                return sum;
        }
}*/

int main()
{
        
        /*int rt = 3;
        float l = 1;
        float h = 1000000;
        printf("Average percent error of smallpow for %dth root from %f to %f: %f%%\n", rt, l, h, get_average_accuracy(l, h, rt));*/
        

        ishowerrors = 0;

        /* move on to tests of iutils */
        printf(KWHT "Making sure that ifast_epsilon_cmp works for all possible test situations... ");
        /*test situations:
         * = - -
         * = + +
         * < - -
         * < - +
         * < + +
         * > - -
         * > + -
         * > + +
         */
        // =
        assert(!ifast_epsilon_cmp(-675.02, -675.013, 0.011)); assert(!ifast_epsilon_cmp(-675.013, -675.02, 0.011));
        assert(!ifast_epsilon_cmp(675.02, 675.013, 0.011)); assert(!ifast_epsilon_cmp(675.013, 675.02, 0.011));
        // <
        assert( ifast_epsilon_cmp(-675.02, -675.013, 0.001) == -1);
        assert( ifast_epsilon_cmp(-675.02, 675.013, 0.011)  == -1);
        assert( ifast_epsilon_cmp(675.013, 675.02, 0.001) == -1);
        // >
        assert( ifast_epsilon_cmp(-675.013, -675.02, 0.001) == 1);
        assert( ifast_epsilon_cmp(675.013, -675.02, 0.011)  == 1);
        assert( ifast_epsilon_cmp(675.02, 675.013, 0.001) == 1);
        printf(KGRN "Success.\n" KWHT);
        
        /* move on to tests of ipercent_error */
#define EPS 0.000005
        printf("Check ipercent_error for a few givens... ");
        float e1 = 5., e2 = 31.307903570339, e3 = 5.358, e4 = 194.642;
        assert(ifast_epsilon_cmp( ipercent_error(95, 100), e1, EPS) == 0);
        assert(ifast_epsilon_cmp( ipercent_error(675.312, 983.1), e2, EPS) == 0);
        assert(ifast_epsilon_cmp( ipercent_error(-47.321, -50), e3, EPS) == 0);
        assert(ifast_epsilon_cmp( ipercent_error(-47.321, 50), e4, EPS) == 0);
        assert(ifast_epsilon_cmp( ipercent_error(47.321, -50), e4, EPS) == 0);
        printf(KGRN "Success.\n" KWHT);
#undef EPS

        /* move on to tests of ifast_bigpow */
#define ERR ipercent_error
#define POW ifast_bigpow
#define EPS 4.763f
        printf("Making sure that ifast_bigpow has at most 4.763%% error on selected computations... ");
        float b1 = 0.75, b2 = -33.25, b3 = 0, b4 = 27.5; // test bases
        float p1 = 0, p2 = -0.5, p3 = -3.2, p4 = 5.125;   // test powers
        float bnp1 = 1,
              b1p2 = 1.15470053838, b1p3 = 2.51075280841, b1p4 = 0.22892276082,
              b3pn = 0,
              b4p2 = 0.19069251784, b4p3 = 0.00002478194, b4p4 = 23800169.8209;  // expected values
        assert(ERR(POW(b1, p1), bnp1)<EPS); assert(ERR(POW(b2, p1), bnp1)<EPS); assert(ERR(POW(b3, p1), bnp1)<EPS); // all computations involving
        assert(ERR(POW(b4, p1), bnp1)< EPS);                                                                     // p1
        assert(ERR(POW(b1, p2), b1p2)<EPS); assert(ERR(POW(b1, p3), b1p3)<EPS); assert(ERR(POW(b1, p4), b1p4)<EPS); // b1 foreach p 2..4
        /* b2 for p2 is impossible */      /* b2 for p3 is also impossible */ /* b2 for p4 is impossible, too */
        assert(ERR(POW(b3, p2), b3pn)<EPS); assert(ERR(POW(b3, p3), b3pn)<EPS); assert(ERR(POW(b3, p4), b3pn)<EPS); // b3 foreach p 2..4
        assert(ERR(POW(b4, p2), b4p2)<EPS); assert(ERR(POW(b4, p3), b4p3)<EPS); assert(ERR(POW(b4, p4), b4p4)<EPS); // b4 foreach p 2..4
        float sum = ERR(POW(b1, p1), bnp1) + ERR(POW(b2, p1), bnp1) + ERR(POW(b3, p1), bnp1) + ERR(POW(b4, p1), bnp1);
              sum+= ERR(POW(b1, p2), b1p2) + ERR(POW(b1, p3), b1p3) + ERR(POW(b1, p4), b1p4);
              sum+= ERR(POW(b3, p2), b3pn) + ERR(POW(b3, p3), b3pn) + ERR(POW(b3, p4), b3pn);
              sum+= ERR(POW(b4, p2), b4p2) + ERR(POW(b4, p3), b4p3) + ERR(POW(b4, p4), b4p4);
        sum = sum / 13.0f;
        printf(KGRN "Success. Average percent error = %.3f%%\n" KWHT, sum);

#undef ERR
#undef POW
#undef EPS

        /* move on to tests of ifast_smallpow */
#define ERR ipercent_error
#define POW ifast_smallpow
#define EPS 5.1f
        printf("Making sure that ifast_smallpow has at most 5.1%% error on selected computations... ");
        //float b1 = 0.75, b2 = -33.25, b3 = 0, b4 = 27.5; // test bases
        //float p1 = 0, p2 = -0.5, p3 = -3.2, p4 = 5.125;   // test powers
        assert(ERR(POW(64, 0.5f), 8)<EPS); assert(ERR(POW(125, 0.33333333), 5)<EPS); // all computations involving
        assert(ERR(POW(8, 0.333333f), 2.0f)<EPS); assert(ERR(POW(1024, 0.2), 4)<EPS);
        //assert(ERR(POW(b4, p1), bnp1)< EPS);                                                                     // p1
        //assert(ERR(POW(b1, p2), b1p2)<EPS); assert(ERR(POW(b1, p3), b1p3)<EPS); assert(ERR(POW(b1, p4), b1p4)<EPS); // b1 foreach p 2..4
        sum = ERR(POW(64, 0.5f), 8.0f) + ERR(POW(125, 0.33333333), 5.0f) + ERR(POW(8, 0.333333f), 2.0f) + ERR(POW(1024, 0.2f), 4.0f);
        printf(KGRN "Success. Average percent error = %.3f%%\n" KWHT, sum / 4);

#undef ERR
#undef POW
#undef EPS

        /* move on to tests of ifast_sqrt */
#define ERR ipercent_error
#define SQRT ifast_sqrt
#define EPS 2.0f
        printf("Making sure that ifast_sqrt has at most %f%% error on select computations... ", EPS);
        assert(ERR(SQRT(9), 3.0f) < EPS); assert(ERR(SQRT(81), 9.0f) < EPS); assert(ERR(SQRT(2), 1.41421356237f) < EPS);
        assert(ERR(SQRT(25), 5.0f) < EPS); assert(ERR(SQRT(36), 6.0f) < EPS); assert(ERR(SQRT(10000), 100.0f) < EPS);
              sum = ERR(SQRT(9), 3.0f) + ERR(SQRT(81), 9.0f) + ERR(SQRT(2), 1.4421356237f);
              sum+= ERR(SQRT(25), 5.0f) + ERR(SQRT(36), 6.0f) + ERR(SQRT(10000), 100.0f);
        sum /= 6.0f;
        printf(KGRN "Success. Average percent error = %.3f%%\n" KWHT, sum);
#undef ERR
#undef SQRT
#undef EPS

        /* move on to tests of inabs */
        printf("Checking to see if inabs works... ");
        assert(inabs(0) == 0); assert(inabs(1)==1); assert(inabs(-37)==37);
        printf(KGRN "Success.\n" KWHT);

        /* move on to tests of ifabs */
        printf("Checking to see if ifabs works... ");
        e1 = 37.5, e2 = 894.0883, e3 = 0.0;
        assert(ifabs(37.5) == e1); assert(ifabs(-894.0883) == e2); assert(ifabs(0) == 0);
        printf(KGRN "Success.\n" KWHT);

        /* start with tests of icoordinates */
        struct ipoint_t *point1;
        struct ipoint_t *point2;
        struct ipoint_t *point3;

        point1 = inew_point(627.075, -51.4, 33.0, NULL);
        point2 = inew_point(1000.0, -0.5, 6.0, NULL);
        point3 = inew_point(-43.675, 12.0, 0.0, NULL);

        printf("Making sure that inew_point does not assign conflicting addresses... ");
        assert( point1 != point2); assert( point2 != point3); assert( point1 != point3);
        printf(KGRN "Success.\n" KWHT);

        float x1 = 627.075, y1 = -51.4,
              x2 = 1000.,   y2 = -0.5,
              x3 = -43.675, y3 = 12.;
        printf("Making sure that inew_point properly assigns accessible values, and");
        printf(" that that ipoint_t structure is functional... ");
        assert(point1->x == x1); assert(point1->y == y1); assert(point1->z == 33);
        assert(point2->x == x2); assert(point2->y == y2); assert(point2->z == 6);
        assert(point3->x == x3); assert(point3->y == y3); assert(point3->z == 0);

        free(point1);
        free(point2);
        free(point3);

        printf(KGRN "Success.\n" KWHT);

        /* tests of linked lists */
        printf(KWHT "Doing tests of ilinked_t... ");
        struct ilinked_t *lp1 = NULL, *lp2 = NULL, *lp3 = NULL;
        int *arr1 = malloc(10 * sizeof(int));
        arr1[8] = -3; arr1[1] = -2; arr1[5] = -1; arr1[9] = 0; arr1[6] = 1; arr1[2] = 2; arr1[4] = 3; arr1[7] = 4; arr1[0] = 5; arr1[3] = 6;
        int *arr2 = malloc(10 * sizeof(int));
        arr2[7] = -4; arr2[1] = -3; arr2[2] = -2; arr2[3] = -1;arr2[9] = 0; arr2[5] = 1; arr2[6] = 2; arr2[0] = 3; arr2[8] = 4; arr2[4] = 5;

        struct test_struct { int x; int y; int z; };
        struct test_struct str1;
        str1.x = 0; str1.y = 1; str1.z = 2;

        assert(lp1 == NULL); // make sure that lp1 doesn't already somehow point to something
        lp1 = inew_linked(arr1); // initialize lp1
        assert(lp1 != NULL); // make sure that lp1 points to something
        assert(lp1->previous == NULL); assert(lp1->next == NULL); // make sure the next/prev pointers
                                                                  // have been NULLed
        assert(((int*)(lp1->data))[0] == 5); assert(((int*)(lp1->data))[1] == -2); // make sure that the
                                                                                   // data points to the
                                                                                   // right array
        
        assert(lp2 == NULL); // make sure lp2 is empty before we start
        lp3 = NULL; lp3 = inew_linked(&str1); // make sure lp3 is empty, then make a new linked node out of it
        lp2 = iadd_linked(NULL, lp3); // this should just make lp3 into the primary list.
        assert(lp2 != NULL); assert(lp2 == lp3); lp3 = NULL; // make sure lp2 isn't NULL, then make sure it's lp3, then
                                                             // null out lp3 for later use
        assert(lp2->previous == NULL); assert(lp2->next == NULL); // make sure next and previous are nulled

                        // make sure values copied over properly.
        assert(((struct test_struct*)(lp2->data))->x == 0); assert(((struct test_struct*)(lp2->data))->y == 1);
        assert(((struct test_struct*)(lp2->data))->z == 2); assert(lp2->data == &str1);

        lp3 = lp1; // set lp3 to lp1 (tmp)
        lp1 = iadd_linked(lp1, lp2); // lp1->next=lp2
        assert(lp3 == lp1); lp3 = NULL; // make sure lp1 hasn't changed, then null out lp3 for later
        assert(lp1->previous == NULL); assert(lp1->next == lp2); assert(lp2->previous == lp1); // make sure structure checks out
        assert(iadd_linked(NULL, lp2) == NULL); assert(lp1->next == lp2); assert(lp2->previous == lp1); // make sure that adding lp2
                                                                                                        // to the NULL node doesn't
                                                                                                        // misbehave.
        assert(iadd_linked(lp1, NULL) == lp1); assert(lp2->previous == lp1); assert(lp1->next == lp2); // make sute that adding the
                                                                                                        // NULL node to lp1 doesn't
                                                                                                        // misbehave.

        lp3 = inew_linked(arr2);
        iadd_linked(lp1, lp3); /// 1 2 3
        assert(iget_linked(lp1, 7) == NULL); assert(iget_linked(lp2, 2) == lp3);
        assert(iremove_linked(lp1) == lp2); /// 2 3
        iadd_linked(lp3, lp1 = inew_linked(arr1)); /// 2 3 1
        assert(iremove_linked(lp3) == lp1); /// 2 1
        iadd_linked(lp1, lp3 = inew_linked(arr2)); /// 2 1 3
        assert(iremove_linked(lp1) == lp3); /// 2 3
        lp1 = inew_linked(arr1);
        lp1 = iadd_linked(lp1, lp2);

        assert(lp1->next == lp2); assert(lp2->next == lp3); assert(lp3->next == NULL);
        assert(lp1->previous == NULL); assert(lp2->previous == lp1); assert(lp3->previous == lp2);

        assert(iset_linked(lp1, 0, arr2) == lp1); assert(iget_linked(lp1, 0)->data == arr2);
        assert(iset_linked(lp2, 0, arr1) == lp1); assert(iget_linked(lp2, 0)->data == arr1);
        assert(iset_linked(lp1, 1, arr2) == lp2); assert(lp2->data == arr2);
        assert(iset_linked(lp2, 1, &str1) == lp2); assert(lp2->data == &str1);

        assert(igetfirst_linked(lp1) == lp1); assert(igetfirst_linked(lp1) == igetfirst_linked(lp2));
        assert(igetfirst_linked(NULL) == NULL);

        assert(igetlast_linked(lp1) == lp3); assert(igetlast_linked(lp1) == igetlast_linked(lp2));
        assert(igetlast_linked(NULL) == NULL);

        assert(igetnext_linked(lp1) == lp2); assert(igetnext_linked(lp3) == NULL);
        assert(igetnext_linked(NULL) == NULL);

        assert(lp1->next = lp2); assert(lp2->next = lp3); assert(lp3->next == NULL);

        ifree_linked(lp1, 'n');

        printf(KGRN "Success.\n" KWHT);

        /* test inmin and inmax functions */
        printf(KWHT "Testing inmin and inmax functions... ");

        assert(inmin(arr1, 10) == -3); assert(inmin(arr2, 10) == -4);
        assert(inmax(arr1, 10) ==  6); assert(inmax(arr2, 10) ==  5);

        printf(KGRN "Success.\n" KWHT);

        /* test the region system */
        printf(KWHT "Testing the new region system and the frontend state system as a bonus... ");

        struct ifrontendstate_t *state = malloc(sizeof(struct ifrontendstate_t));
        state->num_regions = 0;
        state->regions =  NULL;

        assert(DEFAULT_REGION_WIDTH >= 50); assert(DEFAULT_REGION_HEIGHT >= 50);
        point1 = inew_point(13.0, 23.0, 4, state);
        
        assert(state->num_regions == 1);
        assert(point1->region == ((struct iregion_t*)(state->regions->data))->id);
        assert(point1->x == 13.0); assert(point1->y == 23.0); assert(point1->z == 4);

        point2 = inew_point(DEFAULT_REGION_WIDTH + 13.0, DEFAULT_REGION_HEIGHT + 23.0, 4, state);
        
        assert(state->num_regions == 2);
        assert(point2->region == ((struct iregion_t*)(state->regions->next->data))->id);
        assert(point2->x == 13.0); assert(point2->y == 23.0); assert(point2->z == 4);

        point3 = inew_point(-DEFAULT_REGION_WIDTH + 13.0, -DEFAULT_REGION_HEIGHT + 23.0, 4, state);
        
        assert(state->num_regions == 3);
        assert(point3->region == ((struct iregion_t*)(state->regions->next->next->data))->id);
        assert(point3->x == 13.0); assert(point3->y == 23.0); assert(point3->z == 4);

        free(point1); free(point2); free(point3);

        printf(KGRN "Success.\n" KWHT);

        printf(RESET);

        free(arr1);
        free(arr2);
        return 0;
}

void iassert(int val)
{
        
}

void iassert_setmsg(char *msg)
{
        
}
