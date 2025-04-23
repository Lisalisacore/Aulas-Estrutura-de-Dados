#include "cidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

// Função para inicializar a estrutura Estrada a partir de um arquivo
Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (estrada == NULL) {
        printf("Erro ao alocar memória para a estrada.\n");
        fclose(arquivo);
        return NULL;
    }

    // Ler o comprimento total da estrada
    fscanf(arquivo, "%d", &estrada->T);
    if (estrada->T < 3 || estrada->T > 1000000) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    // Ler o número de cidades
    fscanf(arquivo, "%d", &estrada->N);
    if (estrada->N < 2 || estrada->N > 10000) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (estrada->C == NULL) {
        printf("Erro ao alocar memória para as cidades.\n");
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    // Ler os dados das cidades
    for (int i = 0; i < estrada->N; i++) {
        int xi;
        char nome[256];
        fscanf(arquivo, "%d %s", &xi, nome);

        if (xi <= 0 || xi >= estrada->T) {
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        }

        estrada->C[i].Posicao = xi;
        strcpy(estrada->C[i].Nome, nome);
    }

    fclose(arquivo);
    return estrada;
}

// Função para calcular a menor vizinhanca
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return -1.0; // Retorna -1.0 em caso de erro
    }

    double menorVizinhanca = DBL_MAX;

    // Ordenar as cidades por posição (caso necessário)
    for (int i = 0; i < estrada->N - 1; i++) {
        for (int j = i + 1; j < estrada->N; j++) {
            if (estrada->C[i].Posicao > estrada->C[j].Posicao) {
                Cidade temp = estrada->C[i];
                estrada->C[i] = estrada->C[j];
                estrada->C[j] = temp;
            }
        }
    }

    // Calcular as vizinhancas
    for (int i = 0; i < estrada->N - 1; i++) {
        double vizinhanca = estrada->C[i + 1].Posicao - estrada->C[i].Posicao;
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }

    free(estrada->C);
    free(estrada);
    return menorVizinhanca;
}

// Função para encontrar a cidade com a menor vizinhanca
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return NULL; // Retorna NULL em caso de erro
    }

    double menorvizinhanca = DBL_MAX;
    int cidadeIndex = -1;

    // Ordenar as cidades por posição (caso necessário)
    for (int i = 0; i < estrada->N - 1; i++) {
        for (int j = i + 1; j < estrada->N; j++) {
            if (estrada->C[i].Posicao > estrada->C[j].Posicao) {
                Cidade temp = estrada->C[i];
                estrada->C[i] = estrada->C[j];
                estrada->C[j] = temp;
            }
        }
    }

    // Encontrar a cidade com a menor vizinhanca
    for (int i = 0; i < estrada->N - 1; i++) {
        double vizinhanca = estrada->C[i + 1].Posicao - estrada->C[i].Posicao;
        if (vizinhanca < menorvizinhanca) {
            menorvizinhanca = vizinhanca;
            cidadeIndex = i;
        }
    }

    char *cidade = (char *)malloc(256 * sizeof(char));
    if (cidadeIndex != -1) {
        strcpy(cidade, estrada->C[cidadeIndex].Nome);
    } else {
        strcpy(cidade, ""); // Retorna string vazia em caso de erro
    }

    free(estrada->C);
    free(estrada);
    return cidade;
}