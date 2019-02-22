/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 07: Fibra �tica                          */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COST 999 // custos inteiros menores que 1000

//-------------Defini��o da estrutura de dados---------------------
struct grafo{							// strct que define como ser� armazenado o grafo, nesse caso: matriz de adjac�ncia
	int v,								// armazena o n�mero de v�rtices do grafo		
		a;								// armazena o n�mero de arestas do grafo
	char name[159][19];					// vetor de strings com a poci��o dos nomes dos pr�dios
	int  custo[159][159],				// matriz de adjac�ncia que guarda os custos das arestas, caso o custo[i][j] seja zero, n�o h� caminho entre i e j
		 	   grau[159];    			// vetor de inteiros que armazena o grau de cada v�rtice do grafo
};

struct aresta {							// struct que define uma aresta, ou seja, os v�rtices com os quais se conecta, al�m do custo (peso) da aresta
	char predioA[19],					// string predioA � auxiliar para armazenar os pr�dios a serem adicionados no grafo
		 predioB[19];					// string predioB � auxiliar para armazenar os pr�dios a serem adicionados no grafo
	int 	   custo;					// custo � auxiliar para guardar o custo (dist�ncia) entre o predioA e o predioB
};

//-------------Prot�tipos de fun��es--------------------------------
void inicializaGrafo(struct grafo *gr);								// inicializa a matriz adjunta com todos os custos zero, at� que sejam inseridas aresta
void insereAresta (struct grafo *gr, int p1, int p2, int custo);	// fun��o que insere aresta entre dois v�rtices do grafo
void insereVertice (struct grafo *gr, char predio[]);				// fun��o que insere novo v�rtice no vetor de posi��es 
int busca (struct grafo gr, char predio[]);							// fun��o que retorna a posi��o de um pr�dio no vetor de posi��es
int posVizinho (struct grafo gr, int predio, int viz);				// fun��o que retorna a posi��o de um vizinho de um v�rtice, a partir da ordem do vizinho na lista de vizinhos
void BubbleSort (int n, struct aresta ar[]);						// algoritmo de ordena��o BubbleSort

//-----------Fun��es Relacionadas ao Algoritmo de Prim--------------
void algPrim(struct grafo gr, FILE *out);							// fun��o que implementa algoritmo de Prim para achar a �rvore de cobertura de custo m�nimo (MST - Minimum Spanning Tree)
void printMST(int parent[], int ord[], struct grafo gr, FILE *out);			// Fun��o auxiliar para escrever no arquivo de sa�da o MST constru�do guardado no vetor de pais parent[] 
int minKey(int key[], bool mstSet[], struct grafo gr);				// Fun��o auxiliar para encontrar o v�rtice com valor chave m�nimo do conjunto de v�rtices ainda n�o inclusos no MST

int main(){
	struct   grafo gr;						// gr armazenar� a estrutura do grafo 
	struct  aresta ar;						// ar armazenar� a aresta a ser colocada no grafo
	char     line[79];       				// string line � auxiliar para pular linha 
    int            p1,						// auxiliar que guarda a posi��o de um v�rtice no vetor de posi��es
    			   p2,						// auxiliar que guarda a posi��o de um v�rtice no vetor de posi��es
					i,					 	// inteiro i � vari�vel auxiliar que serve para itera��es (la�os)
					j;					 	// inteiro j � vari�vel auxiliar que serve para itera��es (la�os)

    FILE *in, *out; 	 					// vari�veis para os aqrquivos de entrada e sa�da
	printf("\t\t----------Fibra Otica----------\n");
    in = fopen ("C:\\Lab7\\entrada7.txt", "r");
    if (in == NULL)
	{
	   	printf("\n\nPROBLEMAS NO ARQUIVO DE ENTRADA!\n");
	   	return 1;
	}
    out = fopen ("C:\\Lab7\\vidal7.txt", "w");
    if (out == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA!\n");
        return 1;
    }
    //Cria��o do Cabe�alho do arquivo de sa�da
    fprintf(out,"*FIBRA OTICA*\n");
    fprintf(out,"Make It Simple But Significant!\n");
    fprintf(out,"Prim's Algorithm!\n");
    fprintf(out,"CES-11 - ITA - Prof.: Armando\n");
    fprintf(out,"--------------------------------------------------\n");
    fprintf(out, "Ordem de selecao pelo algoritmo\n");
    fprintf(out,"--------------------------------------------------\n");
    //Fim do cabe�alho do arquivo de sa�da

    for(i = 1; i <= 6; i++){// Pular as 6 primeiras linhas
        fgets(line,79,in);
    }
    
    //---------------Inicializa��o do grafo--------------
	inicializaGrafo(&gr);						// inicializa a matriz adjunta com todos os custos zero, at� que sejam inseridas arestas

	while (!feof(in)){
		fscanf(in,"%s %s %d", ar.predioA, ar.predioB, &ar.custo);   	// l� cada aresta a ser adicionada presente em cada linha do arquivo 
		insereVertice (&gr, ar.predioA);								// insere o primeiro pr�dio no vetor de posi��es
		insereVertice (&gr, ar.predioB);								// insere o segundo pr�dio no vetor de posi��es
		p1 = busca(gr, ar.predioA);										// guarda a posi��o do primeiro pr�dio em p1
		p2 = busca(gr, ar.predioB);										// guarda a posi��o do primeiro pr�dio em p2
		insereAresta (&gr, p1, p2, ar.custo);							// insere aresta (custo) entre o primeiro e o segundo pr�dio
    }

    /*
    // TESTE 1 - Matriz de Adjacencia
    printf("\n\nTESTE 1 - Matriz Adjunta\n");
    for (i = 1; i <= gr.v; i++){
    	for (j = 1; j <= gr.v; j++){
    		printf("%3d", gr.custo[i][j]);
		}
		printf("\n");
	}
	
	// TESTE 2 - Vetor de Posi��es
	printf("\n\nTESTE 2 - Vetor de Posicoes");
	for (i = 1; i <= gr.v; i++){
		printf("\n%s",gr.name[i]);
	}
    
    // TESTE 3 - N�mero de V�rtices e Arestas
    printf("\n\nTESTE 3 - Numero de Vertices e Arestas");
    printf("\n(V,A) = (%d,%d)", gr.v, gr.a);
    
    // TESTE 4 - Grau de Cada V�rtice
    printf("\n\nTESTE 4 - Grau de Cada Vertice");
    for (i = 1; i <= gr.v; i++){
    	printf("\n grau (%d) = %d",i, gr.grau[i]);
	}
    */
    
    //---------------Chamada do Algoritmo de Prim--------------
	algPrim(gr, out);												// chamada da fun��o de Prim
	
    // Fechamento dos arquivos de entrada e sa�da
	fclose(in);
    fclose(out); 
    
    return 0;
}	

void inicializaGrafo(struct grafo *gr){		// inicializa a matriz adjunta com todos os custos zero, at� que sejam inseridas aresta
	int i, j;								// vari�veis auxiliares de la�os
	gr->v = 0;								// n�o h� v�rtices no in�cio
	gr->a = 0;								// n�o h� arestas no in�cio
	for (i = 1; i <= 150; i++){				// quantidade de pr�dios: no m�ximo 150 
		for (j = 1; j <= 150; j++){			
			gr->custo[i][j] = 0;			// no in�cio, todos os custos s�o zero
		}
		gr->grau[i] = 0;					// grau de todos os v�rtices � zero no in�cio
	}
}

void insereAresta (struct grafo *gr, int p1, int p2, int custo){	// fun��o que insere aresta entre dois v�rtices do grafo
	gr->custo[p1][p2] = custo;										// atribui��o de custo na matriz de adjac�ncia de custos
	gr->custo[p2][p1] = custo;										// atribui��o de custo na matriz de adjac�ncia de custos
	gr->a++;														// incrementa a quantidade de arestas do grafo
	gr->grau[p1]++;													// incrementa o grau do primeiro v�rtice da nova aresta
	gr->grau[p2]++;													// incrementa o grau do segundo v�rtice da nova aresta 
	// note que a matriz ser� sim�trica, como esperado
}

void insereVertice (struct grafo *gr, char predio[]){	// fun��o que insere novo v�rtice no vetor de posi��es 
	if (busca (*gr, predio) == -1){						// caso o pr�dio ainda n�o exista no vetore de posi��es
		gr->v++;										// incremeta a quantidade v�rtices do grafo
		strcpy (gr->name[gr->v], predio);				// copia para o vetor de posi��es o novo pr�dio inserido
	}
	// caso o v�rtice j� exista, n�o � feito nada
}

int busca (struct grafo gr, char predio[]){							// fun��o que retorna a posi��o de um pr�dio no vetor de posi��es
	int i;
	for (i = 1; i <= gr.v && strcmp(gr.name[i],predio) != 0; i++);		// percorre o vetore de posi��es at� achar a posi��o do elemento ou at� o final
	if ( i == gr.v + 1){		// caso n�o tenha encontrado o elemento, retorna -1
		return -1;
	}
	else{				// caso tenha encontrado o elemento, retorna sua posi��o
		return i;		// quando o la�o acaba porque achou o elemento, i armazena a posi��o desejada	
	}
}

//---------------Fun��es Relacionadas ao Algoritmo de Prim--------------

// Fun��o auxiliar para escrever no arquivo de sa�da o MST constru�do guardado no vetor de pais parent[] 
void printMST(int parent[], int ord[], struct grafo gr, FILE *out)
{	
	int min, orig, dest, k;
	int custo_total = 0,				// inicializa o custo total do MST como zero
   				     i, 				// vari�vel auxiliar para la�o
					 j;					// vari�vel auxiliar para contabilizar quantas arestas ser�o salvas em um vetor de arestas, para que possam, em seguida, serem ordenadas em ordem crescente de custo
	struct aresta *ar;					// ponteiro struct para declara��o do vetor de arestas do MST

	ar = (struct aresta*)malloc((gr.v)*sizeof(struct aresta)); 				// aloca��o din�mica do vetor de arestas no MST constru�do; h� necessariamente o n�mero de v�rtices - 1 arestas
	
	j = 1;
	for (i = 2; i <= gr.v; i++){																						// n�o � necess�rio escrever o primeiro, j� que n�o tem pai	
		fprintf(out, "%-11s%-11s%3d\n", gr.name[parent[ord[i]]], gr.name[ord[i]], gr.custo[ord[i]][parent[ord[i]]]);	// escreve no arquivo de sa�da em ordem ordem de selecao pelo algoritmo de Prim							
		strcpy (ar[j].predioA, gr.name[parent[i]]);																		// copia o nome do primeiro pr�dio para a struct
		strcpy (ar[j].predioB, gr.name[i]);																				// copia o nome do segundo pr�dio para a struct
		ar[j].custo = gr.custo[i][parent[i]];																			// copia o custo da aresta para a struct
		custo_total+=gr.custo[i][parent[i]];																			// incrementa no custo total do MST o custo da aresta
		j++;
	}


    fprintf(out, "\nCusto total: %d\n", custo_total);														// escreve no arquivo de sa�da o custo total do MST
	// Escrita do grafo em ordem crescente de custos
    fprintf(out, "\n--------------------------------------------------\n");
    fprintf(out, "Ordem crescente de custos\n");
    fprintf(out, "--------------------------------------------------\n");
    
    BubbleSort(gr.v-1, ar);																					// ordena o MST em ordem crescente de custo				
	for (i = 1; i <= gr.v-1; i++){
   		fprintf(out, "%-11s%-11s%3d\n", ar[i].predioA, ar[i].predioB, ar[i].custo);						// escreve na tela as arestas do MST em ordem crescente de custo
	}
	
	free(ar);

}

// Fun��o auxiliar para encontrar o v�rtice com valor chave m�nimo do conjunto de v�rtices ainda n�o inclusos no MST
int minKey(int key[], bool mstSet[], struct grafo gr)
{
   
	int min = MAX_COST, 			// inicializa o valor m�nimo
   			 min_index,				// �ndice do v�rtice com m�nimo valor chave
   			     	 v;				// vari�vel auxiliar de la�o
	for (v = 1; v <= gr.v; v++){
		if (mstSet[v] == false && key[v] < min){ 		// encontrar o v�rtice com valor chave m�nimo do conjunto de v�rtices ainda n�o inclusos no MST
			min = key[v];								// atualiza o valor chave de m�nimo
			min_index = v;								// atualiza o �ndice do v�rtice de valor chave de m�nimo
		}
	}
        
   return min_index;
}

// Fun��o para construir e mostrar o MST (Minimum Spanning Tree) de um grafo representado por matriz de adjac�ncia
void algPrim(struct grafo gr, FILE *out)
{
	int *parent,	 								// vetor para guardar o MST  
    	   *key,   									// valores chave usados para escolher a ponta de m�nimo custo em corte 
    	   *ord,									// ponteiro para um vetor que guardar� a ordem dos elementos adicionados no MST
    	      i,									// vari�vel auxiliar em la�o 
    	      v,									// vari�vel auxiliar em la�o
    	      u,									// vari�vel auxiliar que guarda o m�nimo valor chave do vetor de v�rtices ainda n�o inclusos no MST, em um determinado momento
    	      k;									// contador auxiliar para o vetor da ordem dos v�rtices adicionados no MST
    bool *mstSet;  									// para representar o conjunto de v�rtices ainda n�o inclusos no MST, true para insluso e false para n�o incluso 
	ord = (int*) malloc((gr.v+1)*sizeof(int));				
    parent = (int*) malloc((gr.v+1)*sizeof(int));
    key = (int*) malloc ((gr.v+1)*sizeof (int));
    mstSet = (bool*) malloc ((gr.v+1)*sizeof (bool));

    // inicializa todos os valores chave como m�ximo, no caso o m�ximo custo � a constante MAX_COST
    for (i = 1; i <= gr.v; i++){
    	key[i] = MAX_COST;  
		mstSet[i] = false; 						// nenhum v�rtice est� no MST ainda
	}
	
	// inclue o primeiro v�rtice no MST, n�o importa por onde come�a o algoritmo, o MST final � �nico
    key[1] = 0;     			// faz o valor chave zero, para que, assim, esse primeiro v�rtice seija escolhido primeiro
    parent[1] = -1; 			// primeiro n� � a raiz do MST
 	
 	k = 1; 		// inicializa o contador para os selecionados para o MST
 	
    // O MST ter� gr.v v�rtices
    for (i = 1; i <= gr.v; i++)
    {
    	// escolha o v�rtice com m�nimo valor chave do conjunto de v�rtices, ainda n�o inslusos no MST
        u = minKey(key, mstSet, gr);
 
        // adiciona o v�rtice escolhido no conjunto do MST
        mstSet[u] = true; 
        ord [k] = u;
        k++;
		
 
        // atualiza o valor chave e o �ndice do pai do v�rtice adjacente ao v�rtice escolhido 
        // considera apenas aqueles v�rtices que ainda n�o foram inclusos no MST
        for (v = 1; v <= gr.v; v++)
           // gr.custo [u][v] � diferente de zero apenas para v�rtices adjacentes
           // mstSet[v] � falso para os v�rtices ainda n�o inclusos no MST
           // atualiza o valor chave, somente se gr.custo[u][v] � menor do que key[v]
        	if (gr.custo[u][v] != 0 && mstSet[v] == false && gr.custo[u][v] <  key[v]){
       			parent[v]  = u;			
				key[v] = gr.custo[u][v];
			} 
	}
	
    // escreve no arquivo de sa�da o MST constru�do
    printMST(parent, ord, gr, out);
    
    free(parent);
    free(key);
    free(mstSet);
    
}

void BubbleSort (int n, struct aresta ar[]){						// algoritmo de ordena��o BubbleSort
	int i, p;					// vari�veis auxiliares de la�o
	struct aresta aux;			// struct auxiliar do tipo aresta para fazer a troca de posi��o de arestas para a ordem crescente de custo
	bool trocou;				// vari�vel de otimiza��o do algoritmo bubble sort, n�o precisa finalizar o la�o externo, apenas at� encontrar uma situa��o que n�o seja necess�rio trocar a posi��o de ningu�m
	trocou = true;										// inicializa��o como true para que o primeiro la�o seja executado
	for (p = n-1; p >= 1 && trocou; p--){
		trocou = false;									// trocou de volta para false
		for (i = 1; i <= p; i++){						// aqui h� outra otimiza��o: n�o � necess�rio ir at� o final do vetor de arestas, j� que as "posi��es finais" j� est�o determinadas
			if (ar[i].custo > ar[i+1].custo){			// caso ordem decrescente, troca
				aux = ar[i];
				ar[i] = ar[i+1];
				ar[i+1] = aux;
				trocou = true;							// trocou passa a ser true
			}
		}
	}
}
