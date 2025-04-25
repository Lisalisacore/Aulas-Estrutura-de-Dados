#include "cidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Estrutura para representar uma cidade
typedef struct Cidade {
    int Xi;
    char Si[100];
    int vizinhanca;
    struct Cidade *prox;
} Cidade;

// Estrutura para representar a estrada
typedef struct Estrada {
    int T;
    int N;
    Cidade *cidades;
} Estrada;

// Função para criar uma nova cidade
Cidade *criarCidade(int Xi, const char *Si) {
    Cidade *nova = (Cidade *)malloc(sizeof(Cidade));
    if (!nova) {
        printf("Erro ao alocar memória para a cidade.\n");
        return NULL;
    }
    nova->Xi = Xi;
    strcpy(nova->Si, Si);
    nova->vizinhanca = 0;
    nova->prox = NULL;
    return nova;
}

// Função para inserir uma cidade na lista encadeada
void inserirCidade(Estrada *estrada, Cidade *nova) {
    if (!estrada->cidades) {
        estrada->cidades = nova;
    } else {
        Cidade *atual = estrada->cidades;
        while (atual->prox) {
            atual = atual->prox;
        }
        atual->prox = nova;
    }
}

// Função para calcular as vizinhanças de cada cidade
void calcularVizinhanças(Estrada *estrada) {
    Cidade *atual = estrada->cidades;
    while (atual) {
        Cidade *anterior = NULL, *proxima = NULL;
        int menorDistEsq = INT_MAX, menorDistDir = INT_MAX;

        // Encontrar a cidade anterior
        Cidade *temp = estrada->cidades;
        while (temp) {
            if (temp->Xi < atual->Xi && atual->Xi - temp->Xi < menorDistEsq) {
                menorDistEsq = atual->Xi - temp->Xi;
                anterior = temp;
            }
            temp = temp->prox;
        }

        // Encontrar a cidade próxima
        temp = estrada->cidades;
        while (temp) {
            if (temp->Xi > atual->Xi && temp->Xi - atual->Xi < menorDistDir) {
                menorDistDir = temp->Xi - atual->Xi;
                proxima = temp;
            }
            temp = temp->prox;
        }

        // Calcular a vizinhança
        int vizinhancaEsq = anterior ? menorDistEsq / 2 : atual->Xi;
        int vizinhancaDir = proxima ? menorDistDir / 2 : estrada->T - atual->Xi;
        atual->vizinhanca = vizinhancaEsq + vizinhancaDir;

        atual = atual->prox;
    }
}

// Função para encontrar a cidade com a menor vizinhança
Cidade *cidadeMenorVizinhança(Estrada *estrada) {
    Cidade *atual = estrada->cidades;
    Cidade *menor = atual;
    while (atual) {
        if (atual->vizinhanca < menor->vizinhanca) {
            menor = atual;
        }
        atual = atual->prox;
    }
    return menor;
}

// Função para inicializar a estrutura Estrada a partir de um arquivo
Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (!estrada) {
        printf("Erro ao alocar memória para a estrada.\n");
        fclose(arquivo);
        return NULL;
    }

    fscanf(arquivo, "%d", &estrada->T);
    fscanf(arquivo, "%d", &estrada->N);

    estrada->cidades = NULL;

    for (int i = 0; i < estrada->N; i++) {
        int Xi;
        char Si[100];
        fscanf(arquivo, "%d %s", &Xi, Si);
        Cidade *nova = criarCidade(Xi, Si);
        if (!nova) {
            fclose(arquivo);
            return NULL;
        }
    }

    fclose(arquivo);
    return estrada;
}