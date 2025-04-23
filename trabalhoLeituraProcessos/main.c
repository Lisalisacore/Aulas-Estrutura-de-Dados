#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 256

int main() {
    Processo *processos = NULL; 
    int capacidade = 10;        
    int n = 0;                  

    processos = (Processo *)malloc(capacidade * sizeof(Processo));
    if (processos == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    FILE *arquivo = fopen("ordenado_por_data.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        free(processos);
        return 1;
    }

    char linha[MAX_LINHA];
    fgets(linha, MAX_LINHA, arquivo);

    while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
        
        if (n >= capacidade) {
            capacidade *= 2; 
            processos = (Processo *)realloc(processos, capacidade * sizeof(Processo));
            if (processos == NULL) {
                printf("Erro ao realocar memória.\n");
                fclose(arquivo);
                return 1;
            }
        }

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
    for (int i = 0; i < n; i++) {
        printf("ID: %d\n", processos[i].id);
    }

    FILE *saida1 = fopen("ordenado_por_id.csv", "w");
    if (saida1 == NULL) {
        printf("Erro ao criar o arquivo ordenado_por_id.csv.\n");
        free(processos);
        return 1;
    }
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
        printf("Escrevendo processo ID: %d\n", processos[i].id); 
    }
    fclose(saida1);

    ordenarPorData(processos, n);
    FILE *saida2 = fopen("ordenado_por_data.csv", "w");
    if (saida2 == NULL) {
        printf("Erro ao criar o arquivo ordenado_por_data.csv.\n");
        fclose(saida1);
        free(processos);
        return 1;
    }
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

    free(processos);

    return 0;
}