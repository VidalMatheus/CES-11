/*  Autor:  Armando Gouveia                   */
/*                                            */
/*  CES-11: Algoritmos e Estruturas de Dados  */
/*                                            */
/*  Programa compilado com Code::Bocks 13.12  */
/*
--------------------------------------------------
       MARCAR  TEMPO
--------------------------------------------------

Este programa mostra como utilizar a biblioteca time.h
para marcar o tempo de execucao de algum procedimento.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( )
{
   clock_t inicio,
           fim;
   float   delta_t;

   printf ("Vou marcar o tempo. Aperte ENTER para terminar...");
   inicio = clock();
   getchar ();
   fim    = clock();
   delta_t = (fim - inicio) / (float) CLOCKS_PER_SEC;
   printf("\nDemorou: %.2f segundos\n\n", delta_t);
   system ("pause");
   return 0;
}
