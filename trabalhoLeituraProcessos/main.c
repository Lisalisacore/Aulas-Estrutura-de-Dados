#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSOS 1000
#define MAX_LINHA 256

int main() {
    Processo processos[MAX_PROCESSOS];
    int n = 0;

    FILE *arquivo = fopen("processo_043_202409032338.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    char linha[MAX_LINHA];
    fgets(linha, MAX_LINHA, arquivo);

    while (fgets(linha, MAX_LINHA, arquivo) != NULL && n < MAX_PROCESSOS) {
        sscanf(linha, "%d,\"%[^\"]\",%d-%d-%d %d:%d:%d,{%d},{%d},%d",
               &processos[n].id,
               processos[n].numero,
               &processos[n].data_ajuizamento.tm_year,
               &processos[n].data_ajuizamento.tm_mon,
               &processos[n].data_ajuizamento.tm_mday,
               &processos[n].data_ajuizamento.tm_hour,
               &processos[n].data_ajuizamento.tm_min,
               &processos[n].data_ajuizamento.tm_sec,
               &processos[n].id_classe,
               &processos[n].id_assunto,
               &processos[n].ano_eleicao);

        processos[n].data_ajuizamento.tm_year -= 1900;
        processos[n].data_ajuizamento.tm_mon -= 1;

        n++;
    }

    fclose(arquivo);

    ordenarPorId(processos, n);
    FILE *saida1 = fopen("processo_043_202409032338.csv", "w");
    fprintf(saida1, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(saida1, "%d,%s,%d-%02d-%02d %02d:%02d:%02d,%d,%d,%d\n",
                processos[i].id,
                processos[i].numero,
                processos[i].data_ajuizamento.tm_year + 1900,
                processos[i].data_ajuizamento.tm_mon + 1,
                processos[i].data_ajuizamento.tm_mday,
                processos[i].data_ajuizamento.tm_hour,
                processos[i].data_ajuizamento.tm_min,
                processos[i].data_ajuizamento.tm_sec,
                processos[i].id_classe,
                processos[i].id_assunto,
                processos[i].ano_eleicao);
    }
    fclose(saida1);

    ordenarPorData(processos, n);
    FILE *saida2 = fopen("processo_043_202409032338.csv", "w");
    fprintf(saida2, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(saida2, "%d,%s,%d-%02d-%02d %02d:%02d:%02d,%d,%d,%d\n",
                processos[i].id,
                processos[i].numero,
                processos[i].data_ajuizamento.tm_year + 1900,
                processos[i].data_ajuizamento.tm_mon + 1,
                processos[i].data_ajuizamento.tm_mday,
                processos[i].data_ajuizamento.tm_hour,
                processos[i].data_ajuizamento.tm_min,
                processos[i].data_ajuizamento.tm_sec,
                processos[i].id_classe,
                processos[i].id_assunto,
                processos[i].ano_eleicao);
    }
    fclose(saida2);

    int id_classe = 638633058;
    int countClasse = contarPorClasse(processos, n, id_classe);
    printf("Quantidade de processos com id_classe %d: %d\n", id_classe, countClasse);

    int assuntosUnicos = contarAssuntosUnicos(processos, n);
    printf("Quantidade de assuntos únicos: %d\n", assuntosUnicos);

    listarProcessosMultiplosAssuntos(processos, n);

    for (int i = 0; i < n; i++) {
        int dias = calcularDiasTramitacao(processos[i]);
        printf("Processo ID %d está em tramitação há %d dias.\n", processos[i].id, dias);
    }

    return 0;
}