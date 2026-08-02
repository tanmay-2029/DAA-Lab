/*
 * Name: Alex Chen
 * Roll No: 10240391
 * Lab Assignment 4: Towers of Hanoi Simulation
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long moveCount = 0;

// Recursive function for Towers of Hanoi
void solveHanoi(int n, char fromPeg, char auxPeg, char toPeg, int showMoves) {
    if (n == 0) {
        return;
    }
    
    // Move n-1 discs from source to aux
    solveHanoi(n - 1, fromPeg, toPeg, auxPeg, showMoves);
    
    // Move the remaining disc
    moveCount++;
    if (showMoves == 1) {
        printf("Move disc %d: %c -> %c\n", n, fromPeg, toPeg);
    }
    
    // Move n-1 discs from aux to dest
    solveHanoi(n - 1, auxPeg, fromPeg, toPeg, showMoves);
}

int main() {
    int n = 4; // default discs to display moves

    printf("=== Towers of Hanoi Lab ===\n");
    printf("Showing moves for n = %d discs:\n", n);
    
    moveCount = 0;
    solveHanoi(n, 'A', 'B', 'C', 1);
    
    long expected = (long)pow(2, n) - 1;
    printf("Total moves = %ld (Expected formula 2^n - 1 = %ld)\n\n", moveCount, expected);

    // Save results to CSV for n = 1 to 20
    FILE *fp = fopen("hanoi_moves.csv", "w");
    fprintf(fp, "n,total_moves\n");
    
    printf("Generating move counts for graph...\n");
    printf("n\tTotal Moves\n");
    
    for (int i = 1; i <= 20; i++) {
        moveCount = 0;
        solveHanoi(i, 'A', 'B', 'C', 0); // don't print individual moves
        fprintf(fp, "%d,%ld\n", i, moveCount);
        if (i <= 10 || i % 5 == 0) {
            printf("%d\t%ld\n", i, moveCount);
        }
    }
    
    fclose(fp);
    printf("\nDone! Data saved to hanoi_moves.csv\n");
    
    return 0;
}
