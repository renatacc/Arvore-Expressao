/*
 * File:   arvore.c
 * Autora: Renata Caroline Cunha
 * Matrícula : 0022429
 * Data: 14/11/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

#define OPERANDO 0
#define OPERADOR 1

/* ------ Estrutura da Pilha ----- */
struct nodo
{
 	 struct no *arvore;
 	 struct nodo *prox;
};

struct pilha
{
	 struct nodo *inicio;
	 int tam;
};

/* ------------------------------- */

/* -- Estrutura do Nó da árvore -- */
struct no
{
	struct no *dir;
	struct no *esq;
	int operando;
	char operador;
	int tipoNo;
};

/* ------------------------------- */

/* -------------- Pilha -------------- */
typedef struct pilha *Pilha;

Pilha criaPilha(void);

void destroiPilha(Pilha p);

void inserePilha(Pilha p,Arvore arvore);

Arvore retiraPilha(Pilha p);

int vaziaPilha(Pilha p);
/* ----------------------------------- */

/* ---------- Cabeçalho das funções ----------*/
void copiaExpressao(char *expressao);

Arvore criaArvore(Arvore esq, Arvore dir, int tipo, int operando, char operador);

void destroiArvore(Arvore arv);

void imprimePosOrdem(Arvore arv, FILE *arq);

void imprimePreOrdem(Arvore arv, FILE *arq);

void imprimeInOrdem(Arvore arv, FILE *arq);

void imprimeParenteseEsq(Arvore arv, char parentese,FILE *arq);

void imprimeParenteseDir(Arvore arv, char parentese,FILE *arq);

float calculaExpressao(Arvore arv);

void imprimeSaida(Arvore arv, FILE *arq, float resultado);
/* --------------------------------------------*/

/* ------------------ Pilha -------------------*/
Pilha criaPilha(void)
{
	 Pilha p=(struct pilha*)malloc(sizeof(struct pilha));

	 p->tam=0;
	 p->inicio=NULL;

	 return(p);
}

void destroiPilha(Pilha p)
{
     int i;

     if(vaziaPilha(p)==0)
     {
        for(i=1;i<=p->tam;i++)
        {
           retiraPilha(p);
        }
     }

     free(p);
     p=NULL;

     return;
}

void inserePilha(Pilha p,Arvore arvore)
{
     if (vaziaPilha(p)==1) /* Se a pilha estiver vazia */
     {
        struct nodo *novo=(struct nodo*)malloc(sizeof(struct nodo));

        novo->arvore=arvore;
        novo->prox=NULL;

        p->inicio=novo;
        p->tam++;

	    return;
     }
     /* Outros casos */
     struct nodo *novo=(struct nodo*)malloc(sizeof(struct nodo));

     novo->arvore=arvore;
     novo->prox=p->inicio;

     p->inicio=novo;
     p->tam++;

     return;
}

Arvore retiraPilha(Pilha p)
{
    Arvore retornaValor;
    /*Se a pilha tiver apenas um elemento */
	if(p->tam==1)
	{
		 retornaValor=(p->inicio)->arvore;
		 free(p->inicio);

		 p->inicio=NULL;
		 p->tam--;

		 return(retornaValor);
	}
    /* Outros casos */
    retornaValor=(p->inicio)->arvore;

    struct nodo *aux=p->inicio;
    p->inicio=aux->prox;

    free(aux);
    aux=NULL;
    p->tam--;

    return(retornaValor);
}

int vaziaPilha(Pilha p)
{
    if(p->tam==0) /* Se a pilhar estiver vazia retorna 1 */
    {
	     return(1);
    }

    return(0);
}
/* --------------------------------------------*/

/* Mostra um menu para o usuário escolher como será a entrada e saída de dados */
void menu(int *opEntrada, int *opSaida, char *arqEntrada, char *arqSaida)
{
    printf("Escolha uma opção para Entrada de Dados: \n");
    printf("1- Via teclado\n");
    printf("2- Via arquivo de texto\n\n");

    scanf("%d",opEntrada);

    if(*opEntrada==1)
    {
    	printf("Digite a expressão: ");
      scanf(" %[^\n]s",arqEntrada);
    }
    else
    {
    	printf("Digite o nome do Arquivo de Entrada: ");
    	scanf(" %[^\n]s",arqEntrada);
    }

    printf("\n\n");

    printf("Escolha uma opção para Saída de Dados: \n");
    printf("1- Via teclado\n");
    printf("2- Via arquivo de texto\n\n");

    scanf("%d",opSaida);

    if(*opSaida==2)
    {
   		printf("Digite o nome do Arquivo de Saída: ");
    	scanf(" %[^\n]s",arqSaida);
    }

    return;
}

/* Lê cada expressão do arquivo */
void leArquivo(char arqEntrada[], FILE *arq)
{
     FILE *arquivo;
     arquivo=fopen(arqEntrada,"r");
     char expressao[1000];

     if(arquivo==NULL)
     {
         return;
     }

     while((fgets(expressao,sizeof(expressao),arquivo))!=NULL)
     {
         if(strlen(expressao)>3)
         {
             leExpressaoAritmetica(expressao, arq); /* Função que monta a árvore e gera os dados de saída. */
         }
     }

     fclose(arquivo);

     return;
}

/* Pega o restante da expressão sem o caracter que já foi utilizado */
void copiaExpressao(char *expressao)
{
    int i,j=0;

    for(i=2;i<strlen(expressao);i++) /*2 porque nao conta o operador anterior e nem o espaço*/
    {
        expressao[j]=expressao[i];
        j++;
    }

    expressao[j]='\0';

    return;
}

Arvore criaArvore(Arvore esq, Arvore dir, int tipo, int operando, char operador)
{
    Arvore novo=(struct no*)malloc(sizeof(struct no));

    novo->esq=esq;
    novo->dir=dir;
    novo->tipoNo=tipo;

    if(novo->tipoNo==OPERANDO) /* Se o tipo for um operando */
    {
        novo->operando=operando;
    }

    if(novo->tipoNo==OPERADOR) /* Se tipo for um operador */
    {
        novo->operador=operador;
    }

    return(novo);
}

void destroiArvore(Arvore arv)
{
    if (!arv)
    {
        destroiArvore(arv->esq);  /* libera sae */
        destroiArvore(arv->esq);   /* libera sad */
        free(arv);  /* libera raiz */
    }

    arv=NULL;

    return;
}

/* Gera expressão e imprime resultados */
void leExpressaoAritmetica(char *expressao, FILE *arq)
{
	 int num;
   float resultado;
	 char operador;
   Pilha pilha=criaPilha();
   Arvore arv;

	 while(strlen(expressao)>0) /* Enquanto o tamanho da expressão for maior que zero.. */
	 {
	     sscanf(expressao,"%c ", &operador); /* Pega um caracter na expressão*/

	     if(operador=='+' || operador=='-' || operador=='*' || operador=='/') /* Se o caracter for um operador */
	     {
	         arv=retiraPilha(pilha); /* Retira arvore da direita da nova arvore */
	         arv=criaArvore(retiraPilha(pilha),arv,OPERADOR,num,operador); /* Cria nova árvore */
	         inserePilha(pilha,arv); /* Insere a árvore criada na pilha */
	     }
	     else /* Se o caracter for um operando */
	     {
	         num=operador-'0'; /* Transforma um char em um inteiro */
	         arv=criaArvore(NULL,NULL,OPERANDO,num,operador); /* Cria a árvore com este operando*/
	         inserePilha(pilha,arv); /* Insere a árvore criada na pilha */
	     }

	     copiaExpressao(expressao); /* Pega o restante da expressão sem o caracter que já foi utilizado */
	 }

     arv=retiraPilha(pilha); /* Retira a árvore de expressões gerada */
     resultado=calculaExpressao(arv); /* Calcula o valor da expressão aritmetica */
     imprimeSaida(arv,arq,resultado); /* Imprime os resultados gerados */
     destroiPilha(pilha);
     destroiArvore(arv);

     return;
}

void imprimePosOrdem(Arvore arv, FILE *arq)
{
    if(arv)
    {
        imprimePosOrdem(arv->esq,arq);
        imprimePosOrdem(arv->dir,arq);

       if(arv->tipoNo==OPERANDO)
       {
           fprintf(arq,"%d ", arv->operando);
       }
       if(arv->tipoNo==OPERADOR)
       {
           fprintf(arq,"%c ", arv->operador);
       }
    }

    return;
}

void imprimePreOrdem(Arvore arv, FILE *arq)
{
    if(arv)
    {
    	if(arv->tipoNo==OPERANDO)
        {
           fprintf(arq,"%d ", arv->operando);
        }

        if(arv->tipoNo==OPERADOR)
        {
           fprintf(arq,"%c ", arv->operador);
        }

        imprimePreOrdem(arv->esq,arq);
        imprimePreOrdem(arv->dir,arq);
    }

    return;
}

void imprimeInOrdem(Arvore arv, FILE *arq)
{
    if(arv)
    {
        imprimeParenteseEsq(arv,'(',arq);

        imprimeInOrdem(arv->esq,arq);

        imprimeParenteseEsq(arv,')',arq);

        if(arv->tipoNo==OPERANDO) /* Se for operando */
        {
            fprintf(arq,"%d ", arv->operando);
        }
        else /* Operador */
        {
            fprintf(arq,"%c ", arv->operador);
        }

        imprimeParenteseDir(arv,'(',arq);

        imprimeInOrdem(arv->dir,arq);

        imprimeParenteseDir(arv,')',arq);
    }
}

void imprimeParenteseEsq(Arvore arv, char parentese,FILE *arq)
{
  if( (arv->tipoNo == OPERADOR) && (arv->operador == '*' || arv->operador == '/') ) /* Se for o operador '*' ou '/' */
  {
      if( (arv->esq->tipoNo == OPERADOR) && ( arv->esq->operador == '+' || arv->esq->operador == '-') )
      { /* E se a sub arvore  da esquerda for o operador '+' ou '-' deve-se colocar o parentese passado por parametro */
          fprintf(arq,"%c",parentese);
      }
  }

  return;
}

void imprimeParenteseDir(Arvore arv, char parentese,FILE *arq)
{ /* Se for um operador e ele for diferente do operador da subárvore a direita */
  if( (arv->tipoNo == OPERADOR) && (arv->operador != arv->dir->operador) )
  {
      if((arv->dir->tipoNo == OPERADOR) && ( arv->dir->operador == '+' || arv->dir->operador == '-') )
      {/* E se a sub arvore da direita for o operador '+' ou '-' deve-se colocar o parentese passado por parametro */
          fprintf(arq,"%c",parentese);
      }
  }

  return;
}

void imprimeSaida(Arvore arv, FILE *arq, float resultado)
{
	 fprintf(arq,"-------------------------------");
	 fprintf(arq,"\nInOrdem: ");
   imprimeInOrdem(arv,arq);
   fprintf(arq,"\n\nPréOrdem: ");
   imprimePreOrdem(arv,arq);
   //fprintf(arq,"\n\nPósOrdem: ");
   //imprimePosOrdem(arv,arq);
   fprintf(arq,"\n\nResultado da Expressao: %.2f\n", resultado);

   return;
}

/* Calcula o valor da árvore de expressão */
float calculaExpressao(Arvore arv)
{
    float resultado, direita, esquerda;

    if(arv)
    {
       esquerda=calculaExpressao(arv->esq);
       direita=calculaExpressao(arv->dir);

        if(arv->tipoNo==OPERADOR)
        {
             if(arv->operador=='+')
             {
                 resultado=esquerda+direita;
             }

             if(arv->operador=='-')
             {
                 resultado=esquerda-direita;
             }

             if(arv->operador=='*')
             {
                 resultado=esquerda*direita;
             }

             if(arv->operador=='/')
             {
                 if(direita==0)
                 {
                     resultado=esquerda/1;
                 }
                 else
                 {
                     resultado=esquerda/direita;
                 }
             }
        }
        else
        {
             resultado=arv->operando;
        }
    }

    return(resultado);
}
