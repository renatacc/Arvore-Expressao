/*
 * File:   arvore.h
 * Autora: Renata Caroline Cunha
 * Matrícula : 0022429
 * Data: 14/11/2016
 */

#ifndef arvore_H
#define arvore_H

typedef struct no *Arvore;

void menu(int *opEntrada, int *opSaida, char *arqEntrada, char *arqSaida);

void leExpressaoAritmetica(char *expressao, FILE *arq);

void leArquivo(char arqEntrada[], FILE *arq);

#endif
