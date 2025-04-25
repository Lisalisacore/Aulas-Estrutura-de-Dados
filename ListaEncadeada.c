#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Item {
    int chave;
    struct Item* proximo;
} Item;

typedef struct {
    Item* Array;
    int tamanho;
    int capacidade;
} Lista;

int main() {
    
}