/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 01: Controlador de Voo                   */
/*  Programa compilado com CodeBlocks 16.01      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct ptr {
    char city[39];
    int code;
    struct ptr *prox;
};

void build(struct ptr **inicio, FILE *in, FILE *out); //Constru��o da fila e do arquivo de sa�da

int main(){
    struct ptr *inicio = (struct ptr*)NULL;
    FILE *in, *out;
	printf("\t\t----------Controlador de Voo----------");
    in = fopen ("C:\\Lab1\\entrada1.txt", "r");
    if (in == NULL)
	{
	   	printf("\n\nPROBLEMAS NO ARQUIVO DE ENTRADA!\n");
	   	return 1;
	}
    out = fopen ("C:\\Lab1\\vidal1.txt", "w");
    if (out == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA!\n");
        return 1;
    }
    build(&inicio, in, out);
    fclose(in);
    fclose(out);
    printf("\n");
    return 0;
}

void build(struct ptr **inicio, FILE *in, FILE *out){
    struct ptr *aux = (struct ptr*)NULL;
    struct ptr *aux1 = (struct ptr*)NULL;
    struct ptr *aux2 = (struct ptr*)NULL;
    char city[39], line[79], status[79];
    int i, code, numUrg = 0;
    for(i=1;i<=8;i++){//Pular as 8 primeiras linhas
        fgets(line,79,in);
    }
    fscanf(in,"%s",status);
    //Cria��o do Cabe�alho do arquivo de sa�da
    fprintf(out,"Controle de Voo - Joseloid International Airport\n");
    fprintf(out,"Houston! Houston...We are good.\n");
    fprintf(out,"AUTORIZACOES DE POUSO\n");
    fprintf(out,"=========================================\n");
    fprintf(out,"FLIGHT  FROM\n\n");
    //Fim do cabe�alho do arquivo de sa�da
    while (strcmp(status,"FIM") != 0){
        if (!strcmp(status,"pede_pouso")){
                if((*inicio) == (struct ptr*)NULL){ //Fila vazia
                    (*inicio) = (struct ptr*) malloc(sizeof (struct ptr));
                    aux = (*inicio);
                    fscanf(in, "%d  ", &aux->code);
                	fgets(aux->city,39,in);
               		aux->prox = (struct ptr*)NULL;
                }
                else{
				 	for (aux = (*inicio); aux->prox != (struct ptr*) NULL;aux = aux->prox);//Posicionamento do ponteiro aux no final da fila
                	aux->prox = (struct ptr*) malloc(sizeof (struct ptr));
                	aux = aux->prox;
                	fscanf(in, "%d  ", &aux->code);
                	fgets(aux->city,39,in);
                	aux->prox = (struct ptr*)NULL;
            	}
        }
        if (!strcmp(status,"pista_liberada")){
			fgets(line,79,in);
			if((*inicio) == (struct ptr*)NULL){//Fila vazia
				fprintf(out,"0000    Nenhum aviao pousando\n");
			}
			else{
				fprintf(out,"%04d    %s", (*inicio)->code, (*inicio)->city);
				aux = (*inicio);
				(*inicio) = (*inicio)->prox;
				aux->prox = (struct ptr*)NULL;
				free(aux);
				aux = (struct ptr*)NULL;
			}
			if(numUrg > 0) numUrg--;
        }
        if (!strcmp(status,"URGENCIA")){ //Muda o primeiro da fila com o voo urgente
        	numUrg++;
          	fscanf(in, "%d  ", &code);
           	fgets(line,79,in);
        	if( (*inicio) != (struct ptr*)NULL){//Se for pedido urg�ncia no in�cio, nada ser� feito
               	if( (*inicio)->prox != (struct ptr*)NULL){ //Se tiver s� 1 elemento na fila, n�o faz nada
               		if( (*inicio)->prox->prox == (struct ptr*)NULL && numUrg == 1){// Se tiver apenas 2 elementos na fila, garantindo que, caso o segundo pedir urg�ncia consecutivamente, a ordem ficar� correta
               			if( (*inicio)->code != code){//Caso contr�rio, o primeiro elemento j� est� na primeira posi��o
               				(*inicio)->prox->prox = (*inicio);
               				(*inicio) = (*inicio)->prox;
               				(*inicio)->prox->prox = (struct ptr*)NULL;
						}
					}
					else{
                		for (aux2 = (*inicio), i = 1; i != numUrg-1 && numUrg != 1 ; i++, aux2 = aux2->prox); // Posicionamento do ponteiro aux2 para realizar a troca com a ordem de urg�ncia
				       	for (aux = (*inicio)->prox, aux1 = (*inicio); aux->code != code ;aux = aux->prox,aux1 = aux1->prox);//Posicionamento do ponteiro aux no avi�o que pediu urg�ncia
				       	if( numUrg == 1){ //Caso a urg�ncia seja para o primeiro da fila
				       		aux1->prox = aux->prox;
               				aux->prox = (*inicio);
               				(*inicio) = aux;
               				aux = (struct ptr*)NULL;
               				aux1 = (struct ptr*)NULL;
               				aux2 = (struct ptr*)NULL;
						}
						else{
							aux1->prox = aux->prox;
               				aux->prox = aux2->prox;
               				aux2->prox = aux;
               				aux = (struct ptr*)NULL;
               				aux1 = (struct ptr*)NULL;
               				aux2 = (struct ptr*)NULL;

						}
					}
                }
            }
		}
		fscanf(in,"%s",status);
    }
    //Mostrar elementos remanescenstes na fila
    fprintf(out,"\nSituacao da fila\n\n");
    if((*inicio) == (struct ptr*)NULL)
    		fprintf(out,"Nao ha avioes em fila");
	else{
		for (aux = (*inicio); aux->prox != (struct ptr*) NULL;aux = aux->prox)
			fprintf(out,"%04d    %s", aux->code, aux->city);
		fprintf(out,"%04d    %s", aux->code, aux->city);
	}
}

