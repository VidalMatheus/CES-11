/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 03: Ordena��o                            */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para poder marcar o tempo

#define TRUE 1
#define FALSE 0

/*Fun��es de ordena��o*/
void BubbleSort (int n, char **nomes); // M�todo Bubble-Sort
void MergeSort (char **nomes, int ini, int fim); // M�todo  recursivo MergeSort
void Merge (char **nomes, int ini, int fim); // M�todo Merge utilizado para unir as partes separadas pelo meio ordenadas do MergeSort
int Partition (char **nomes, int ini, int fim); // M�todo Partition utilizado para unir as partes separadas pelo piv� ordenadas do QuickSort
void QuickSort (char **nomes, int ini, int fim); // M�todo recursivo QuickSort

int N; // Quantidade de nomes

int countCompBubble = 0;
int countCompMerge = 0;
int countCompQuick = 0;

/*Vari�veis de tempo*/
clock_t inicio,
        fim;
float   delta_t;

char **T; // Como a vari�vel static n�o funcionou, vamos declarar global e, em seguida, alocar dinamicamente

int main(){
	char **bubble,**merg,**quick; // Bubble, Merge e Sort
    int i;
    FILE *in, *outBubble, *outMerge, *outQuick;
	printf("\t\t----------Ordenacao----------\n");
    in = fopen ("C:\\Lab3\\entrada3.txt", "r");
    if (in == NULL)
	{
	   	printf("\n\nPROBLEMAS NO ARQUIVO DE ENTRADA!\n");
	   	return 1;
	}
    outBubble = fopen ("C:\\Lab3\\vidal3bubble.txt", "w");
    if (outBubble == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA BUBBLE!\n");
        return 1;
    }
    outMerge = fopen ("C:\\Lab3\\vidal3merge.txt", "w");
    if (outMerge == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA MERGE!\n");
        return 1;
    }
    outQuick = fopen ("C:\\Lab3\\vidal3quick.txt", "w");
    if (outQuick == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA QUICK!\n");
        return 1;
    }

    fscanf(in, "%d  ", &N); // Captura o n�mero de nomes a serem ordenados

	/*Declara��o da matriz de nomes din�micamente*/
	bubble = (char **) malloc (N*sizeof(char*));
	merg = (char **) malloc (N*sizeof(char*));
	quick = (char **) malloc (N*sizeof(char*));
	for (i = 0; i < N; i++){
		bubble[i] = (char *) malloc (59*sizeof(char));
		merg[i] = (char *) malloc (59*sizeof(char));
		quick[i] = (char *) malloc (59*sizeof(char));
	} // Matrizes N x 59

	/*Leitura dos nomes do arquivo de entrada*/
	for (i = 0; i < N; i++){
		fgets(bubble[i],59,in);
		strcpy (merg[i],bubble[i]);
		strcpy (quick[i],bubble[i]);
	}

	/*Chamada da fun��es de ordena��o BubbleSort*/
	inicio = clock();
	BubbleSort(N,bubble);
	fim    = clock();
   	delta_t = (fim - inicio) / (float) CLOCKS_PER_SEC;
	/*Escrever no arquivo de sa�da do BubbleSort*/
	fprintf(outBubble,"Algoritmo: Bubble-Sort\n\n");
    fprintf(outBubble,"Tamanho da entrada: %d \n", N);
    fprintf(outBubble,"Comparacoes feitas: %d\n", countCompBubble);
    fprintf(outBubble,"Tempo de execucao : %.3f segundos\n\n", delta_t);
    fprintf(outBubble,"--------------------------------------------------\n");
    /*Escrita dos nomes ordenados no arquivo de sa�da*/
    for (i = 0; i < N; i++){
    	fprintf(outBubble,"%s", bubble[i]);
    }

	/*Chamada da fun��es de ordena��o MergeSort*/
	/*Como n�o funcionou a vari�vel static, vamos alocar T dinamicamente antes da fun��o e, em seguida, dar free*/
	inicio = clock();
	T = (char **) malloc (N*sizeof(char*));
	for (i = 0; i < N; i++)
		T[i] = (char *) malloc (59*sizeof(char));
	MergeSort (merg, 0, N-1);
	for (i = 0; i < N; i++)
		free(T[i]);
	free(T);
	fim    = clock();
	delta_t = (fim - inicio) / (float) CLOCKS_PER_SEC;
	/*Escrever no arquivo de sa�da do MergeSort*/
	fprintf(outMerge,"Algoritmo: Merge-Sort\n\n");
    fprintf(outMerge,"Tamanho da entrada: %d \n", N);
    fprintf(outMerge,"Comparacoes feitas: %d\n", countCompMerge);
    fprintf(outMerge,"Tempo de execucao : %.3f segundos\n\n", delta_t);
    fprintf(outMerge,"--------------------------------------------------\n");
    /*Escrita dos nomes ordenados no arquivo de sa�da*/
    for (i = 0; i < N; i++){
    	fprintf(outMerge,"%s", merg[i]);
    }

	/*Chamada da fun��es de ordena��o QuickSort*/
	inicio = clock();
	QuickSort(quick, 0, N-1);
	fim    = clock();
	delta_t = (fim - inicio) / (float) CLOCKS_PER_SEC;
	/*Escrever no arquivo de sa�da do BubbleSort*/
	fprintf(outQuick,"Algoritmo: Quick-Sort\n\n");
    fprintf(outQuick,"Tamanho da entrada: %d \n", N);
    fprintf(outQuick,"Comparacoes feitas: %d\n", countCompQuick);
    fprintf(outQuick,"Tempo de execucao : %.3f segundos\n\n", delta_t);
    fprintf(outQuick,"--------------------------------------------------\n");
    /*Escrita dos nomes ordenados no arquivo de sa�da*/
    for (i = 0; i < N; i++){
    	fprintf(outQuick,"%s", quick[i]);
    }

    /*Fechamento dos arquivos de entrada e sa�da*/
	fclose(in);
	fclose(outBubble);
	fclose(outMerge);
	fclose(outQuick);

	/*Fechamento dos Arquivos*/
	free(bubble);
	free(merg);
	free(quick);
	
	return 0;
}

void BubbleSort (int n, char **nomes){ // Ordena��o em Bubble
	int i, p, trocou;
	char aux[59];
	p = n - 2;
	trocou = TRUE;
	while (p >= 0 && trocou){
		trocou = FALSE;
		i = 0;
		while ( i <= p){
			if( strcmp(nomes[i],nomes[i+1]) > 0 ){
				strcpy(aux, nomes[i]);
				strcpy (nomes[i], nomes[i+1]);
				strcpy(nomes[i+1], aux);
				trocou = TRUE; // Otimiza��o do Bubble
			}
			countCompBubble++; // Conta compara��es
			i++;
		}
		p--;
	}
}

//--------------------------------------------------
//    MERGE
//--------------------------------------------------

void Merge (char **nomes, int ini, int fim){
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
      countCompMerge++; //Conta compara��es
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

void MergeSort (char **nomes, int ini, int fim)
{
   int med;
   if (ini < fim)
   {
      med = (ini+fim)/2;          // dividir ao meio
      MergeSort (nomes, ini,   med);  // ordenar lado esquerdo
      MergeSort (nomes, med+1, fim);  // ordenar lado direito
      Merge     (nomes, ini,   fim);  // mesclar as duas metades
   }
}

//--------------------------------------------------
//    PARTITION
//--------------------------------------------------

int Partition (char **nomes, int ini, int fim)
{
   char pivo[59], aux[59];
   int esq, dir;

   strcpy(pivo,nomes[ini]);    // neste exemplo, o pivo � o primeiro da esquerda
   esq  = ini + 1;
   dir  = fim;
   do
   {
      // percorrer da esquerda para a direita. encontrar alguem >= pivo
      while (esq < fim && strcmp(nomes[esq],pivo) < 0){
      	countCompQuick++; //Conta compara��es
		esq++;
	  }
	  if(esq < fim) countCompQuick++;//Ao sair do while, pode ocorrer a contagem de novo. Basta verificar se a primeira senten�a � verdadeira, que a segunda ser� falsa. 
	  //Se a primeira senten�a for falsa, n�o � feita a compara��o entre string, j� se sai do la�o. Da�, a necessidade do if acima

      // percorrer da direita para a esquerda. encontrar alguem < pivo
      while (ini < dir && strcmp(pivo,nomes[dir]) <= 0){
      	countCompQuick++;//Conta compara��es
      	dir--;
	  }
	  if (ini < dir) countCompQuick++;//Ao sair do while, pode ocorrer a contagem de novo. Basta verificar se a primeira senten�a � verdadeira, que a segunda ser� falsa. 
	  //Se a primeira senten�a for falsa, n�o � feita a compara��o entre string, j� se sai do la�o. Da�, a necessidade do if acima

      // se achou um `a esquerda e outro `a direita
      if(esq < dir)
      {
         // permutar
         strcpy(aux,nomes[esq]);
         strcpy(nomes[esq],nomes[dir]);
         strcpy(nomes[dir],aux);
      }
      // caso contrario, terminara� a repeticao
   }while (esq < dir);

   // trocar o pivo com o elemento que "divide" os subvetores
   strcpy(nomes[ini],nomes[dir]);
   strcpy(nomes[dir],pivo);
   // retornar a posi��o da "divisa"
   return dir;
}

//--------------------------------------------------
//    QUICK SORT
//--------------------------------------------------

void QuickSort (char **nomes, int ini, int fim)
{
   int p; //posi��o do pivo
   if (ini < fim)
   {
      p = Partition (nomes, ini, fim);
      QuickSort     (nomes, ini, p-1);
      QuickSort     (nomes, p+1, fim);
   }
}
