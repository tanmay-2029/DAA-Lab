#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int flip(double p) {
    double r = (double)rand() / RAND_MAX;
    return (r<p)?1:0;
}

void simulate(const char *name, double prob, long tosses, const char *filename) {
    long heads = 0;
    FILE *f = fopen("fair_coin.csv", "w");
    if(f==NULL) f=stdout;

    fprintf(f, "toss,p_head\n");

    // don't log every single toss if we're doing a million
    long step = tosses / 2000;
    if (step < 1) step = 1;

    for (long i = 1; i <= tosses; i++) {
        heads += flip(prob);
        if (i % step == 0 || i == tosses) {
            fprintf(f, "%ld,%.6f\n", i, (double)heads / i);
        }
    }
    fclose(f);

    printf("%s: %ld tosses, observed p = %.4f (expected = %.2f)\n", 
           name, tosses, (double)heads / tosses, prob);
}

int main(int argc, char **argv) {
    long n = 100000;
    double bias = 0.7;

    if (argc > 1) n = atol(argv[1]);
    if (argc > 2) bias = atof(argv[2]);

    srand(time(NULL));

    printf("Starting simulation...\n");
    simulate("Fair", 0.5, n, "fair.csv");
    simulate("Biased", bias, n, "biased.csv");
    printf("Done. Run the python script to plot.\n");

    return 0;
}
