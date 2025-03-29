#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Quicksort(int *V, int inf, int sup);
    
    int Particao(int *V, int inf, int sup);
    
    int main() {
        int N = 100000000;
        int *V = (int *) malloc(N * sizeof(int));
        int i;
        srand(time(NULL));

        for (i = 0; i < N; i++) {

            V[i] = rand() % 1000;

            clock_t T1 = clock();
            Quicksort(V, 0, N - 1);
            clock_t T2 = clock();

            printf("Tempo de execucao: %lf\n", (double)(T2 - T1) / CLOCKS_PER_SEC);V[i] = rand() % 100;
        }
    }

void Quicksort(int *V, int inf, int sup) {
    if (inf < sup) {
       int P = Particao(V, inf, sup);
       Quicksort(V, inf, P - 1);
         Quicksort(V, P, sup); 
    }
}

int Particao(int *V, int inf, int sup) {
    
    int pivot = V[(inf + sup) / 2];
    int i = inf;
    int j = sup;

    while (i <= j) {
        while (V[i] < pivot) i++;
        while (V[j] > pivot) j--;

        if (i <= j) {
            int temp = V[i];
            V[i] = V[j];
            V[j] = temp;
            i++;
            j--;
        }
    }
}