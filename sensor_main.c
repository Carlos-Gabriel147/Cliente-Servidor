#include "sensor.h"  //Incluir biblioteca
#include <stdlib.h>  //Adicionar rand()
#include <string.h>  //Bom costume
#include <unistd.h>  //Adicionar sleep()
#include <stdio.h>   //Padrão

int main(void){
	//Declaração de variáveis
	int sensor_socket = inicializa_e_conecta(80);
	float carga = 1;
	char vivo_perg[6], carga_perg[7], vivo_resp = '1', carga_resp[7];
	
	while(1){
		//Vivo
		if(recv(sensor_socket, vivo_perg, 6, 0)){
			printf("\nRequisicao recebida: %s\n", vivo_perg);
			send(sensor_socket, &vivo_resp, 1, 0);
			printf("Resposta enviada: %c\n", vivo_resp);
		}
		
		//Carga
		sprintf(carga_resp, "%.4f", carga);
		if(recv(sensor_socket, carga_perg, 7, 0)){
			printf("Requisicao recebida: %s\n", carga_perg);
			send(sensor_socket, carga_resp, 7, 0);
			printf("Resposta enviada: %s\n", carga_resp);
		}
		
		//Decrementação da carga
		carga -= 0.005;
		
		//Finalizar só pra não enviar valor negativo (com if(carga<0) deu pau)
		if(carga<-0.00001){
			printf("\nDescarreguei :(\n");
			exit(0);
		}
	}
	return 0;
}