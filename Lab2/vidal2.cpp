/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 02: Produto de Matrizes                  */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int m (int i, int j); // Conta o mínimo de multiplicações necessárias para se calcular o produto de matrizes de A_i a A_j
int countCall = 0; // Contador de chamadas da função m(i,j)
int countMulti = 0; // Contador de multiplicações escalares
int mc[21][21] = {0}; // Matriz que guarda, no elemento multiplica_i,j, o número mínimo de operações necessárias para se efetuar a multiplicação de A_i até A_j 
int L[21] = {0};  // Guarda o Número de linhas das matrizes
int C[21] = {0}; // Guarda o número de colunas das matrizes
void ordem(FILE *out, int i, int j); // Mostra a ordem de para realizar o produto de matrizes

int main(){
	char line[79];
    int i, N;
    FILE *in, *out;
	printf("\t\t----------Produto de Matrizes----------");
    in = fopen ("C:\\Lab2\\entrada2.txt", "r");
    if (in == NULL)
	{
	   	printf("\n\nPROBLEMAS NO ARQUIVO DE ENTRADA!\n");
	   	return 1;
	}
    out = fopen ("C:\\Lab2\\vidal2.txt", "w");
    if (out == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA!\n");
        return 1;
    }
    for(i = 1; i <= 5; i++){//Pular as 5 primeiras linhas
        fgets(line,79,in);
    }
    fscanf(in, "%d  ", &N); // Captura o número de matrizes a serem multiplicadas
    fgets(line,79,in); // Pula 1 linha, pois o fscanf já coloca o ponteiro do arquivo para o próximo caractere ou número.
    for (i = 1; i<= N; i++) // Guardando as dimensões das matrizes
    	fscanf(in, "%d  %d", &L[i], &C[i]); 
    	
    /* Início - Parte A*/    
    //Criação do Cabeçalho do arquivo de saída
    fprintf(out,"PRODUTO EFICIENTE DE UMA CADEIA DE MATRIZES\n");
    fprintf(out,"INPUT: Cadeia Multiplicavel de Matrizes \n");
    fprintf(out,"OUTPUT: Melhor Ordem, visando o Menor Tempo Computacional\n\n");
    fprintf(out,"Total de multiplicacoes escalares = %d\n\n",  m(1,N)); // Chamada da função recursiva para as N matrizes
    fprintf(out,"Chamadas de funcao: %d\n\n", countCall);
    /*Fim - Parte A*/
    
    /*Início - Parte B*/
    fprintf(out,"Melhor ordem para realizar o produto:\n");
    ordem (out,1,N);
    /*Fim - Parte B*/
    
    fclose(in);
    fclose(out);
    return 0;
}

int m (int i , int j){ 
	countCall++;
	int min, k, prox;
	if( i == j) 
		return 0;
	else{
		min = m(i,i)+m(i+1,j)+L[i]*C[i]*C[j];
		mc[i][j] = i;
		for( k=i+1; k<j ; k++){
			prox = m(i,k)+m(k+1,j)+L[i]*C[k]*C[j];
			if( prox < min){
				mc[i][j] = k;
				min = prox;
			}
		}
		return min;
	}
}

void ordem(FILE *out, int i, int j){ 
	if (i+1 == j){ // Basta multiplicar as matrizes i e j, que estão nas folhas da árvore
		fprintf(out,"\n%6d x %2d", i, j);
	}
	else {
		if (i == mc[i][j] && j != mc[i][j] + 1) { // Corte igual ao primeiro elemento
			ordem(out,mc[i][j]+1,j);
			fprintf(out,"\n%6d x %2d- %d", i, mc[i][j] +1, j);
		}
		else {
			if (i != mc [i][j] && j == mc[i][j] + 1){ // Corte igual ao segundo elemento
				ordem(out,i, mc[i][j]);
				fprintf(out,"\n%3d- %d x %2d", i, mc[i][j], j);
			}
			else {// Caso diferente dos casos acima, basta percorrer como se fosse na notação pós-fixada ( "esquerda, direita e visita")
				ordem(out, i ,mc[i][j]); // Esquerda do nó
				ordem(out, mc[i][j]+1, j); // Direita do nó
				fprintf(out,"\n%3d- %d x %2d- %d", i, mc[i][j], mc[i][j]+1,j); // Apresenda a multilicação dos ramos da árvore
			}
		}
	}
}
