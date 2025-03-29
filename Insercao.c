#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Insercao(int *V, int N);
    int main() {
        int N = 100000;
        int *V = (int *) malloc(N * sizeof(int));
        int i;
        srand(time(NULL));

        for (i = 0; i < N; i++) V[i] = rand() % 1000; {

            clock_t T1 = clock();
            Insercao(V, N);
            clock_t T2 = clock();

            printf("Tempo de execucao: %lf\n", (double)(T2 - T1) / CLOCKS_PER_SEC);V[i] = rand() % 100;
        }
    }

void Insercao(int *V, int N) {
    int i, j;
    int Chave;

    for (i = 1; i < N; i++) {
        Chave = V[i];
        j = i - 1;

        while (j >= 0 && V[j] > Chave) {
            V[j + 1] = V[j];
            j--;
        }

        V[j + 1] = Chave;
    }
    
}