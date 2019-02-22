/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 07: Fibra Ótica                          */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COST 999 // custos inteiros menores que 1000

//-------------Definição da estrutura de dados---------------------
struct grafo{							// strct que define como será armazenado o grafo, nesse caso: matriz de adjacência
	int v,								// armazena o número de vértices do grafo		
		a;								// armazena o número de arestas do grafo
	char name[159][19];					// vetor de strings com a pocição dos nomes dos prédios
	int  custo[159][159],				// matriz de adjacência que guarda os custos das arestas, caso o custo[i][j] seja zero, não há caminho entre i e j
		 	   grau[159];    			// vetor de inteiros que armazena o grau de cada vértice do grafo
};

struct aresta {							// struct que define uma aresta, ou seja, os vértices com os quais se conecta, além do custo (peso) da aresta
	char predioA[19],					// string predioA é auxiliar para armazenar os prédios a serem adicionados no grafo
		 predioB[19];					// string predioB é auxiliar para armazenar os prédios a serem adicionados no grafo
	int 	   custo;					// custo é auxiliar para guardar o custo (distância) entre o predioA e o predioB
};

//-------------Protótipos de funções--------------------------------
void inicializaGrafo(struct grafo *gr);								// inicializa a matriz adjunta com todos os custos zero, até que sejam inseridas aresta
void insereAresta (struct grafo *gr, int p1, int p2, int custo);	// função que insere aresta entre dois vértices do grafo
void insereVertice (struct grafo *gr, char predio[]);				// função que insere novo vértice no vetor de posições 
int busca (struct grafo gr, char predio[]);							// função que retorna a posição de um prédio no vetor de posições
int posVizinho (struct grafo gr, int predio, int viz);				// função que retorna a posição de um vizinho de um vértice, a partir da ordem do vizinho na lista de vizinhos
void BubbleSort (int n, struct aresta ar[]);						// algoritmo de ordenação BubbleSort

//-----------Funções Relacionadas ao Algoritmo de Prim--------------
void algPrim(struct grafo gr, FILE *out);							// função que implementa algoritmo de Prim para achar a árvore de cobertura de custo mínimo (MST - Minimum Spanning Tree)
void printMST(int parent[], int ord[], struct grafo gr, FILE *out);			// Função auxiliar para escrever no arquivo de saída o MST construído guardado no vetor de pais parent[] 
int minKey(int key[], bool mstSet[], struct grafo gr);				// Função auxiliar para encontrar o vértice com valor chave mínimo do conjunto de vértices ainda não inclusos no MST

int main(){
	struct   grafo gr;						// gr armazenará a estrutura do grafo 
	struct  aresta ar;						// ar armazenará a aresta a ser colocada no grafo
	char     line[79];       				// string line é auxiliar para pular linha 
    int            p1,						// auxiliar que guarda a posição de um vértice no vetor de posições
    			   p2,						// auxiliar que guarda a posição de um vértice no vetor de posições
					i,					 	// inteiro i é variável auxiliar que serve para iterações (laços)
					j;					 	// inteiro j é variável auxiliar que serve para iterações (laços)

    FILE *in, *out; 	 					// variáveis para os aqrquivos de entrada e saída
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
    //Criação do Cabeçalho do arquivo de saída
    fprintf(out,"*FIBRA OTICA*\n");
    fprintf(out,"Make It Simple But Significant!\n");
    fprintf(out,"Prim's Algorithm!\n");
    fprintf(out,"CES-11 - ITA - Prof.: Armando\n");
    fprintf(out,"--------------------------------------------------\n");
    fprintf(out, "Ordem de selecao pelo algoritmo\n");
    fprintf(out,"--------------------------------------------------\n");
    //Fim do cabeçalho do arquivo de saída

    for(i = 1; i <= 6; i++){// Pular as 6 primeiras linhas
        fgets(line,79,in);
    }
    
    //---------------Inicialização do grafo--------------
	inicializaGrafo(&gr);						// inicializa a matriz adjunta com todos os custos zero, até que sejam inseridas arestas

	while (!feof(in)){
		fscanf(in,"%s %s %d", ar.predioA, ar.predioB, &ar.custo);   	// lê cada aresta a ser adicionada presente em cada linha do arquivo 
		insereVertice (&gr, ar.predioA);								// insere o primeiro prédio no vetor de posições
		insereVertice (&gr, ar.predioB);								// insere o segundo prédio no vetor de posições
		p1 = busca(gr, ar.predioA);										// guarda a posição do primeiro prédio em p1
		p2 = busca(gr, ar.predioB);										// guarda a posição do primeiro prédio em p2
		insereAresta (&gr, p1, p2, ar.custo);							// insere aresta (custo) entre o primeiro e o segundo prédio
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
	
	// TESTE 2 - Vetor de Posições
	printf("\n\nTESTE 2 - Vetor de Posicoes");
	for (i = 1; i <= gr.v; i++){
		printf("\n%s",gr.name[i]);
	}
    
    // TESTE 3 - Número de Vértices e Arestas
    printf("\n\nTESTE 3 - Numero de Vertices e Arestas");
    printf("\n(V,A) = (%d,%d)", gr.v, gr.a);
    
    // TESTE 4 - Grau de Cada Vértice
    printf("\n\nTESTE 4 - Grau de Cada Vertice");
    for (i = 1; i <= gr.v; i++){
    	printf("\n grau (%d) = %d",i, gr.grau[i]);
	}
    */
    
    //---------------Chamada do Algoritmo de Prim--------------
	algPrim(gr, out);												// chamada da função de Prim
	
    // Fechamento dos arquivos de entrada e saída
	fclose(in);
    fclose(out); 
    
    return 0;
}	

void inicializaGrafo(struct grafo *gr){		// inicializa a matriz adjunta com todos os custos zero, até que sejam inseridas aresta
	int i, j;								// variáveis auxiliares de laços
	gr->v = 0;								// não há vértices no início
	gr->a = 0;								// não há arestas no início
	for (i = 1; i <= 150; i++){				// quantidade de prédios: no máximo 150 
		for (j = 1; j <= 150; j++){			
			gr->custo[i][j] = 0;			// no início, todos os custos são zero
		}
		gr->grau[i] = 0;					// grau de todos os vértices é zero no início
	}
}

void insereAresta (struct grafo *gr, int p1, int p2, int custo){	// função que insere aresta entre dois vértices do grafo
	gr->custo[p1][p2] = custo;										// atribuição de custo na matriz de adjacência de custos
	gr->custo[p2][p1] = custo;										// atribuição de custo na matriz de adjacência de custos
	gr->a++;														// incrementa a quantidade de arestas do grafo
	gr->grau[p1]++;													// incrementa o grau do primeiro vértice da nova aresta
	gr->grau[p2]++;													// incrementa o grau do segundo vértice da nova aresta 
	// note que a matriz será simétrica, como esperado
}

void insereVertice (struct grafo *gr, char predio[]){	// função que insere novo vértice no vetor de posições 
	if (busca (*gr, predio) == -1){						// caso o prédio ainda não exista no vetore de posições
		gr->v++;										// incremeta a quantidade vértices do grafo
		strcpy (gr->name[gr->v], predio);				// copia para o vetor de posições o novo prédio inserido
	}
	// caso o vértice já exista, não é feito nada
}

int busca (struct grafo gr, char predio[]){							// função que retorna a posição de um prédio no vetor de posições
	int i;
	for (i = 1; i <= gr.v && strcmp(gr.name[i],predio) != 0; i++);		// percorre o vetore de posições até achar a posição do elemento ou até o final
	if ( i == gr.v + 1){		// caso não tenha encontrado o elemento, retorna -1
		return -1;
	}
	else{				// caso tenha encontrado o elemento, retorna sua posição
		return i;		// quando o laço acaba porque achou o elemento, i armazena a posição desejada	
	}
}

//---------------Funções Relacionadas ao Algoritmo de Prim--------------

// Função auxiliar para escrever no arquivo de saída o MST construído guardado no vetor de pais parent[] 
void printMST(int parent[], int ord[], struct grafo gr, FILE *out)
{	
	int min, orig, dest, k;
	int custo_total = 0,				// inicializa o custo total do MST como zero
   				     i, 				// variável auxiliar para laço
					 j;					// variável auxiliar para contabilizar quantas arestas serão salvas em um vetor de arestas, para que possam, em seguida, serem ordenadas em ordem crescente de custo
	struct aresta *ar;					// ponteiro struct para declaração do vetor de arestas do MST

	ar = (struct aresta*)malloc((gr.v)*sizeof(struct aresta)); 				// alocação dinâmica do vetor de arestas no MST construído; há necessariamente o número de vértices - 1 arestas
	
	j = 1;
	for (i = 2; i <= gr.v; i++){																						// não é necessário escrever o primeiro, já que não tem pai	
		fprintf(out, "%-11s%-11s%3d\n", gr.name[parent[ord[i]]], gr.name[ord[i]], gr.custo[ord[i]][parent[ord[i]]]);	// escreve no arquivo de saída em ordem ordem de selecao pelo algoritmo de Prim							
		strcpy (ar[j].predioA, gr.name[parent[i]]);																		// copia o nome do primeiro prédio para a struct
		strcpy (ar[j].predioB, gr.name[i]);																				// copia o nome do segundo prédio para a struct
		ar[j].custo = gr.custo[i][parent[i]];																			// copia o custo da aresta para a struct
		custo_total+=gr.custo[i][parent[i]];																			// incrementa no custo total do MST o custo da aresta
		j++;
	}


    fprintf(out, "\nCusto total: %d\n", custo_total);														// escreve no arquivo de saída o custo total do MST
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

// Função auxiliar para encontrar o vértice com valor chave mínimo do conjunto de vértices ainda não inclusos no MST
int minKey(int key[], bool mstSet[], struct grafo gr)
{
   
	int min = MAX_COST, 			// inicializa o valor mínimo
   			 min_index,				// índice do vértice com mínimo valor chave
   			     	 v;				// variável auxiliar de laço
	for (v = 1; v <= gr.v; v++){
		if (mstSet[v] == false && key[v] < min){ 		// encontrar o vértice com valor chave mínimo do conjunto de vértices ainda não inclusos no MST
			min = key[v];								// atualiza o valor chave de mínimo
			min_index = v;								// atualiza o índice do vértice de valor chave de mínimo
		}
	}
        
   return min_index;
}

// Função para construir e mostrar o MST (Minimum Spanning Tree) de um grafo representado por matriz de adjacência
void algPrim(struct grafo gr, FILE *out)
{
	int *parent,	 								// vetor para guardar o MST  
    	   *key,   									// valores chave usados para escolher a ponta de mínimo custo em corte 
    	   *ord,									// ponteiro para um vetor que guardará a ordem dos elementos adicionados no MST
    	      i,									// variável auxiliar em laço 
    	      v,									// variável auxiliar em laço
    	      u,									// variável auxiliar que guarda o mínimo valor chave do vetor de vértices ainda não inclusos no MST, em um determinado momento
    	      k;									// contador auxiliar para o vetor da ordem dos vértices adicionados no MST
    bool *mstSet;  									// para representar o conjunto de vértices ainda não inclusos no MST, true para insluso e false para não incluso 
	ord = (int*) malloc((gr.v+1)*sizeof(int));				
    parent = (int*) malloc((gr.v+1)*sizeof(int));
    key = (int*) malloc ((gr.v+1)*sizeof (int));
    mstSet = (bool*) malloc ((gr.v+1)*sizeof (bool));

    // inicializa todos os valores chave como máximo, no caso o máximo custo é a constante MAX_COST
    for (i = 1; i <= gr.v; i++){
    	key[i] = MAX_COST;  
		mstSet[i] = false; 						// nenhum vértice está no MST ainda
	}
	
	// inclue o primeiro vértice no MST, não importa por onde começa o algoritmo, o MST final é único
    key[1] = 0;     			// faz o valor chave zero, para que, assim, esse primeiro vértice seija escolhido primeiro
    parent[1] = -1; 			// primeiro nó é a raiz do MST
 	
 	k = 1; 		// inicializa o contador para os selecionados para o MST
 	
    // O MST terá gr.v vértices
    for (i = 1; i <= gr.v; i++)
    {
    	// escolha o vértice com mínimo valor chave do conjunto de vértices, ainda não inslusos no MST
        u = minKey(key, mstSet, gr);
 
        // adiciona o vértice escolhido no conjunto do MST
        mstSet[u] = true; 
        ord [k] = u;
        k++;
		
 
        // atualiza o valor chave e o índice do pai do vértice adjacente ao vértice escolhido 
        // considera apenas aqueles vértices que ainda não foram inclusos no MST
        for (v = 1; v <= gr.v; v++)
           // gr.custo [u][v] é diferente de zero apenas para vértices adjacentes
           // mstSet[v] é falso para os vértices ainda não inclusos no MST
           // atualiza o valor chave, somente se gr.custo[u][v] é menor do que key[v]
        	if (gr.custo[u][v] != 0 && mstSet[v] == false && gr.custo[u][v] <  key[v]){
       			parent[v]  = u;			
				key[v] = gr.custo[u][v];
			} 
	}
	
    // escreve no arquivo de saída o MST construído
    printMST(parent, ord, gr, out);
    
    free(parent);
    free(key);
    free(mstSet);
    
}

void BubbleSort (int n, struct aresta ar[]){						// algoritmo de ordenação BubbleSort
	int i, p;					// variáveis auxiliares de laço
	struct aresta aux;			// struct auxiliar do tipo aresta para fazer a troca de posição de arestas para a ordem crescente de custo
	bool trocou;				// variável de otimização do algoritmo bubble sort, não precisa finalizar o laço externo, apenas até encontrar uma situação que não seja necessário trocar a posição de ninguém
	trocou = true;										// inicialização como true para que o primeiro laço seja executado
	for (p = n-1; p >= 1 && trocou; p--){
		trocou = false;									// trocou de volta para false
		for (i = 1; i <= p; i++){						// aqui há outra otimização: não é necessário ir até o final do vetor de arestas, já que as "posições finais" já estão determinadas
			if (ar[i].custo > ar[i+1].custo){			// caso ordem decrescente, troca
				aux = ar[i];
				ar[i] = ar[i+1];
				ar[i+1] = aux;
				trocou = true;							// trocou passa a ser true
			}
		}
	}
}
