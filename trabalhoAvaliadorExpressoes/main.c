#include <stdio.h>
#include <string.h>
#include "expressao.c"
#include "expressao.h"

int main() {
    char expr[512];

    printf("Digite uma expressao (pode ser em notacao infixa ou pos-fixa):\n");
    fgets(expr, sizeof(expr), stdin);
    // Remove o caractere de nova linha, se houver
    expr[strcspn(expr, "\n")] = '\0';

    printf("\nEntrada: %s\n", expr);

    // Converte a partir da notacao infixa para pos-fixa
    char *posFixa = getFormaPosFixa(expr);
    // Converte a partir da notacao pos-fixa para infixa
    char *inFixa = getFormaInFixa(expr);
    
    // Avalia a expressao:
    float valorInFixa = getValorInFixa(expr);
    float valorPosFixa = getValorPosFixa(expr);

    printf("Expressao em notacao pos-fixa: %s\n", posFixa);
    printf("Expressao em notacao infixa: %s\n", inFixa);
    printf("Valor (interpretando como infixa): %f\n", valorInFixa);
    printf("Valor (interpretando como pos-fixa): %f\n", valorPosFixa);

    return 0;
}
