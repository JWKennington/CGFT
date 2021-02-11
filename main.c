#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "z2_example.h"

//int main() {
//    printf("Hello, World!\n");
//    return 0;
//}

int main() {
    double beta, dbeta, action, beta_max, beta_min;
    srand48(1234L);  /* initialize random number generator */

    /* Set Beta boundaries */
    beta_min = 0.0;
    beta_max = 1.0;
    dbeta = .001;

    /* Initialize variables related to data */
    int num_iters = (int) (2 * (beta_max - beta_min) / dbeta);
    double data_beta[num_iters];
    double data_action[num_iters];
    memset( data_beta, -1, num_iters*sizeof(int) );
    memset( data_action, -1, num_iters*sizeof(int) );
    coldstart();

    /* heat it up */
    int index = 0;
    for (beta = beta_max; beta > beta_min; beta -= dbeta) {
        action = update(beta);

        /* Record data */
        // printf("%g\t%g\n", beta, action);
        data_beta[index] = beta;
        data_action[index] = action;
        index ++;
    }

    /* cool it down */
    for (beta = beta_min; beta < beta_max; beta += dbeta) {
        action = update(beta);

        /* Record data */
        // printf("%g\t%g\n", beta, action);
        data_beta[index] = beta;
        data_action[index] = action;
        index ++;
    }

    /* Write data out to csv file */
    FILE *fptr;
    fptr = fopen("/Users/jim/repos/research/CGFT/data.csv", "w");
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    int n;
    for (n = 0; n < index + 1; n++) {
        fprintf(fptr, "%.3f,%.7f\n", data_beta[n], data_action[n]);
    }
    fclose(fptr);
    return 0;
}
