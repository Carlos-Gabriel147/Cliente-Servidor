#define TIMEOUT 10

#include "central.h"  //Incluir biblioteca
#include <stdlib.h>  //Adicionar rand()
#include <string.h>  //Bom costume
#include <unistd.h>  //Adicionar sleep()
#include <stdio.h>   //Padrão
#include <time.h>    //Clock

int main(void){
	//Declaração de variáveis
	int central_socket = inicializa_e_espera(80);
	int ciclo, cont = 0;
	unsigned long int non_blocking = 1;
	float limiar, controle = 1;
	char vivo_perg[6] = "vivo?", carga_perg[7] = "carga?",  vivo_resp, carga_resp[7];
	clock_t t1 = 0, t2 = 0, t3 = 0;
	
	//Abrir arquivo e cria a fila
	FILE* arquivo = fopen("entradas.txt", "r");
	Fila* fileira = cria_fila();
	
	//Se o arquivo não existir
	if(arquivo==NULL){
		printf("Não foi possível ler o arquivo, crie um e volte aqui >:( !\n");
		exit(1);
	}
	
	//Ler valores no arquivo
	fscanf(arquivo, "%d", &ciclo);
	fscanf(arquivo, "%f", &limiar);
	
	//A função sleep só funciona 100% corretamente com inteiros >=1
	if(ciclo<1){
		ciclo = 1;
	}
	
	while(controle>0){
		
		//Vivo ou morto
		//sleep(ciclo) tem de ser logo depois do send, pois, se não, uma dessincronização afeta o programa e buga tudo
		//A variável vivo_resp recebe '0' para poder ser possível verificar uma atualização dela por meio do recv
		printf("\nEnviando ao sensor: vivo?\n");
		send(central_socket, vivo_perg, 6, 0);
		sleep(ciclo);
		while(1){
			t1 = clock();
			non_blocking = 1;
			ioctlsocket(central_socket, FIONBIO, &non_blocking);
			recv(central_socket, &vivo_resp, 1, 0);
			if(vivo_resp == '1'){
				printf("Recebido: %c\n", vivo_resp);
				vivo_resp = '0';
				break; //Comando essencial, para não entrar no próximo if caso receba resposta '1'
			}
			if(vivo_resp == '0'){
				sleep(1); //1 segundo de espera, para poder incrementá-lo no contador
				if(t3<1000){
					printf("Sensor parou de responder, esperando resposta:\n");
				}
				printf("%lds...\n", t3/CLOCKS_PER_SEC);
				t2 = clock();
				t3 += difftime(t2, t1);
				if(t3/CLOCKS_PER_SEC > TIMEOUT){
					printf("Sensor demorou de mais pra responder, reinicie o servidor e/ou o sensor >:(\n");
					exit(1);
				}
			}
		}
		//Resetar t3, se o sensor voltar a responder
		t3 = 0;
		//Nível de carga
		printf("Enviando ao sensor: carga?\n");
		send(central_socket, carga_perg, 7, 0);
		non_blocking = 0;
		ioctlsocket(central_socket, FIONBIO, &non_blocking);
		recv(central_socket, carga_resp, 7, 0);
		printf("Recebido: %s\n", carga_resp);
		
		//Inserir níveis na fila
		inserir(fileira, atof(carga_resp));
		
		//Processar 5 últimos dados
		cont++;
		if(cont!=0 && cont%5==0){
			liberar(&fileira, limiar);
		}
		controle = atof(carga_resp);
	}

	//Finalizar só pra n ficar recebendo valor negativo, e limpar a memória (variável controle)
	printf("Bateria decarregou!\n");
	limpar_memoria(&fileira);
	
	return 0;
}