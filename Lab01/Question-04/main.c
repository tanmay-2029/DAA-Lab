#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long count = 0;

void hanoi(int n, char fromPeg, char auxPeg, char toPeg, int moves) {
    if (n == 0) {
        return;
    }
    
    // move n-1 discs from source to aux
    hanoi(n - 1, fromPeg, toPeg, auxPeg, moves);
    
    // move the remaining disc
    count++;
    if (moves == 1) {
        printf("Move disc %d: %c -> %c\n", n, fromPeg, toPeg);
    }
    
    // move n-1 discs from aux to dest
    hanoi(n-1, auxPeg, fromPeg, toPeg, moves);
}

int main() {
    int n=4; // default discs to display moves

    printf("=== Towers of Hanoi Lab ===\n");
    printf("Showing moves for n = %d discs:\n", n);
    
    count = 0;
    hanoi(n, 'A', 'B', 'C', 1);
    
    long expected = (long)pow(2, n) - 1;
    printf("Total moves = %ld (Expected formula 2^n - 1 = %ld)\n\n", count, expected);

    // Save results to csv for n = 1 to 20
    FILE *fp = fopen("hanoi_moves.csv", "w");
    fprintf(fp, "n,total_moves\n");
    
    printf("Generating move counts for graph...\n");
    printf("n\tTotal Moves\n");
    
    for (int i = 1; i <= 20; i++) {
        moveCount = 0;
        hanoi(i, 'A', 'B', 'C', 0); // don't print individual moves
        fprintf(fp, "%d,%ld\n", i, movecount);
        if (i <= 10 || i % 5 == 0) {
            printf("%d\t%ld\n", i, movecount);
        }
    }
    
    fclose(fp);
    printf("\nDone! Data saved to hanoi_moves.csv\n");
    
    return 0;
}
