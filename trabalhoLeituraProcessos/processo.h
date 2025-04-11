#ifndef PROCESSO_H
#define PROCESSO_H

#include <time.h>

typedef struct {
    int id;
    char numero[21];
    struct tm data_ajuizamento;
    int id_classe;
    int id_assunto;
    int ano_eleicao;
} Processo;

void ordenarPorId(Processo *processos, int n);
void ordenarPorData(Processo *processos, int n);
int contarPorClasse(Processo *processos, int n, int id_classe);
int contarAssuntosUnicos(Processo *processos, int n);
void listarProcessosMultiplosAssuntos(Processo *processos, int n);
int calcularDiasTramitacao(Processo processo);

#endif