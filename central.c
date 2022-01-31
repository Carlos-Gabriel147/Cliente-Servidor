#include "central.h"

//Função do professor
int inicializa_e_espera(uint16_t porta){
  int welcomeSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_in client;
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
  {
      printf("Erro de versao do WSocket!\n");
      exit(1);
  }
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(porta);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if(listen(welcomeSocket,5)==0)
    printf("Esperando...\n");
  else
    printf("Erro!\n");
  int addr_size = sizeof(struct sockaddr_in);
  newSocket = accept(welcomeSocket, (struct sockaddr *) &client, &addr_size);
  return newSocket;
}

//Funções do aluno
Fila* cria_fila(){
	Fila* f= (Fila*) malloc(sizeof(Fila));
	f->frente= NULL;
	f->fundo= NULL;
	return f;
}

int fila_vazia(Fila* f){
	return (f->frente==NULL) || (f==NULL);
}

void inserir(Fila* f, float valor){
	Lista* novo= (Lista*) malloc(sizeof(Lista));
	novo->info= valor;
	novo->prox= NULL;
	if(fila_vazia(f)){
		f->frente= novo;
	}else{
		f->fundo->prox= novo;
	}
	f->fundo= novo;
}

//Não confundir "liberar" com "limpar_memoria"
void liberar(Fila** f, float limiar){
    if(fila_vazia(*f)){
        return;
    }
    Lista* L= (*f)->frente;
    printf("\nProcessando 5 ultimos dados:\n");
	while(L!=NULL){
		if(L->info > limiar){
			printf("Nivel de bateria OK (%.2f%%)\n", L->info*100);
		}else{
			printf("Alerta! Nivel de bateria baixo (%.2f%%)\n", L->info*100);
		}
		Lista* aux= L->prox;
        free(L);
        L= aux;
    }
    free(*f);
    (*f)= NULL;
	(*f) = cria_fila();
}

void limpar_memoria(Fila** f){
    if(fila_vazia(*f)){
        return;
    }
    Lista* L= (*f)->frente;
    while(L!=NULL){
        Lista* aux= L->prox;
        free(L);
        L= aux;
    }
	free(*f);
    (*f)= NULL;
}