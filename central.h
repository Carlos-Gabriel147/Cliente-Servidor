#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <winsock2.h>

//Estruturas da fila
typedef struct lista{
	float info;
	struct lista* prox;
}Lista;

typedef struct fila{
	struct lista* frente;
	struct lista* fundo;
}Fila;

//Funções utilizadas no projeto
//Não confundir "liberar" com "limpar_memoria"
int inicializa_e_espera(uint16_t porta);
Fila* cria_fila();
int fila_vazia(Fila* f);
void inserir(Fila* f, float valor);
void liberar(Fila** f, float limiar);
void limpar_memoria(Fila** f);

#endif // SERVIDOR_H_INCLUDED