//
// Created by Jim on 2/8/21.
//

#include "z2_3D.h"

/* Z_2 lattice gauge simulation */
/* Michael Creutz <creutz@bnl.gov>     */
/* http://thy.phy.bnl.gov/~creutz/z2.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* the lattice is of dimensions SIZE**4  */
#define SIZE 3
int link_3d[SIZE][SIZE][SIZE][4]; /* last index gives link direction */

/* utility functions */
void moveup_3d(int x[], int d) {
    x[d] += 1;
    if (x[d] >= SIZE) x[d] -= SIZE;
    return;
}

void movedown_3d(int x[], int d) {
    x[d] -= 1;
    if (x[d] < 0) x[d] += SIZE;
    return;
}

void coldstart_3d() {  /* set all links to unity */
    int x[4], d;
    for (x[0] = 0; x[0] < SIZE; x[0]++)
        for (x[1] = 0; x[1] < SIZE; x[1]++)
            for (x[2] = 0; x[2] < SIZE; x[2]++)
                for (d = 0; d < 3; d++)
                    link_3d[x[0]][x[1]][x[2]][d] = 1;
    return;
}
/* for a random start: call coldstart() and then update once at beta=0 */

/* do a Monte Carlo sweep; return energy */
double update_3d(double beta) {
    int x[4], d, dperp, staple, staplesum;
    double bplus, bminus, action = 0.0;
    for (x[0] = 0; x[0] < SIZE; x[0]++)
        for (x[1] = 0; x[1] < SIZE; x[1]++)
            for (x[2] = 0; x[2] < SIZE; x[2]++)
                for (d = 0; d < 3; d++) {
                    staplesum = 0;
                    for (dperp = 0; dperp < 3; dperp++) {
                        if (dperp != d) {
                            /*  move around thusly:
                                dperp        6--5
                                ^            |  |
                                |            1--4
                                |            |  |
                                -----> d     2--3  */
                            /* plaquette 1234 */
                            movedown_3d(x, dperp);
                            staple = link_3d[x[0]][x[1]][x[2]][dperp]
                                     * link_3d[x[0]][x[1]][x[2]][d];
                            moveup_3d(x, d);
                            staple *= link_3d[x[0]][x[1]][x[2]][dperp];
                            moveup_3d(x, dperp);
                            staplesum += staple;
                            /* plaquette 1456 */
                            staple = link_3d[x[0]][x[1]][x[2]][dperp];
                            moveup_3d(x, dperp);
                            movedown_3d(x, d);
                            staple *= link_3d[x[0]][x[1]][x[2]][d];
                            movedown_3d(x, dperp);
                            staple *= link_3d[x[0]][x[1]][x[2]][dperp];
                            staplesum += staple;
                        }
                    }
                    /* calculate the Boltzmann weight */
                    bplus = exp(beta * staplesum);
                    bminus = 1 / bplus;
                    bplus = bplus / (bplus + bminus);
                    /* the heatbath algorithm */
                    if (drand48() < bplus) {
                        link_3d[x[0]][x[1]][x[2]][d] = 1;
                        action += staplesum;
                    } else {
                        link_3d[x[0]][x[1]][x[2]][d] = -1;
                        action -= staplesum;
                    }
                }
    action /= (SIZE * SIZE * SIZE * 4 * 6);
    return 1. - action;
}