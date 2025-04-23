#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void ordenarPorId(Processo *processos, int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (processos[j].id < processos[minIdx].id) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Processo temp = processos[i];
            processos[i] = processos[minIdx];
            processos[minIdx] = temp;
        }
    }
}

void ordenarPorData(Processo *processos, int n) {
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (mktime(&processos[j].data_ajuizamento) > mktime(&processos[maxIdx].data_ajuizamento)) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            Processo temp = processos[i];
            processos[i] = processos[maxIdx];
            processos[maxIdx] = temp;
        }
    }
}

int contarPorClasse(Processo *processos, int n, int id_classe) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (processos[i].id_classe == id_classe) {
            count++;
        }
    }
    return count;
}

int contarAssuntosUnicos(Processo *processos, int n) {
    int *assuntos = (int *)malloc(n * sizeof(int)); 
    int count = 0;

    for (int i = 0; i < n; i++) {
        int encontrado = 0;
        for (int j = 0; j < count; j++) {
            if (assuntos[j] == processos[i].id_assunto) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            assuntos[count++] = processos[i].id_assunto;
        }
    }

    free(assuntos);
    return count;
}

void listarProcessosMultiplosAssuntos(Processo *processos, int n) {
    printf("Processos com mais de um assunto:\n");
    printf("ID\tNumero\t\t\tData de Ajuizamento\tID Classe\tID Assuntos\tAno Eleicao\n");

    for (int i = 0; i < n; i++) {

        char *assuntosStr = strstr(processos[i].numero, "{");
        if (assuntosStr != NULL && strchr(assuntosStr, ',') != NULL) {

            printf("%d\t%s\t%d-%02d-%02d %02d:%02d:%02d\t%d\t%s\t%d\n",
                   processos[i].id,
                   processos[i].numero,
                   processos[i].data_ajuizamento.tm_year + 1900,
                   processos[i].data_ajuizamento.tm_mon + 1,
                   processos[i].data_ajuizamento.tm_mday,
                   processos[i].data_ajuizamento.tm_hour,
                   processos[i].data_ajuizamento.tm_min,
                   processos[i].data_ajuizamento.tm_sec,
                   processos[i].id_classe,
                   assuntosStr,
                   processos[i].ano_eleicao);
        }
    }
}

int calcularDiasTramitacao(Processo processo) {

    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);

    time_t dataAjuizamento = mktime(&processo.data_ajuizamento);
    time_t dataAtualTime = mktime(dataAtual);

    double diferencaSegundos = difftime(dataAtualTime, dataAjuizamento);
    int diferencaDias = (int)(diferencaSegundos / (60 * 60 * 24));

    return diferencaDias;
}