#ifndef EXPRESSAO_H 
#define EXPRESSAO_H 

    typedef struct { 
        char posFixa[512];     
        char inFixa[512];      
        float Valor;           
    } Expressao; 
    // Expressão na forma pos-fixa, como 3 12 4 + * 
    // Expressão na forma infixa, como 3 * (12 + 4) 
    // Valor numérico da expressão   
    char *getFormaInFixa(char *Str); // Retorna a forma inFixa de Str (posFixa) 
    char *getFormaPosFixa(char *Str); // Retorna a forma posFixa de Str (inFixa) 
    float getValorPosFixa(char *StrPosFixa); // Calcula o valor de Str (na forma posFixa) 
    float getValorInFixa(char *StrInFixa); // Calcula o valor de Str (na forma inFixa) 
#endif