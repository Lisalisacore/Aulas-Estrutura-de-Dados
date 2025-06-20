#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expressao.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Função auxiliar para obter o próximo token da expressão
// Atualiza o índice *i e copia o token para o array token.
int getNextToken(const char *expr, int *i, char *token) {
    int j = 0;
    // Pula espaços em branco
    while(expr[*i] != '\0' && isspace(expr[*i]))
        (*i)++;
    if(expr[*i] == '\0')
        return 0;

    // Se o token for um número (suporta dígitos e ponto decimal)
    if(isdigit(expr[*i]) || expr[*i]=='.') {
        while(expr[*i] != '\0' && (isdigit(expr[*i]) || expr[*i]=='.')) {
            token[j++] = expr[*i];
            (*i)++;
        }
        token[j] = '\0';
        return 1;
    }
    // Se o token for uma sequência de letras (ex.: funções: sen, cos, tg, log, raiz)
    else if(isalpha(expr[*i])) {
        while(expr[*i] != '\0' && isalpha(expr[*i])) {
            token[j++] = expr[*i];
            (*i)++;
        }
        token[j] = '\0';
        return 1;
    }
    // Caso contrário, trata-se de um operador ou parêntese (token de 1 caractere)
    else {
        token[j++] = expr[*i];
        token[j] = '\0';
        (*i)++;
        return 1;
    }
}

// Verifica se o token (string) corresponde a um operador binário
int isOperator(const char *token) {
    if(strlen(token) == 1 &&
       (token[0]=='+' || token[0]=='-' || token[0]=='*' ||
        token[0]=='/' || token[0]=='%' || token[0]=='^'))
        return 1;
    return 0;
}

// Verifica se o token é uma das funções unárias previstas
int isFunction(const char *token) {
    return (strcmp(token, "sen") == 0 ||
            strcmp(token, "cos") == 0 ||
            strcmp(token, "tg") == 0  ||
            strcmp(token, "log") == 0 ||
            strcmp(token, "raiz") == 0);
}

// Retorna a precedência do operador ou função
int precedence(const char *token) {
    if(isFunction(token))
        return 4;  // funções têm alta precedência
    if(strcmp(token, "^") == 0)
        return 3;
    if(strcmp(token, "*") == 0 || strcmp(token, "/") == 0 || strcmp(token, "%") == 0)
        return 2;
    if(strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
        return 1;
    return 0;
}

// Verifica se o operador é associativo para a direita
int isRightAssociative(const char *token) {
    return (strcmp(token, "^") == 0);
}

// Converte uma expressão infixa (em Str) para a notação pós-fixa.
// A implementação utiliza o algoritmo de Shunting-yard.
char *getFormaPosFixa(char *Str) {
    static char output[512];
    output[0] = '\0';

    char opStack[100][64]; // Pilha para operadores/funções
    int top = -1;
    int i = 0;
    char token[64];

    while(getNextToken(Str, &i, token)) {
        // Se for número, escreve-o na saída (adicionando um espaço para separar os tokens)
        if(isdigit(token[0]) || token[0]=='.') {
            strcat(output, token);
            strcat(output, " ");
        }
        // Se for uma sequência de letras, pode ser uma função
        else if(isalpha(token[0])) {
            if(isFunction(token)) {
                top++;
                strcpy(opStack[top], token);
            } else {
                // Se for variá­vel (não esperado neste trabalho), adiciona direto
                strcat(output, token);
                strcat(output, " ");
            }
        }
        else if(strcmp(token, "(") == 0) {
            top++;
            strcpy(opStack[top], token);
        }
        else if(strcmp(token, ")") == 0) {
            // Desempilha até encontrar o '('
            while(top >= 0 && strcmp(opStack[top], "(") != 0) {
                strcat(output, opStack[top]);
                strcat(output, " ");
                top--;
            }
            if(top >= 0 && strcmp(opStack[top], "(") == 0)
                top--;  // descarta o '('
            // Se houver uma função no topo, desempilha-a
            if(top >= 0 && isFunction(opStack[top])) {
                strcat(output, opStack[top]);
                strcat(output, " ");
                top--;
            }
        }
        // Se for operador binário
        else if(isOperator(token)) {
            while(top >= 0 && strcmp(opStack[top], "(") != 0 &&
                  ((precedence(opStack[top]) > precedence(token)) ||
                   (precedence(opStack[top]) == precedence(token) && !isRightAssociative(token)))) {
                strcat(output, opStack[top]);
                strcat(output, " ");
                top--;
            }
            top++;
            strcpy(opStack[top], token);
        }
        else {
            // Token não reconhecido (poderia adicionar tratamento de erro)
        }
    }
    // Desempilha os operadores remanescentes
    while(top >= 0) {
        if(strcmp(opStack[top], "(")==0 || strcmp(opStack[top], ")")==0)
            top--; // erro de parênteses incompatíveis
        else {
            strcat(output, opStack[top]);
            strcat(output, " ");
            top--;
        }
    }
    return output;
}

// Converte uma expressão em notação pós-fixa (em Str) para sua forma infixa.
// Para cada token em pós-fixa, se for operando, empilha; se for operador, desempilha os operandos
// e forma a expressão infixa com parênteses.
char *getFormaInFixa(char *Str) {
    static char output[512];
    output[0] = '\0';
    char stack[100][512];   // Pilha para strings intermediárias
    int top = -1;
    int i = 0;
    char token[64];

    while(getNextToken(Str, &i, token)) {
        // Se for número, empilha
        if(isdigit(token[0]) || token[0]=='.') {
            top++;
            strcpy(stack[top], token);
        }
        // Se for sequência de letras (pode ser função ou variável)
        else if(isalpha(token[0]) && !isOperator(token)) {
            if(isFunction(token)) {
                if(top < 0) {
                    strcpy(stack[++top], "error");
                } else {
                    char operand[512];
                    strcpy(operand, stack[top]);
                    top--;
                    char temp[512];
                    sprintf(temp, "%s(%s)", token, operand);
                    top++;
                    strcpy(stack[top], temp);
                }
            } else {
                top++;
                strcpy(stack[top], token);
            }
        }
        // Se for operador binário
        else if(isOperator(token)) {
            if(top < 1) {
                strcpy(stack[++top], "error");
            } else {
                char op2[512], op1[512];
                strcpy(op2, stack[top]);
                top--;
                strcpy(op1, stack[top]);
                top--;
                char temp[512];
                sprintf(temp, "(%s%s%s)", op1, token, op2);
                top++;
                strcpy(stack[top], temp);
            }
        }
    }
    if(top == 0)
        strcpy(output, stack[top]);
    else
        strcpy(output, "error");
    return output;
}

// Avalia uma expressão em notação pós-fixa e retorna o valor numérico (float).
// Para cada token, se for número empilha; se for função ou operador, desempilha os operandos necessários.
float getValorPosFixa(char *StrPosFixa) {
    float stack[100];
    int top = -1;
    int i = 0;
    char token[64];

    while(getNextToken(StrPosFixa, &i, token)) {
        if(isdigit(token[0]) || token[0]=='.') {
            float num = atof(token);
            top++;
            stack[top] = num;
        }
        // Se for função unária
        else if(isalpha(token[0]) && !isOperator(token)) {
            if(top < 0)
                return 0; // erro de operandos insuficientes
            float operand = stack[top];
            top--;
            float result = 0.0;
            if(strcmp(token, "sen") == 0)
                result = sin(operand * M_PI/180.0);
            else if(strcmp(token, "cos") == 0)
                result = cos(operand * M_PI/180.0);
            else if(strcmp(token, "tg") == 0)
                result = tan(operand * M_PI/180.0);
            else if(strcmp(token, "log") == 0)
                result = log10(operand);
            else if(strcmp(token, "raiz") == 0)
                result = sqrt(operand);
            top++;
            stack[top] = result;
        }
        // Se for operador binário
        else if(isOperator(token)) {
            if(top < 1)
                return 0; // erro de operandos insuficientes
            float b = stack[top]; top--;
            float a = stack[top]; top--;
            float result = 0.0;
            if(strcmp(token, "+") == 0)
                result = a + b;
            else if(strcmp(token, "-") == 0)
                result = a - b;
            else if(strcmp(token, "*") == 0)
                result = a * b;
            else if(strcmp(token, "/") == 0) {
                if(b == 0) return 0; // erro: divisão por zero
                result = a / b;
            }
            else if(strcmp(token, "%") == 0)
                result = fmod(a, b);
            else if(strcmp(token, "^") == 0)
                result = pow(a, b);
            top++;
            stack[top] = result;
        }
    }
    if(top == 0)
        return stack[top];
    return 0;
}

// Avalia uma expressão em notação infixa
// (para isso, converte-a para pós-fixa e então a avalia).
float getValorInFixa(char *StrInFixa) {
    char *postfix = getFormaPosFixa(StrInFixa);
    return getValorPosFixa(postfix);
}
