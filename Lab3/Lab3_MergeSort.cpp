/*  Autor:  Armando Gouveia                   */
/*                                            */
/*  CES-11: Algoritmos e Estruturas de Dados  */
/*                                            */
/*  Programa compilado com Code::Bocks 13.12  */
/*

Este arquivo contem a implementacao do algoritmo
MERGE SORT.
Os codigos aqui apresentados sao os mesmos
que foram explicados durante a aula de CES-11.
*/

#include <stdio.h>
#include <stdlib.h>

//--------------------------------------------------
//    tipo do vetor
//--------------------------------------------------

#define MAX 100
typedef int tipelemento;
typedef tipelemento vetor [MAX];


//--------------------------------------------------
//    MERGE
//--------------------------------------------------
/*
Dado um vetor V e dadas duas posicoes ini e fim.
A metade da esquerda desse trecho ja´ esta´ ordenada e
a metade da direita  desse trecho ja´ esta´ ordenada.
Fazer intercalacao (merge) e obter V ordenado.
*/

void Merge (vetor V, int ini, int fim)
{
   static vetor T; // temporario
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
      if (V[j] <= V[k])
      {
         T[i] = V[j];
         j++;
      }
      else
      {
         T[i] = V[k];
         k++;
      }
      i++;
   }
   //se sobrou algo `a esquerda, copiar para T
   while(j <= med)
   {
      T[i] = V[j];
      i++;
      j++;
   }
   //se sobrou algo `a direita, copiar para T
   while(k<=fim)
   {
      T[i] = V[k];
      i++;
      k++;
   }
   // V recebe T
   for(i=ini; i<=fim; i++)
      V[i]=T[i];
}

//--------------------------------------------------
//    MERGE SORT
//--------------------------------------------------

void MergeSort (vetor V, int ini, int fim)
{
   int med;
   if (ini < fim)
   {
      med = (ini+fim)/2;          // dividir ao meio
      MergeSort (V, ini,   med);  // ordenar lado esquerdo
      MergeSort (V, med+1, fim);  // ordenar lado direito
      Merge     (V, ini,   fim);  // mesclar as duas metades
   }
}


//--------------------------------------------------
//    MAIN
//--------------------------------------------------

int main ()
{
   int n;
   vetor V; // a ser ordenado

   MergeSort (V, 0, n-1);

   return 0;
}

