/*
 * File:   main.c
 * Autora: Renata Caroline Cunha
 * Matrícula : 0022429
 * Data: 14/11/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

int main(int argc, char const *argv[])
{
  	int opEntrada, opSaida;
    char arqEntrada[100], arqSaida[100];
    FILE *arquivo;

    menu(&opEntrada, &opSaida, arqEntrada, arqSaida);

    if(opSaida==1) /*Se a opção escolhida para saída de dados for via Teclado*/
    {
        arquivo=stdout;
    }
    else /*Se a opção escolhida para saída de dados for via arquivo de texto*/
    {
        arquivo=fopen(arqSaida,"wt");
        if(!arquivo)
        {
            printf("Erro ao criar o arquivo");
            exit(EXIT_FAILURE);
        }
    }

    if(opEntrada==1) /* Se a entrada escolhida for via Teclado */
    {
        leExpressaoAritmetica(arqEntrada, arquivo);  /* arqEntrada contém a expressão obtida via teclado */
    }
    else /* Se a entrada escolhida for via arquivo de texto */
    {
        leArquivo(arqEntrada, arquivo);
    }

    fclose(arquivo);

    return 0;
}
