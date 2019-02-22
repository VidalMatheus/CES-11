/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 03: Ordenação                            */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para poder marcar o tempo


void MergeSort (char **nomes, int ini, int fim, int n);
void Merge (char **nomes, int ini, int fim, int n);

int countCompMerge = 0;

clock_t inicio,
        fim;
float   delta_t;

int main(){
	char **bubble,**merg,**quick; // Bubble, Merge e Sort
    int i, N;
    FILE *in, *outBubble, *outMerge, *outQuick;
	printf("\t\t----------Ordenacao----------\n");
    in = fopen ("C:\\Lab3\\entrada3.txt", "r");
    if (in == NULL)
	{
	   	printf("\n\nPROBLEMAS NO ARQUIVO DE ENTRADA!\n");
	   	return 1;
	}
    outMerge = fopen ("C:\\Lab3\\vidal3merge.txt", "w");
    if (outMerge == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA MERGE!\n");
        return 1;
    }
    fscanf(in, "%d  ", &N); // Captura o número de nomes a serem ordenados
    /*Declaração da matriz de nomes dinâmicamente*/
	merg = (char **) malloc (N*sizeof(char*));
	for (i = 0; i < N; i++){
		merg[i] = (char *) malloc (59*sizeof(char));
	} // Matrizes N x 59

	/*Leitura dos nomes do arquivo de entrada*/

	for (i = 0; i < N; i++){
		fgets(merg[i],59,in);
	}

    /*Chamada da funções de ordenação MergeSort*/
	inicio = clock();
	MergeSort (merg, 0, N-1, N);
	fim    = clock();
	delta_t = (fim - inicio) / (float) CLOCKS_PER_SEC;
	/*Escrever no arquivo de saída do MergeSort*/
	fprintf(outMerge,"Algoritmo: Merge-Sort\n\n");
    fprintf(outMerge,"Tamanho da entrada: %d \n", N);
    fprintf(outMerge,"Comparacoes feitas: %d\n", countCompMerge);
    fprintf(outMerge,"Tempo de execucao : %.3f segundos\n\n", delta_t);
    fprintf(outMerge,"--------------------------------------------------\n");
    /*Escrita dos nomes ordenados no arquivo de saída*/
    for (i = 0; i < N; i++){
    	fprintf(outMerge,"%s", merg[i]);
    }

    /*Fechamento dos arquivos de entrada e saída*/
	fclose(in);
	fclose(outMerge);

	system("vidal3merge.txt");

	free(merg);

	return 0;
}


//--------------------------------------------------
//    MERGE
//--------------------------------------------------

void Merge (char **nomes, int ini, int fim, int n){
   char T[n][59]; // temporario
   int med,
       i,          //percorre T
       j,          //percorre lado esquerdo de V
       k;          //percorre lado direito  de V

   med = (ini+fim)/2; //mesmo arredondamento feito ao chamar MergeSort
   j = ini;
   k = med+1;
   i = ini;
   //percorrer selecionando os menores
   while (j <= med && k <= fim)
   {
      if (strcmp (nomes[j],nomes[k]) <= 0)
      {
         strcpy(T[i],nomes[j]);
         j++;
      }
      else
      {
         strcpy (T[i],nomes[k]);
         k++;
      }
      i++;
      countCompMerge++;
   }
   //se sobrou algo `a esquerda, copiar para T
   while(j <= med)
   {
      strcpy(T[i],nomes[j]);
      i++;
      j++;
   }
   //se sobrou algo `a direita, copiar para T
   while(k<=fim)
   {
      strcpy(T[i],nomes[k]);
      i++;
      k++;
   }
   // V recebe T
   for(i=ini; i<=fim; i++)
      strcpy(nomes[i],T[i]);
}

//--------------------------------------------------
//    MERGE SORT
//--------------------------------------------------

void MergeSort (char **nomes, int ini, int fim, int n)
{
   int med;
   if (ini < fim)
   {
      med = (ini+fim)/2;          // dividir ao meio
      MergeSort (nomes, ini,   med, n);  // ordenar lado esquerdo
      MergeSort (nomes, med+1, fim, n);  // ordenar lado direito
      Merge     (nomes, ini,   fim, n);  // mesclar as duas metades
   }
}
