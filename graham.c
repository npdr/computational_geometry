/*
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 3.  It is not written to be comprehensible without the
explanation in that book.

Input: 2n integer coordinates of points in the plane.
Output: the convex hull, cw, in PostScript; other output precedes the PS.

NB: The original array storing the points is overwritten.

Compile: gcc -o graham graham.c  (or simply: make)

Written by Joseph O'Rourke.
Last modified: October 1997
Questions to orourke@cs.smith.edu.
--------------------------------------------------------------------
This code is Copyright 1998 by Joseph O'Rourke.  It may be freely
redistributed in its entirety provided that this copyright notice is
not removed.
--------------------------------------------------------------------
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

#define EXIT_FAILURE 1
#define X 0
#define Y 1
typedef enum { FALSE, TRUE } bool;

#define DIM 2             /* Dimension of points */
typedef int tPointi[DIM]; /* Type integer point */

/*----------Point(s) Structure-------------*/
typedef struct tPointStructure tsPoint;
typedef tsPoint *tPoint;
struct tPointStructure {
    int vnum;
    tPointi v;
    bool delete;
};
/* Global variables */
#define PMAX 1000 /* Max # of points */
typedef tsPoint tPointArray[PMAX];
static tPointArray P;
int n = 0;       /* Actual # of points */
int ndelete = 0; /* Number deleted */

/*----------Stack Structure-------------*/
typedef struct tStackCell tsStack; /* Used on in NEW() */
typedef tsStack *tStack;
struct tStackCell {
    tPoint p;
    tStack next;
};

/*----------Function Prototypes-------------*/
tStack Pop(tStack s);
void PrintStack(tStack t);
tStack Push(tPoint p, tStack top);
tStack Graham(void);
void Squash(void);
void Copy(int i, int j);
void PrintPostscript(tStack t);
int Compare(const void *tp1, const void *tp2);
void FindLowest(void);
void Swap(int i, int j);

int AreaSign(tPointi a, tPointi b, tPointi c);
bool Left(tPointi a, tPointi b, tPointi c);
int ReadPoints(void);
void PrintPoints(void);

int main() {
    tStack top;

    n = ReadPoints();
    FindLowest();
    qsort(&P[1],           /* pointer to 1st elem */
          n - 1,           /* number of elems */
          sizeof(tsPoint), /* size of each elem */
          Compare          /* -1,0,+1 compare function */
    );

    top = Graham();
    printf("Hull:\n");
    PrintStack(top);
    return 0;
}

/*---------------------------------------------------------------------
FindLowest finds the rightmost lowest point and swaps with 0-th.
The lowest point has the min y-coord, and amongst those, the
max x-coord: so it is rightmost among the lowest.
---------------------------------------------------------------------*/
void FindLowest(void) {
    int i;
    int m = 0; /* Index of lowest so far. */

    for (i = 1; i < n; i++)
        if ((P[i].v[Y] < P[m].v[Y]) ||
            ((P[i].v[Y] == P[m].v[Y]) && (P[i].v[X] > P[m].v[X])))
            m = i;
    Swap(0, m); /* Swap P[0] and P[m] */
}

void Swap(int i, int j) {
    int temp;
    /* Uses swap macro. */

    SWAP(temp, P[i].vnum, P[j].vnum);
    SWAP(temp, P[i].v[X], P[j].v[X]);
    SWAP(temp, P[i].v[Y], P[j].v[Y]);
    SWAP(temp, P[i].delete, P[j].delete);
}
/*---------------------------------------------------------------------
Compare: returns -1,0,+1 if p1 < p2, =, or > respectively;
here "<" means smaller angle.  Follows the conventions of qsort.
---------------------------------------------------------------------*/
int Compare(const void *tpi, const void *tpj) {
    int a;    /* area */
    int x, y; /* projections of ri & rj in 1st quadrant */
    tPoint pi, pj;
    pi = (tPoint)tpi;
    pj = (tPoint)tpj;

    a = AreaSign(P[0].v, pi->v, pj->v);
    if (a > 0)
        return -1;
    else if (a < 0)
        return 1;
    else { /* Collinear with P[0] */
        x = abs(pi->v[X] - P[0].v[X]) - abs(pj->v[X] - P[0].v[X]);
        y = abs(pi->v[Y] - P[0].v[Y]) - abs(pj->v[Y] - P[0].v[Y]);

        ndelete++;
        if ((x < 0) || (y < 0)) {
            pi->delete = TRUE;
            return -1;
        } else if ((x > 0) || (y > 0)) {
            pj->delete = TRUE;
            return 1;
        } else { /* points are coincident */
            if (pi->vnum > pj->vnum)
                pj->delete = TRUE;
            else
                pi->delete = TRUE;
            return 0;
        }
    }
}

/*---------------------------------------------------------------------
Pops off top elment of stack s, frees up the cell, and returns new top.
---------------------------------------------------------------------*/
tStack Pop(tStack s) {
    tStack top;

    top = s->next;
    FREE(s);
    return top;
}

/*---------------------------------------------------------------------
Get a new cell, fill it with p, and push it onto the stack.
Return pointer to new stack top.
---------------------------------------------------------------------*/
tStack Push(tPoint p, tStack top) {
    tStack s;

    /* Get new cell and fill it with point. */
    NEW(s, tsStack);
    s->p = p;
    s->next = top;
    return s;
}
/*---------------------------------------------------------------------
---------------------------------------------------------------------*/
void PrintStack(tStack t) {
    if (!t) printf("Empty stack\n");
    while (t) {
        printf("vnum=%d\tx=%d\ty=%d\n", t->p->vnum, t->p->v[X], t->p->v[Y]);
        t = t->next;
    }
}

/*---------------------------------------------------------------------
Performs the Graham scan on an array of angularly sorted points P.
---------------------------------------------------------------------*/
tStack Graham() {
    tStack top;
    int i;
    tPoint p1, p2; /* Top two points on stack. */

    /* Initialize stack. */
    top = NULL;
    top = Push(&P[0], top);
    top = Push(&P[1], top);

    /* Bottom two elements will never be removed. */
    i = 2;

    while (i < n) {
        if (!top->next) printf("Error\n"), exit(EXIT_FAILURE);
        p1 = top->next->p;
        p2 = top->p;
        if (Left(p1->v, p2->v, P[i].v)) {
            top = Push(&P[i], top);
            i++;
        } else
            top = Pop(top);
    }

    return top;
}

/*---------------------------------------------------------------------
Squash removes all elements from P marked delete.
---------------------------------------------------------------------*/
void Squash(void) {
    int i, j;
    i = 0;
    j = 0;
    while (i < n) {
        if (!P[i].delete) { /* if not marked for deletion */
            Copy(i, j);     /* Copy P[i] to P[j]. */
            j++;
        }
        i++;
    }
    n = j;
    printf("After Squash: n=%d\n", n);
    PrintPoints();
}

void Copy(int i, int j) {
    P[j].v[X] = P[i].v[X];
    P[j].v[Y] = P[i].v[Y];
    P[j].vnum = P[i].vnum;
    P[j].delete = P[i].delete;
}

/*---------------------------------------------------------------------
Returns twice the signed area of the triangle determined by a,b,c.
The area is positive if a,b,c are oriented ccw, negative if cw,
and zero if the points are collinear.
---------------------------------------------------------------------*/
int Area2(tPointi a, tPointi b, tPointi c) {
    // printf("Error: Area2 called, no longer used.\n");
    return (b[X] - a[X]) * (c[Y] - a[Y]) - (c[X] - a[X]) * (b[Y] - a[Y]);
}

/*---------------------------------------------------------------------
Returns true iff c is strictly to the left of the directed
line through a to b.
---------------------------------------------------------------------*/
bool Left(tPointi a, tPointi b, tPointi c) {
    /*return  Area2( a, b, c ) > 0;*/
    return AreaSign(a, b, c) > 0;
}

/*---------------------------------------------------------------------
Reads in the coordinates of the points from stdin,
puts them into P, and returns n, the number of vertices.
Initializes other fields of point structure.
---------------------------------------------------------------------*/
int ReadPoints(void) {
    int n = 0;

    while ((n < PMAX) && (scanf("%d %d", &P[n].v[0], &P[n].v[1]) != EOF)) {
        P[n].vnum = n;
        P[n].delete = FALSE;
        ++n;
    }
    if (n < PMAX)
        return n;
    else {
        printf("Error in ReadPoints:  too many points; max is %d\n", PMAX);
        exit(EXIT_FAILURE);
    }

    return n;
}

void PrintPoints(void) {
    int i;

    for (i = 0; i < n; i++)
        printf("vnum=%3d, x=%4d, y=%4d, delete=%d\n", P[i].vnum, P[i].v[X],
               P[i].v[Y], P[i].delete);
}

int AreaSign(tPointi a, tPointi b, tPointi c) {
    double area2;

    area2 = (b[0] - a[0]) * (double)(c[1] - a[1]) -
            (c[0] - a[0]) * (double)(b[1] - a[1]);

    /* The area should be an integer. */
    if (area2 > 0.5)
        return 1;
    else if (area2 < -0.5)
        return -1;
    else
        return 0;
}

/*---------------------------------------------------------------------
Bresenham algorithm for modifying O'Rourke's Graham scan
---------------------------------------------------------------------*/
void bresenham(tStack top, tPointi p1, tPointi p2) {
    int dx, dy, p, x, y;

    /* Case zero */
    //for vertical lines
    
    /* First case */
    if(p2[X] >= p1[X] && p2[Y] >= p1[Y]) {
        dx = p2[X] - p1[Y];
        dy = p2[Y] - p1[Y];

        tPointi point = p1;

        p = 2 * dy - dx;

        while (point[X] <= p2[X]) {
            Push(point, top);
            x++;
            if (p < 0) {
                p = p + 2 * dy;
            } else {
                p = p + 2 * dy - 2 * dx;
                y++;
            }
        }
    }

    /* Second case */
    if(p2[X] >= p1[X] && p2[Y] <= p1[Y]) {
        dx = p2[X] - p1[Y];
        dy = p2[Y] - p1[Y];

        tPointi point = p1;

        p = 2 * dy - dx;

        while (point[X] <= p2[X]) {
            Push(point, top);
            x++;
            if (p < 0) {
                p = p + 2 * dy;
            } else {
                p = p + 2 * dy - 2 * dx;
                y++;
            }
        }
    }

    /* Third case */
    if(p2[X] <= p1[X] && p2[Y] >= p1[Y]) {
        dx = p2[X] - p1[Y];
        dy = p2[Y] - p1[Y];

        tPointi point = p1;

        p = 2 * dy - dx;

        while (point[X] <= p2[X]) {
            Push(point, top);
            x++;
            if (p < 0) {
                p = p + 2 * dy;
            } else {
                p = p + 2 * dy - 2 * dx;
                y++;
            }
        }
    }

    /* Fourth case */
    if(p2[X] <= p1[X] && p2[Y] <= p1[Y]) {
        dx = p2[X] - p1[Y];
        dy = p2[Y] - p1[Y];

        tPointi point = p1;

        p = 2 * dy - dx;

        while (point[X] <= p2[X]) {
            Push(point, top);
            x++;
            if (p < 0) {
                p = p + 2 * dy;
            } else {
                p = p + 2 * dy - 2 * dx;
                y++;
            }
        }
    }
}
