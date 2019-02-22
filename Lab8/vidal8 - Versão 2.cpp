/*  Copyright by Matheus Vidal de Menezes        			  */
/*  Turma 4                                  	 			  */
/*  Lab 08: PERT - Program Evaluation and Review Tecnique     */
/*  Programa compilado com Dev-C++ 5.11          			  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------Marcas de um vértice--------------------------------
#define BRANCO		-1
#define DESCOBERTO   0
#define VISITADO  	 1

//-------------Definição da estrutura de dados---------------------
struct vertice{							// struct que define um vértice, com letra, descrição e prazo
	char      letra,					// string letra é auxiliar para armazenar as tarefas a serem adicionados no grafo
		   desc[39],					// string desc é auxiliar para armazenar as descrições das tarefas a serem adicionados no grafo  
 		   anterior;					// caractere que guarda a posição do vértice precedente; isso será importante para a escrita do caminho crítico, após do cálculos dos TMT
	int  	  prazo,					// inteiro prazo é auxiliar para armazenar a duração da tarefa
		  	  marca;					// inteiro que armazena a marca como: BRANCO, DESCOBERTO ou VISITADO
};

struct grafo{								// strct que define como será armazenado o grafo, nesse caso: matriz de adjacência
	int 				 v,					// armazena o número de vértices do grafo		
						 a,					// armazena o número de arestas do grafo
			   adj[59][59],					// matriz de adjacência que guarda as conexões das arestas orientadas, caso o adj[i][j] seja zero, não há caminho entre i e j, caso contrário, há de i para j
			  iniciais[59],					// vetor que armanezará os vértices sem precedentes, o que será necessário no cálculos do TMT de cada vértice do grafo
			   contSemPrev,					// contador para quantos vértices há no grafo sem precedentes
			       TMT[59];					// vetor com o TMT de cada vértice
	struct vertice ver[59];					// vetor de vértices do grafo 
};

struct pilha{							// estrutura de pilha a ser usada na ordenação topológica e na determinação de ciclos
	int    topo,						// inteiro para o topo da pilha
		vet[59];						// pilha implementada com pilha
};

//-------------Protótipos de funções--------------------------------
void inicializaGrafo(struct grafo *gr);											// inicializa a matriz adjunta com todos zero, até que sejam inseridas aresta
void insereAresta (struct grafo *gr, int p1, int p2);							// função que insere aresta entre dois vértices do grafo
void insereVertice (struct grafo *gr, char letra, char desc[], int prazo);		// função que insere novo vértice no vetor de posições 
void inserePrecedente (struct grafo *gr, char letra);							// função que insere o precedente de um vértice 
int buscaPorLetra (struct grafo gr, char letra);								// função que retorna a posição de uma tarefa no vetor de posições
int buscaPorDesc (struct grafo gr, char desc[]);

//-------------Validação de grafos acíclicos------------------------
int hasCycle (struct grafo *gr, struct pilha *p, int *begin, struct pilha *s);		// Retorna verdadeiro se houver um ciclo no gráfico
int DFS (struct grafo *gr, int noh, struct pilha *p, int *begin, struct pilha *s);	// Função recursiva para descobrir se há borda posterior na árvore de sub-árvore DFS enraizada com "noh"

//-------------Funções de Pilha (T.A.D)-----------------------------
// A pilha será utilizada na implementação do DFS em pós-ordem para guardar um possível ciclo do grafo
void inicializaPilha (struct pilha *p);								// função que inicializa pilha
void empilha (struct pilha *p, int novo);							// função que empilha novo elemento
int desempilha (struct pilha *p);									// função que desempilha
int pilhaVazia (struct pilha *p);									// função que verifica se a pilha está vazia, caso vazia, retorna 1 (true), caso contrário, retorna 0 (false)
void esvaziarPilha (struct pilha *p);								// função que esvazia a pilha



int main(){
	struct     grafo gr;						// gr armazenará a estrutura do grafo 
	struct  vertice ver;						// ver armazenará o vértice a ser colocada no grafo
	struct 	   pilha  p,						// p armazenará a pilha a ser usada no DFS 
					  s;						// s armazenará a pilha a ser usada para guardar a ordem topológica do grafo orientado sem ciclo			
	char       line[79],       					// string line é auxiliar para pular linha 
	    precedente[100],						// vetor auxiliar de rótulos (caracteres) qua armazenar os precedentes do vértice a ser adicionado no grafo, tamanho no máximo 50 (vértices) + 49 (vírgulas) + 1
	    			ant;						// ant é um caractere auxiliar para armazenar o rótulo da tarefa anterior no caminho crítico do grafo
    int              p1,						// auxiliar que guarda a posição de um vértice no vetor de posições
    			     p2,						// auxiliar que guarda a posição de um vértice no vetor de posições
					  i,					 	// inteiro i é variável auxiliar que serve para iterações (laços)
					  j,					 	// inteiro j é variável auxiliar que serve para iterações (laços)
				  begin,						// armazenará o "início de um ciclo" do grafo, caso houver
				    max,						// max é um inteiro auxiliar para descobrir o máximo de uma lista de números; será utilizado no cálculo do TMT do grafo
				    fim;						// fim armanezará o índice do último vértice do caminho crítico

    FILE *in, *out; 	 						// variáveis para os aqrquivos de entrada e saída
	printf("\t\t----------Fibra Otica----------\n");
    in = fopen ("C:\\Lab8\\entrada8.txt", "r");
    if (in == NULL)
	{
	   	printf("\n\nPROBLEMAS NO ARQUIVO DE ENTRADA!\n");
	   	return 1;
	}
    out = fopen ("C:\\Lab8\\vidal8.txt", "w");
    if (out == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA!\n");
        return 1;
    }
    //Criação do Cabeçalho do arquivo de saída
    fprintf(out,"*PERT - Program Evaluation and Review Tecnique*\n");
    fprintf(out,"Simple But Very Useful!\n");
    fprintf(out,"ITA - Technological Institute of Aeronautics\n");
    fprintf(out,"CES-11\n");
    fprintf(out,"Prof.: Armando\n");
    fprintf(out,"------------------------------------------------------------\n\n");
    //Fim do cabeçalho do arquivo de saída
	
    for(i = 1; i <= 7; i++){// Pular as 7 primeiras linhas
        fgets(line,79,in);
    }
    
    //---------------Inicialização do grafo--------------
	inicializaGrafo(&gr);						// inicializa a matriz de adjacência com todos zero, até que sejam inseridas arestas
	
	//---------------Aquisição de Dados do Grafo---------
	while (!feof(in)){
		fscanf(in,"  %c   ", &ver.letra);		 					// lê cada aresta a ser adicionada presente em cada linha do arquivo 
		if ( ver.letra != '-'){										// caso não seja a última linha do arquivo
			fgets(ver.desc,31,in);									// lê a descrição da tarefa (até 31 caracteres)
			fscanf(in,"%d", &ver.prazo);							// lê a duração ou prazo para se executar a tarefa
			insereVertice (&gr, ver.letra, ver.desc, ver.prazo);	// insere o vértice no vetor de posições
			p1 = buscaPorLetra(gr, ver.letra);						// guarda a posição do vértice
			fscanf (in,"%s",precedente);							// guarda uma string de rótulos de precedentes separados por vírgula, sem espaços
			
			// Teste - Entrada
			printf("%c %s %2d %s\n", ver.letra, ver.desc, ver.prazo, precedente);
			
			for ( i = 0; i < strlen (precedente); i++){							// lê caractere por caractere do vetor de precedentes
				if ( precedente[i] != '.'){										// caso tenha precedente, o caractere é diferente de ".", que indica a ausência de precedentes
					if ( precedente[i] != ','){									// pula as vírgulas
						inserePrecedente (&gr, precedente[i]);					// insere o precedente no vetor de posições
						p2 = buscaPorLetra(gr, precedente[i]);					// guarda a posição do precedente
						insereAresta (&gr, p1, p2);							    // insere aresta orientada de P2 para P1
					}
				}
				if ( precedente[i] == '.') {							// vértice sem precedentes
					gr.iniciais[++gr.contSemPrev] = p1;								// atribuição da posição do vértice sem precedentes a uma posição o vetor de vértices sem precedentes
					
					
				}
			}
		}
    } 
    
    
	printf("numero de vertices = %d\n", gr.v);
	system("PAUSE");
    // TESTE 1 - Matriz de Adjacência
    printf("\n\nTESTE 1 - Matriz de Adjacencia\n");
    for (i = 1; i <= gr.v; i++){
    	for (j = 1; j <= gr.v; j++){
    		printf("%d", gr.adj[i][j]);
		}
		printf("\n");
	}																			
	system("PAUSE");
	// TESTE 2 - Vetor de Posições
	printf("\n\nTESTE 2 - Vetor de Posicoes");
	for (i = 1; i <= gr.v; i++){
		printf("\n%s",gr.ver[i].desc);
	}
	printf("\n");
	system("PAUSE");
	// TESTE 3 - Vetor de Posições
	printf("\n\nTESTE 3 - Vetor de Posicoes");
	for (i = 1; i <= gr.v; i++){
		printf("\n%c",gr.ver[i].letra);
	}
	printf("\n");
    system("PAUSE");
    // TESTE 4 - Número de Vértices e Arestas
    printf("\n\nTESTE 4 - Numero de Vertices e Arestas");
    printf("\n(V,A) = (%d,%d)", gr.v, gr.a);
    printf("\n");
    system("PAUSE");
    
    
    //---------------Verifica se há ciclo-------------------
    inicializaPilha(&p);												// inicializa a pilha que será utilizada no DFS
    inicializaPilha(&s);												// inicializa a pilha que será utilizada no DFS para armazernar a ordem topológica do grafo orientado
	if (hasCycle(&gr, &p, &begin, &s)){									// chama a função que verifica se há ciclo
		printf("	Ciclo:\n");											
		for(i = begin; i <= p.topo; i++){								// imprime os elementos do "início" até o topo da pilha
			printf("%c ", gr.ver[p.vet[i]].letra);							
		}
		printf("\n E impossivel buscar o caminho critico.\n");			// Aviso na tela de que não é possível prosseguir para determinação da ordem topológica
		esvaziarPilha  (&p);											// finaliza a pilha
	}
	else {																// caso contrário, o grafo não tem ciclo
		printf("\nNao e ciclico!\n");
		
		//---------------Ordenação Topológica----------------
		fprintf(out,"UMA ORDENACAO TOPOLOGICA:\n\n");
		for(i = s.topo; i >= 1; i--){								// imprime os elementos do topo da pilha até o início
			fprintf(out, "%c ", gr.ver[s.vet[i]].letra);
			printf(" custo %c = %2d\n", gr.ver[s.vet[i]].letra, gr.ver[s.vet[i]].prazo);
		}
		fprintf(out,"\n\n------------------------------------------------------------\n\n");
		
		//---------------Caminho Crítico (CPM - Critical Path Method)----------------
		fprintf(out,"CAMINHO CRITICO:\n\n");
		fprintf(out,"TAREFA        DESCRICAO           DURACAO\n\n");
		
		//---------------Cálculo dos TMT (Tempo Mínimo de Término) dos Vértices----------------
		
		// Primeiro devemos atribuir os TMT dos vértices sem precedentes como sendo suas respectivas durações
		for (i = 1; i <= gr.contSemPrev; i++){
			printf("	%d", gr.iniciais[i]);
		}
		system("PAUSE");
		for (i = 1; i <= gr.contSemPrev; i++){
			gr.TMT[i] = gr.ver[gr.iniciais[i]].prazo;			// atribuição a duração de cada nó sem pré-requisito ao TMT
			gr.ver[gr.iniciais[i]].anterior = '.';				// indicando que é um vértice sem precedentes
		}
		
		
		// Agora, vamos calcular o TMT de cada vértice em ordem topológica
		for(i = s.topo; i >= 1; i--){		
			max = gr.TMT[s.vet[1]];		
			for (j = 1; j <= gr.v; j++){
				if(gr.adj[s.vet[j]][s.vet[i]] == 1){				// analisar somente os precendentes
					if (max < gr.TMT[s.vet[j]]){
						max = gr.TMT[s.vet[j]];
						gr.ver[s.vet[i]].anterior = gr.ver[s.vet[j]].letra;
					}
				}
			}
			gr.TMT[s.vet[i]] = gr.ver[s.vet[i]].prazo + max;
		}
		
		// Basta agora descobrir o máximo TMT calculado, e esse será o TMT do projeto como um todo
		max = gr.TMT[1];
		for (i = 1; i <= gr.v; i++){
			if (max < gr.TMT[i]){
				max = gr.TMT[i];
				fim = i;
			}
				
		}
		
		inicializaPilha(&p);
		empilha(&p, fim);
		ant = gr.ver[fim].anterior;					// rótulo anterior
		while( ant != '.'){							// caso não tenha chegado em um vértice sem precedentes, continua
			fim = buscaPorLetra(gr, ant);			// novo fim é o vértice anterior
			empilha(&p, fim);						// empilha a posição do anterior
			ant = gr.ver[fim].anterior;				// armazena o rótulo anterior
		}
		
		printf("\n\n");
		for( i = p.topo; i >= 1; i--){
			fprintf(out, "  %c   %s %2d\n", gr.ver[p.vet[i]].letra, gr.ver[p.vet[i]].desc, gr.ver[p.vet[i]].prazo);
		}
		esvaziarPilha(&p);	
		
		fprintf(out, "                                   ----");
		fprintf(out, "\nTEMPO MINIMO PARA O PROJETO:         %2d semanas\n", max);
	}
	
	system("PAUSE");
    // Fechamento dos arquivos de entrada e saída
	fclose(in);
    fclose(out); 
    
    return 0;
}	

void inicializaGrafo(struct grafo *gr){		// inicializa a matriz de adjacência com todas as posições zero, até que sejam inseridas arestas
	int i, j;								// variáveis auxiliares de laços
	gr->v = 0;								// não há vértices no início
	gr->a = 0;								// não há arestas no início
	gr->contSemPrev = 0;					// atribui zero à quantidade de vértices sem precedentes
	for (i = 1; i <= 50; i++){				// quantidade de tarefas: no máximo 50 
		for (j = 1; j <= 50; j++){			
			gr->adj[i][j] = 0;				// no início, todos os custos são zero
		}
		gr->iniciais[i] = 0;				// não há vértices sem precedentes
		gr->TMT[i] = 0;						// atribui zero a todos os TMT 
	}
}

void insereAresta (struct grafo *gr, int p1, int p2){	    	    // função que insere aresta entre dois vértices do grafo
	gr->adj[p2][p1] = 1;											// atribuição de custo na matriz de adjacência
	gr->a++;														// incrementa a quantidade de arestas do grafo 
	// note que a matriz pode não ser simétrica, como esperado para um grafo orientado
}

void insereVertice (struct grafo *gr, char letra, char desc[], int prazo){			// função que insere novo vértice nos vetores de posições 
	
	if (buscaPorDesc (*gr, desc) == -1){								// caso a tarefa ainda não exista no vetor de descrições
		printf(" letra = %c		", letra);
		if (buscaPorLetra (*gr, letra) == -1){							// caso a tarefa ainda não exista no vetor de rótulos
			gr->v++;													// incremeta a quantidade vértices do grafo
			strcpy (gr->ver[gr->v].desc, desc);							// copia para o vetor de posições a descrição da nova tarefa a ser inserida
			gr->ver[gr->v].letra = letra;								// copia para o vetor de posições o rótulo da nova tarefa a ser inserida
			gr->ver[gr->v].prazo = prazo;								// copia o prazo da tarefa
		}
		else{															// caso a tarefa já exista no vetor de rótulos, porém não existe no vetor de descrições
			strcpy (gr->ver[buscaPorLetra (*gr, letra)].desc, desc);	// copia para o vetor de posições a descrição da nova tarefa a ser inserida
			gr->ver[buscaPorLetra (*gr, letra)].prazo = prazo;			// copia o prazo da tarefa			
		}
	}
	// caso o vértice já exista, não é feito nada
}

void inserePrecedente (struct grafo *gr, char letra){					// função que insere o rótulo do precedente de um vértice
	if (buscaPorLetra (*gr, letra) == -1){								// caso a tarefa ainda não exista no vetor de rótulos 
		gr->v++;														// incremeta a quantidade vértices do grafo
		gr->ver[gr->v].letra = letra;									// copia para o vetor de posições o rótulo da nova tarefa a ser inserida
	}
	// caso o vértice já exista, não é feito nada
}

int buscaPorLetra (struct grafo gr, char letra){				// função que retorna a posição de uma tarefa no vetor de rótulos
	int i;														// variável auxiliar para loop
	for (i = 1; i <= gr.v && gr.ver[i].letra != letra; i++);	// percorre o vetore de posições até achar a posição do elemento ou até o final
	if ( i == gr.v + 1)											// caso não tenha encontrado o elemento, retorna -1
		return -1;
	else 	return i;											// caso tenha encontrado o elemento, retorna sua posição
	// quando o laço acaba, porque achou o elemento, i armazena a posição desejada	
}

int buscaPorDesc (struct grafo gr, char desc[]){						// função que retorna a posição de uma tarefa no vetor de descrições
	int i;																// variável auxiliar para loop
	for (i = 1; i <= gr.v && strcmp(gr.ver[i].desc,desc) != 0; i++);	// percorre o vetore de posições até achar a posição do elemento ou até o final
	if ( i == gr.v + 1)													// caso não tenha encontrado o elemento, retorna -1
		return -1;
	else	return i;													// caso tenha encontrado o elemento, retorna sua posição
	// quando o laço acaba porque achou o elemento, i armazena a posição desejada	
}

//-------------Validação de grafos acíclicos------------------------

// Função recursiva para descobrir se há borda posterior na árvore de sub-árvore DFS enraizada com "noh"
int DFS (struct grafo *gr, int noh, struct pilha *p, int *begin, struct pilha *s)
{
    int i;			// variável auxiliar para loop
    	
	// DESCOBERTO :  o vértice está começando a ser analizado ( DFS para esse vértice começou, mas não acabou, ou esse vértice está em função da pilha de chamadas)
    gr->ver[noh].marca = DESCOBERTO;									// marca como DESCOBERTO o vértice "noh"
    empilha(p,noh);														// empilha o "noh"
    for(i = 1;i <= gr->v; i++){											// percorre todos os vértices adjacentes a "noh", ou seja, os vizinhos de "noh"
    	if( gr->adj[noh][i] == 1){
			if (gr->ver[i].marca == DESCOBERTO){						// condição de parada para o fechamento do ciclo, caso o DFS encontre um vértice DESCOBERTO, mas não VISITADO
				*begin = i;												// salva a posição do elemento que inicia o ciclo encontrado
    			return 1;												// retorna true, ou seja, achou ciclo
			}
			
    		if(gr->ver[i].marca == BRANCO && DFS(gr, i, p, begin, s) == 1){	// Se i não for processado e há um ciclo na subárvore enraizada em i, retorna true
    			return 1;
			}
    	}
	}
	desempilha(p);														// quando acabar os vizinhos, desempilha
    gr->ver[noh].marca = VISITADO;										// marca como VISITADO, após descobrir os vizinhos do "noh", conforme o DFS em Pós-Ordem
    empilha (s, noh);													// empilha na pilha de ordem topológica
    return 0;
}

// Retorna verdadeiro se houver um ciclo no gráfico
int hasCycle (struct grafo *gr, struct pilha *p, int *begin, struct pilha *s){	
	int 	    i;							// variável auxiliar para loop
	for (i = 1; i <= gr->v; i++)
    	gr->ver[i].marca = BRANCO;				// todos os vértices começam marcados como BRANCO
	for (i = 1; i <= gr->v; i++) {  		    // Faça um percurso DFS começando com todos vértices
		if (gr->ver[i].marca ==  BRANCO)		// fazer DFS em apenas vértices BRANCOS
    		if (DFS(gr,i, p, begin, s) == 1) 	// caso o DFS retorne true, o gráfico tem ciclo
				return 1;
	}
	return 0;
}

//-------------Funções de Pilha------------------------

void inicializaPilha (struct pilha *p){	   // função que inicializa pilha
	p->topo = 0;						   // basta colocar o topo no zero, já que não há elementos
	// o vetor de elementos já está alocado na memória, vide a declaração na main sem ponteiros de "struct pilha p"
}

void empilha (struct pilha *p, int novo){	// função que emepilha novo elemento
	p->vet[++p->topo] = novo;				// basta colocar no novo elemento no topo+1
}

int desempilha (struct pilha *p){			// função que desempilha
	return p->vet[p->topo--];				// basta retornar o elemento do topo e, em seguida, decrementar o topo
}

int pilhaVazia (struct pilha *p){			// função que verifica se a pilha está vazia
	if (p->topo == 0) return 1;				// basta verificar se o topo é zero, caso verdade, retorna 1 (true)
	else return 0;							// caso contrário, retorna 0 (false)
}

void esvaziarPilha  (struct pilha *p){		// função que finaliza a pilha
	p->topo = 0;							// basta colocar o topo no zero, já que considera-se que não há mais elementos
}


