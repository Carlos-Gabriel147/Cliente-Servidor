#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <winsock2.h>

//Função utilizada no projeto
int inicializa_e_conecta(uint16_t porta);

#endif // CLIENTE_H_INCLUDED