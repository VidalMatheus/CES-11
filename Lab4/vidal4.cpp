/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 04: Banco                                */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------Defini��o da estrutura de dados---------------------
struct account{
	char name[29];
	float balance;
};

//-------------Declara��o global do ponteiro para o vetor de structs-------
struct account *vet = (struct account*) NULL;

//-------------Prot�tipos de fun��es--------------------------------
int search (char name[], struct account vet[], int inicio, int fim, int quant, int MAX); // Retorna a posi��o do correntista cujo nome est� em name. Caso n�o exista, ela retornar� -1.

int main(){
	char line[59], action[29], name[29];
	// string line � auxiliar para pular linha ou pular espa�oes em branco
	// string action � auxiliar para identificar as a��es: ABRE_CONTA, FECHA_CONTA, DEPOSITO, SAQUE E EXTRATO
	// string name � auxilar para guardar o nome do correntista ao qual ser� aplicada a a��o
    int i, j, inicio, fim, quant, MAX;
    // inteiros i e j s�o vari�veis auxiliares que servem para itera��es (la�os)
    // inicio: guarda o in�cio da fila circular
    // fim: guarda o fim da fila circular
    // quant: guarda a quantidade de elementos da fila circular
    // MAX: guarda a quantidade m�xima de correntistas no sistema
    int dig; // quntidade de d�gitos de MAX
    float cash; // saldo em conta dos correntistas
    FILE *in, *out; // vari�veis para os aqrquivos de entrada e sa�da
	printf("\t\t----------Banco----------\n");
    in = fopen ("C:\\Lab4\\entrada4.txt", "r");
    if (in == NULL)
	{
	   	printf("\n\nPROBLEMAS NO ARQUIVO DE ENTRADA!\n");
	   	return 1;
	}
    out = fopen ("C:\\Lab4\\vidal4.txt", "w");
    if (out == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA!\n");
        return 1;
    }
    //Cria��o do Cabe�alho do arquivo de sa�da
    fprintf(out,"JoseLloyds Bank\n");
    fprintf(out,"Simple and efficient! Here your money is safer.\n");
    fprintf(out,"Don't worry! You are our priority.\n\n");
    fprintf(out,"RESULTADOS DAS CONSULTAS\n\n");// Caso n�o haja nenhum relat�rio de consultas, n�o ser� mostrado nada.
    //Fim do cabe�alho do arquivo de sa�da
    
    for(i = 1; i <= 5; i++){// Pular as 5 primeiras linhas
        fgets(line,59,in);
    }
    fscanf(in, "%d  ", &MAX); // Captura o n�mero m�ximo de correntistas
    
    
    //-------------Aloca��o din�mica e inicializa��o de par�metros do vetor cirucular de structs----------
    vet = (struct account*) malloc (MAX*sizeof(struct account));
	inicio = 0;
	fim = MAX - 1;
	quant = 0;
	// Fim da aloca��o e da inicializa��o do vetor circular de structs
	
    fgets(line,59,in); // Pula linha com "INICIO"

    fscanf(in,"%s",action); // Captura a a��o a ser realizada 
    
    while (strcmp(action,"FIM") != 0){
        if (!strcmp(action,"ABRE_CONTA")){// Realiza a abertura de conta corrente
        	fgets(line,13,in); // Pula os 12 espa�os em branco entre a a��o e o nome do correntista
        	fgets(name,29,in); // Captura o nome do correntista
        	if ( quant == MAX){ // Lista cheia !
        		fprintf(out, "ERRO: Lista lotada. Impossivel abrir conta de %s", name);
			}
			else{
        		if (quant == 0){
		        	strcpy (vet[0].name, name); // Guarda ao nome no vetor de structs
		        	vet[0].balance = 0; // Quando se abre a conta, come�a-se com zero reais
		        	inicio = 0; 
		        	fim = 0;
		        	quant++; // Incrementa a quantidade de correntistas cadastrados no sistema
				}
				else if (quant == 1){
					if (strcmp (name,vet[0].name) > 0){ // Colocar � direita
						strcpy (vet[1].name, name); // Guarda ao nome no vetor de structs
						vet[1].balance = 0; // Quando se abre a conta, come�a-se com zero reais
						inicio = 0;
						fim = 1;
						quant++; // Incrementa a quantidade de correntistas cadastrados no sistema
					}
					else if (strcmp (name, vet[0].name) < 0){ // Colocar � esquerda
						strcpy (vet[MAX-1].name, name); // Guarda ao nome no vetor de structs
						vet[MAX-1].balance = 0; // Quando se abre a conta, come�a-se com zero reais 
						inicio = MAX - 1;
						fim = 0;
						quant++; // Incrementa a quantidade de correntistas cadastrados no sistema
					}
					else{ // Nome j� existente
						fprintf(out,"ERRO: Conta ja existente de %s", name);
					}	
				}
				else{ // Vetor com pelos 2 elementos
					if ( search (name, vet, inicio, fim, quant, MAX) != -1){ // Caso o nome j� exista, a fun��o search retorna um inteiro diferente de -1, isto �, a posi��o do elemento (que � positiva sempre)
        				fprintf(out,"ERRO: Conta ja existente de %s", name);
					}
					else {
						i = (inicio + (quant-1)/2)%MAX; // Posi��o do termo do meio na lista cicular
						// Note que a conta acima ajusta, quando a quantidade de termos for par, para o �ndice do termo do meio ser a parte inteira da m�dia menos 1.
						// Isso garante que o shift deja realizado para a direita no caso em que houver uma quantidade par de elementos e os dois lados a partir da posi��o de inser��o serem do mesmo tamanho.
						// Al�m disso, note que a conta tamb�m garante que o shift ser� dado para o lado mais curto.
						if (strcmp(name,vet[i].name) > 0) {// Shift para direita
							for ( i = fim; strcmp(name, vet[(i+MAX)%MAX].name) < 0; i--)
								vet[(i+1+MAX)%MAX] = vet[(i+MAX)%MAX];
							strcpy(vet[(i+1+MAX)%MAX].name, name);
							vet[(i+1+MAX)%MAX].balance = 0;
							fim = (fim+1+MAX)%MAX;
						}
						else {// Shift para esquerda
							for ( i = inicio; strcmp(name, vet[(i+MAX)%MAX].name) > 0; i++)
								vet[(i-1+MAX)%MAX] = vet[(i+MAX)%MAX];
							strcpy(vet[(i-1+MAX)%MAX].name, name);
							vet[(i-1+MAX)%MAX].balance = 0;
							inicio = (inicio-1+MAX)%MAX;
						}
						quant++; // Incrementa a quantidade de correntistas cadastrados no sistema
					}
	       		}
	       	}
	    }
        if (!strcmp(action,"FECHA_CONTA")){// Realiza o fechamento da conta corrente
			fgets(line,12,in); // Pula os 11 espa�os em branco entre a a��o e o nome do correntista
        	fgets(name,29,in); // Captura o nome do correntista
        	j = search (name, vet, inicio, fim, quant, MAX);
        	if (j == -1){
        		fprintf(out, "ERRO: Nao encontrado cliente com nome %s", name);
			}
			else {
				i = (inicio + (quant-1)/2)%MAX; // Posi��o do termo do meio na lista cicular. An�logo � abertura de conta.
				if( quant == 1) { // Se houver apenas 1 elemento na fila circular
					quant--; // Decrementa a quantidade de correntistas cadastrados no sistema
				}
				else {
					// Casos das pontas
					if( j == inicio ){ // Caso se queira fechar a conta de algu�m no in�cio da fila circular
							inicio = (inicio + 1 + MAX)%MAX;
							quant--;// Decrementa a quantidade de correntistas cadastrados no sistema
					}
					else if ( j == fim ){ // Caso se queira fechar a conta de algu�m no fim da fila circular
							fim = (fim - 1 + MAX)%MAX;
							quant--;// Decrementa a quantidade de correntistas cadastrados no sistema
					}
					// Fim caso das pontas
					else{
					// Caso de fechamento de conta fora das pontas da fila circular	
						if (strcmp(name,vet[i].name) >= 0) {// Shift da direita. Obs.: Caso nome na posi��o do meio -> shift da direita
							for ( i = (j + MAX)%MAX; i<fim; i++)
								vet[(i+MAX)%MAX] = vet[(i+1+MAX)%MAX];
							fim = (fim - 1 + MAX)%MAX;
							quant--;// Decrementa a quantidade de correntistas cadastrados no sistema
						}
						else {// Shift da esquerda
							for ( i = (j + MAX)%MAX; i>inicio; i--)
								vet[(i+MAX)%MAX] = vet[(i - 1 + MAX)%MAX];
							inicio = (inicio + 1 + MAX)%MAX;
							quant--;// Decrementa a quantidade de correntistas cadastrados no sistema
						}	
					}
				}
			}
        }
        if (!strcmp(action,"DEPOSITO")){// Realiza o dep�sito na conta corrente
        	fscanf(in,"%f",&cash);// Captura o valor a ser depositado
        	fgets(line,3,in); // Pula os 2 espa�os em branco entre o valor e o nome do correntista
        	fgets(name,29,in); // Captura o nome do correntista
			i = search(name, vet, inicio, fim, quant, MAX); // Fun��o que retorna a posi��o do correntista cujo nome est� na vari�vel name
			if (i == -1){
       			fprintf(out, "ERRO: Nao encontrado cliente com nome %s", name);
			}
			else {
				vet[i].balance+= cash; // Incrementa o dep�sito
			}
    	}
    	if (!strcmp(action,"SAQUE")){// // Realiza o valor da conta corrente
    		fscanf(in,"%f",&cash);// Captura o valor a ser sacado
    		fgets(line,3,in); // Pula os 2 espa�os em branco entre o valor e o nome do correntista
			fgets(name,29,in); // Captura o nome do correntista
			i = search(name, vet, inicio, fim, quant, MAX); // Fun��o que retorna a posi��o do correntista cujo nome est� na vari�vel name. Caso n�o exista, ela retornar� -1
			if (i == -1){
				fprintf(out, "ERRO: Nao encontrado cliente com nome %s", name);
			}
			else {
				vet[i].balance-= cash; // Decrementa o saque
			}
    	}
    	if (!strcmp(action,"EXTRATO")){
    		fgets(line,16,in); // Pula os 15 espa�os em branco entre a a��o e o nome do correntista
        	fgets(name,29,in); // Captura o nome do correntista
        	i = search(name, vet, inicio, fim, quant, MAX); // Fun��o que retorna a posi��o do correntista cujo nome est� na vari�vel name. Caso n�o exista, ela retornar� -1
			if (i == -1){ 
				fprintf(out, "ERRO: Nao encontrado cliente com nome %s", name);
			}
			else {
				fprintf(out, "Saldo  %.2f  %s", vet[i].balance, vet[i].name); // Exibe no arquivo o rel�toiro de consulta
			}
    	}
    	
		fscanf(in,"%s",action); // Captura a pr�xima a��o a ser realizada 
    }
	
	// Determina��o do n�mero de d�gitos de MAX, que servir� para a formata��o do relat�rio final
	dig = 0;
	for (i = MAX; i!=0; i = (i - i%10)/10)
		dig++;
	// Fim da determina��o do n�mero de d�gitos de MAX
	
	// Relat�rio Final no arquivo de sa�da
	fprintf(out,"\nRELATORIO FINAL\n\n");
    fprintf(out,"Tamanho maximo da lista:    %*d\n", dig, MAX);
    fprintf(out,"Total de correntistas:      %*d\n", dig, quant);
    fprintf(out,"Posicoes ocupadas: inicio:  %*d\n", dig, inicio);
    fprintf(out,"                      fim:  %*d\n", dig, fim);
	// Fim do Relat�rio Final
	
	// Correntistas Atuais
	fprintf(out,"\nCORRENTISTAS ATUAIS\n\n");
	for (i = inicio; (i+MAX)%MAX!=fim; i++){
		fprintf(out,"%8.2f  %s", vet[(i+MAX)%MAX].balance, vet[(i+MAX)%MAX].name);		
	}
	if (quant >= 1)
		fprintf(out,"%8.2f  %s", vet[(i+MAX)%MAX].balance, vet[(i+MAX)%MAX].name);
	else // quant == 0
		fprintf(out," Lista vazia");
	// Fim - Correntistas Atuais
	
	free(vet); // Excluindo a aloca��o din�mica feita
	vet = (struct account*) NULL; // Apontamento do ponteiro vet para NULL 
		
	// Fechamento dos arquivos de entrada e sa�da
	fclose(in);
    fclose(out);
	
    return 0;
}

int search(char name[], struct account vet[], int inicio, int fim, int quant, int MAX){
	int i, posic;
	i = (inicio + (quant-1)/2)%MAX; // Posi��o do termo do meio na lista cicular. An�logo � abertura de conta.
	if (strcmp(name,vet[i].name) > 0){ // Metade da direita
		while ((i+MAX)%MAX != fim && strcmp(name, vet[(i+MAX)%MAX].name) != 0) // Enquanto n�o chegar ao final da fila circular, nem achar o nome, continua "andando" em sentido hor�rio
			i++;
		if ( strcmp(name, vet[(i+MAX)%MAX].name) == 0) // Caso tenha sa�do do while, porque achou o nome procurado, retornar a posi��o do mesmo
			posic = (i+MAX)%MAX;
		else // Caso n�o tenha achado o nome procurado, retorne -1
			posic = -1;
	}
	else if (strcmp(name,vet[i].name) < 0){// Metade da esquerda
		while ( (i+MAX)%MAX != inicio && strcmp(name, vet[(i+MAX)%MAX].name) != 0) // Enquanto n�o chegar ao in�cio da fila circular, nem achar o nome, continua "andando" em sentido anti-hor�rio
			i--;
		if ( strcmp(name, vet[(i+MAX)%MAX].name) == 0) // Caso tenha sa�do do while, porque achou o nome procurado, retornar a posi��o do mesmo
			posic = (i+MAX)%MAX;
		else // Caso n�o tenha achado o nome procurado, retorne -1
			posic = -1;
	}
	else posic = i; // Caso o nome procurado tenha posi��o igual ao do meio, ent�o basta retornar tal posi��o. O nome foi encontrado
	
	return posic;	
}
