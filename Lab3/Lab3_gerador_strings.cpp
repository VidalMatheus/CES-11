/*  Autor:  Armando Gouveia                   */
/*                                            */
/*  CES-11: Algoritmos e Estruturas de Dados  */
/*                                            */
/*  Programa compilado com Code::Bocks 13.12  */
/*
--------------------------------------------------
       GERADOR  DE  STRINGS  ALEATORIAS
--------------------------------------------------

Este programa gera um arquivo contendo strings aleatorias, uma por linha.
Quantidade de strings: pergunta ao usuario.
Tamanho das strings  : sorteia entre 10 e 50
Conteudo das strings : primeiro caractere e' letra
                       demais caracteres: letras ou espaco
Observacoes:
   - a string pode conter espacos consecutivos
   - pode ocorrer espaco no final da string
   - mas isso nao afeta a proposta do Lab de Ordenacao

A primeira linha do arquivo contera' a quantidade de strings
Nome do arquivo: StringsAleatorias.txt
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main( )
{
   char   s [51];    // string
   int    i,
          k,
          n,         // quantidade de strings
          tam,       // tamanho de cada string
          cart;      // numero carteado
   FILE * sai;       // arquivo de saida

   // ler quantidade
   printf ("Quantos elementos? ");
   scanf  ("%d", & n);
   // abre arquivo e escreve quantidade
   sai = fopen ("entrada3.txt", "w");
   fprintf (sai, "%d\n", n);
   // inicializa o gerador de numeros aleatorios
   time_t t;
   srand((unsigned) time(&t));
   // gera  n  strings
   for(i=0; i<n; i++)
   {
      // sorteia o tamanho entre 10 e 50
      tam = 10 + rand () % 41;
      // primeiro caractere e' uma letra
      cart = rand () % 26;
      s[0] = 'a' + cart;
      // demais caracteres: letras ou espaco
      for (k=1; k<tam; k++)
      {
         cart = rand () % 27;
         // se sortear 26, escreve espaco. De 0 a 25 sao letras
         if (cart == 26)
              s[k] = ' ';
         else s[k] = 'a' + cart;
      }
      // marca fim de string e escreve
      s[tam] = '\0';
      fprintf (sai, "%s\n", s);
   }
   fclose (sai);
   system ("pause");
   return 0;
}
