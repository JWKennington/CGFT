#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include "z2_example.h"

//int thermal_cycle() {
//    printf("Hello, World!\n");
//    return 0;
//}

int thermal_cycle() {
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
    memset(data_beta, -1, num_iters * sizeof(int));
    memset(data_action, -1, num_iters * sizeof(int));
    coldstart();

    /* heat it up */
    int index = 0;
    for (beta = beta_max; beta > beta_min; beta -= dbeta) {
        action = update(beta);

        /* Record data */
        // printf("%g\t%g\n", beta, action);
        data_beta[index] = beta;
        data_action[index] = action;
        index++;
    }

    /* cool it down */
    for (beta = beta_min; beta < beta_max; beta += dbeta) {
        action = update(beta);

        /* Record data */
        // printf("%g\t%g\n", beta, action);
        data_beta[index] = beta;
        data_action[index] = action;
        index++;
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


int phase_transition() {
    double beta, dbeta, action, beta_max, beta_min, beta_center, beta_width;
    int repeat, iter, beta_index, sample_freq, samples;
    srand48(9562L);  /* initialize random number generator */

    /* Set Beta boundaries */
    beta_center = 0.429;
    beta_width = 0.01;
    beta_min = beta_center - beta_width;
    beta_max = beta_center + beta_width;
    dbeta = 0.0001;
    repeat = 100000;
    sample_freq = 100000;
    samples = (int) repeat / sample_freq;

    /* Initialize variables related to data */
    int num_betas = (int) ((beta_max - beta_min) / dbeta) + 1;
    double data_beta[num_betas];
    double data_action[num_betas][samples];
    memset(data_beta, -1, num_betas * sizeof(double));
    memset(data_action, -1, num_betas * samples * sizeof(double));

    for (beta_index = 0; beta_index < num_betas; beta_index ++) {
        /* start from same conditions */
        beta = beta_min + (beta_index * dbeta);
        coldstart();
        action = 0;

        /* Evolve repeatedly at same beta */
        for (iter = 0; iter < repeat; iter += 1) {
            action = update(beta);

            /* Store value */
            if ((iter+1) % sample_freq == 0) {
//                printf("Iter stored: %d \n", iter);
                data_action[beta_index][(int) (iter / sample_freq)] = action;
            }
        }

        /* Record data */
        data_beta[beta_index] = beta;
    }

    /* Write data out to csv file
     *     Make sure to update the filename */
    printf("Writing Out Data\n");
    FILE *fptr;
    fptr = fopen("/Users/jim/repos/research/CGFT/data/phase-transition/fvl_size_5_iter_100000_freq_100000_beta_0429_001_00001_ens_01.csv", "w");
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    int n, i;
    for (n = 0; n < num_betas; n++) {
        for (i = 0; i < samples; i++) {
            fprintf(fptr, "%g,%d,%g\n", data_beta[n], i, data_action[n][i]);
        }
    }
    fclose(fptr);
    return 0;
}


int main() {
    /* Clock initial time */
    clock_t start, end;
    start = clock();

    /* Run Experiment */
    phase_transition();

    /* Clock initial time */
    end = clock();
    double time_taken = ((double) (end - start))/CLOCKS_PER_SEC; // in seconds

    printf("Experiment took %f seconds to execute.\n", time_taken);
}
