/*  Autor:  Armando Gouveia                   */
/*                                            */
/*  CES-11: Algoritmos e Estruturas de Dados  */
/*                                            */
/*  Programa compilado com Code::Bocks 13.12  */
/*
/*

Este arquivo contem a implementacao do algoritmo
QUICK SORT.
Os codigos aqui apresentados são os mesmos
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
//    PARTITION
//--------------------------------------------------
/*
Dado um vetor V com elementos em qualquer ordem,
escolher um elemento para ser pivo^
e então dividir os elementos de V em duas partes:
  `a esquerda: menores que o pivo
  `a direita:  maiores ou iguais ao pivo
*/

int Partition (vetor V, int ini, int fim)
{
   tipelemento pivo, aux;
   int esq, dir;

   pivo = V[ini];    // neste exemplo, o pivo é o primeiro da esquerda
   esq  = ini + 1;
   dir  = fim;
   do
   {
      // percorrer da esquerda para a direita. encontrar alguem >= pivo
      while (esq < fim && V[esq] < pivo)
         esq++;
      // percorrer da direita para a esquerda. encontrar alguem < pivo
      while (ini < dir && pivo <= V[dir])
         dir--;
      // se achou um `a esquerda e outro `a direita
      if(esq < dir)
      {
         // permutar
         aux    = V[esq];
         V[esq] = V[dir];
         V[dir] = aux;
      }
      // caso contrario, terminara´ a repeticao
   }while (esq < dir);

   // trocar o pivo com o elemento que "divide" os subvetores
   V[ini] = V[dir];
   V[dir] = pivo;
   // retornar a posição da "divisa"
   return dir;
}

//--------------------------------------------------
//    QUICK SORT
//--------------------------------------------------

void QuickSort (vetor V, int ini, int fim)
{
   int p; //posição do pivo
   if (ini < fim)
   {
      p = Partition (V, ini, fim);
      QuickSort     (V, ini, p-1);
      QuickSort     (V, p+1, fim);
   }
}

//--------------------------------------------------
//    MAIN
//--------------------------------------------------

int main ()
{
   int n;
   vetor V;  // a ser ordenado

   QuickSort (V, 0, n-1);

   return 0;
}


