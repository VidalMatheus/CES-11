/*  Copyright by Matheus Vidal de Menezes        			  */
/*  Turma 4                                  	 			  */
/*  Lab 08: PERT - Program Evaluation and Review Tecnique     */
/*  Programa compilado com Dev-C++ 5.11          			  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------Marcas de um v�rtice--------------------------------
#define BRANCO		-1
#define DESCOBERTO   0
#define VISITADO  	 1

//-------------Defini��o da estrutura de dados---------------------
struct vertice{							// struct que define um v�rtice, com letra, descri��o e prazo
	char      letra,					// string letra � auxiliar para armazenar as tarefas a serem adicionados no grafo
		   desc[39],					// string desc � auxiliar para armazenar as descri��es das tarefas a serem adicionados no grafo  
 		   anterior;					// caractere que guarda a posi��o do v�rtice precedente; isso ser� importante para a escrita do caminho cr�tico, ap�s do c�lculos dos TMT
	int  	  prazo,					// inteiro prazo � auxiliar para armazenar a dura��o da tarefa
		  	  marca;					// inteiro que armazena a marca como: BRANCO, DESCOBERTO ou VISITADO
};

struct grafo{								// strct que define como ser� armazenado o grafo, nesse caso: matriz de adjac�ncia
	int 				 v,					// armazena o n�mero de v�rtices do grafo		
						 a,					// armazena o n�mero de arestas do grafo
			   adj[59][59],					// matriz de adjac�ncia que guarda as conex�es das arestas orientadas, caso o adj[i][j] seja zero, n�o h� caminho entre i e j, caso contr�rio, h� de i para j
			  iniciais[59],					// vetor que armanezar� os v�rtices sem precedentes, o que ser� necess�rio no c�lculos do TMT de cada v�rtice do grafo
			   contSemPrev,					// contador para quantos v�rtices h� no grafo sem precedentes
			       TMT[59];					// vetor com o TMT de cada v�rtice
	struct vertice ver[59];					// vetor de v�rtices do grafo 
};

struct pilha{							// estrutura de pilha a ser usada na ordena��o topol�gica e na determina��o de ciclos
	int    topo,						// inteiro para o topo da pilha
		vet[59];						// pilha implementada com pilha
};

//-------------Prot�tipos de fun��es--------------------------------
void inicializaGrafo(struct grafo *gr);											// inicializa a matriz adjunta com todos zero, at� que sejam inseridas aresta
void insereAresta (struct grafo *gr, int p1, int p2);							// fun��o que insere aresta entre dois v�rtices do grafo
void insereVertice (struct grafo *gr, char letra, char desc[], int prazo);		// fun��o que insere novo v�rtice no vetor de posi��es 
void inserePrecedente (struct grafo *gr, char letra);							// fun��o que insere o precedente de um v�rtice 
int buscaPorLetra (struct grafo gr, char letra);								// fun��o que retorna a posi��o de uma tarefa no vetor de posi��es
int buscaPorDesc (struct grafo gr, char desc[]);

//-------------Valida��o de grafos ac�clicos------------------------
int hasCycle (struct grafo *gr, struct pilha *p, int *begin, struct pilha *s);		// Retorna verdadeiro se houver um ciclo no gr�fico
int DFS (struct grafo *gr, int noh, struct pilha *p, int *begin, struct pilha *s);	// Fun��o recursiva para descobrir se h� borda posterior na �rvore de sub-�rvore DFS enraizada com "noh"

//-------------Fun��es de Pilha (T.A.D)-----------------------------
// A pilha ser� utilizada na implementa��o do DFS em p�s-ordem para guardar um poss�vel ciclo do grafo
void inicializaPilha (struct pilha *p);								// fun��o que inicializa pilha
void empilha (struct pilha *p, int novo);							// fun��o que empilha novo elemento
int desempilha (struct pilha *p);									// fun��o que desempilha
int pilhaVazia (struct pilha *p);									// fun��o que verifica se a pilha est� vazia, caso vazia, retorna 1 (true), caso contr�rio, retorna 0 (false)
void esvaziarPilha (struct pilha *p);								// fun��o que esvazia a pilha



int main(){
	struct     grafo gr;						// gr armazenar� a estrutura do grafo 
	struct  vertice ver;						// ver armazenar� o v�rtice a ser colocada no grafo
	struct 	   pilha  p,						// p armazenar� a pilha a ser usada no DFS 
					  s;						// s armazenar� a pilha a ser usada para guardar a ordem topol�gica do grafo orientado sem ciclo			
	char       line[79],       					// string line � auxiliar para pular linha 
	    precedente[100],						// vetor auxiliar de r�tulos (caracteres) qua armazenar os precedentes do v�rtice a ser adicionado no grafo, tamanho no m�ximo 50 (v�rtices) + 49 (v�rgulas) + 1
	    			ant;						// ant � um caractere auxiliar para armazenar o r�tulo da tarefa anterior no caminho cr�tico do grafo
    int              p1,						// auxiliar que guarda a posi��o de um v�rtice no vetor de posi��es
    			     p2,						// auxiliar que guarda a posi��o de um v�rtice no vetor de posi��es
					  i,					 	// inteiro i � vari�vel auxiliar que serve para itera��es (la�os)
					  j,					 	// inteiro j � vari�vel auxiliar que serve para itera��es (la�os)
				  begin,						// armazenar� o "in�cio de um ciclo" do grafo, caso houver
				    max,						// max � um inteiro auxiliar para descobrir o m�ximo de uma lista de n�meros; ser� utilizado no c�lculo do TMT do grafo
				    fim;						// fim armanezar� o �ndice do �ltimo v�rtice do caminho cr�tico

    FILE *in, *out; 	 						// vari�veis para os aqrquivos de entrada e sa�da
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
    //Cria��o do Cabe�alho do arquivo de sa�da
    fprintf(out,"*PERT - Program Evaluation and Review Tecnique*\n");
    fprintf(out,"Simple But Very Useful!\n");
    fprintf(out,"ITA - Technological Institute of Aeronautics\n");
    fprintf(out,"CES-11\n");
    fprintf(out,"Prof.: Armando\n");
    fprintf(out,"------------------------------------------------------------\n\n");
    //Fim do cabe�alho do arquivo de sa�da
	
    for(i = 1; i <= 7; i++){// Pular as 7 primeiras linhas
        fgets(line,79,in);
    }
    
    //---------------Inicializa��o do grafo--------------
	inicializaGrafo(&gr);						// inicializa a matriz de adjac�ncia com todos zero, at� que sejam inseridas arestas
	
	//---------------Aquisi��o de Dados do Grafo---------
	while (!feof(in)){
		fscanf(in,"  %c   ", &ver.letra);		 					// l� cada aresta a ser adicionada presente em cada linha do arquivo 
		if ( ver.letra != '-'){										// caso n�o seja a �ltima linha do arquivo
			fgets(ver.desc,31,in);									// l� a descri��o da tarefa (at� 31 caracteres)
			fscanf(in,"%d", &ver.prazo);							// l� a dura��o ou prazo para se executar a tarefa
			insereVertice (&gr, ver.letra, ver.desc, ver.prazo);	// insere o v�rtice no vetor de posi��es
			p1 = buscaPorLetra(gr, ver.letra);						// guarda a posi��o do v�rtice
			fscanf (in,"%s",precedente);							// guarda uma string de r�tulos de precedentes separados por v�rgula, sem espa�os
			
			// Teste - Entrada
			printf("%c %s %2d %s\n", ver.letra, ver.desc, ver.prazo, precedente);
			
			for ( i = 0; i < strlen (precedente); i++){							// l� caractere por caractere do vetor de precedentes
				if ( precedente[i] != '.'){										// caso tenha precedente, o caractere � diferente de ".", que indica a aus�ncia de precedentes
					if ( precedente[i] != ','){									// pula as v�rgulas
						inserePrecedente (&gr, precedente[i]);					// insere o precedente no vetor de posi��es
						p2 = buscaPorLetra(gr, precedente[i]);					// guarda a posi��o do precedente
						insereAresta (&gr, p1, p2);							    // insere aresta orientada de P2 para P1
					}
				}
				if ( precedente[i] == '.') {							// v�rtice sem precedentes
					gr.iniciais[++gr.contSemPrev] = p1;								// atribui��o da posi��o do v�rtice sem precedentes a uma posi��o o vetor de v�rtices sem precedentes
					
					
				}
			}
		}
    } 
    
    
	printf("numero de vertices = %d\n", gr.v);
	system("PAUSE");
    // TESTE 1 - Matriz de Adjac�ncia
    printf("\n\nTESTE 1 - Matriz de Adjacencia\n");
    for (i = 1; i <= gr.v; i++){
    	for (j = 1; j <= gr.v; j++){
    		printf("%d", gr.adj[i][j]);
		}
		printf("\n");
	}																			
	system("PAUSE");
	// TESTE 2 - Vetor de Posi��es
	printf("\n\nTESTE 2 - Vetor de Posicoes");
	for (i = 1; i <= gr.v; i++){
		printf("\n%s",gr.ver[i].desc);
	}
	printf("\n");
	system("PAUSE");
	// TESTE 3 - Vetor de Posi��es
	printf("\n\nTESTE 3 - Vetor de Posicoes");
	for (i = 1; i <= gr.v; i++){
		printf("\n%c",gr.ver[i].letra);
	}
	printf("\n");
    system("PAUSE");
    // TESTE 4 - N�mero de V�rtices e Arestas
    printf("\n\nTESTE 4 - Numero de Vertices e Arestas");
    printf("\n(V,A) = (%d,%d)", gr.v, gr.a);
    printf("\n");
    system("PAUSE");
    
    
    //---------------Verifica se h� ciclo-------------------
    inicializaPilha(&p);												// inicializa a pilha que ser� utilizada no DFS
    inicializaPilha(&s);												// inicializa a pilha que ser� utilizada no DFS para armazernar a ordem topol�gica do grafo orientado
	if (hasCycle(&gr, &p, &begin, &s)){									// chama a fun��o que verifica se h� ciclo
		printf("	Ciclo:\n");											
		for(i = begin; i <= p.topo; i++){								// imprime os elementos do "in�cio" at� o topo da pilha
			printf("%c ", gr.ver[p.vet[i]].letra);							
		}
		printf("\n E impossivel buscar o caminho critico.\n");			// Aviso na tela de que n�o � poss�vel prosseguir para determina��o da ordem topol�gica
		esvaziarPilha  (&p);											// finaliza a pilha
	}
	else {																// caso contr�rio, o grafo n�o tem ciclo
		printf("\nNao e ciclico!\n");
		
		//---------------Ordena��o Topol�gica----------------
		fprintf(out,"UMA ORDENACAO TOPOLOGICA:\n\n");
		for(i = s.topo; i >= 1; i--){								// imprime os elementos do topo da pilha at� o in�cio
			fprintf(out, "%c ", gr.ver[s.vet[i]].letra);
			printf(" custo %c = %2d\n", gr.ver[s.vet[i]].letra, gr.ver[s.vet[i]].prazo);
		}
		fprintf(out,"\n\n------------------------------------------------------------\n\n");
		
		//---------------Caminho Cr�tico (CPM - Critical Path Method)----------------
		fprintf(out,"CAMINHO CRITICO:\n\n");
		fprintf(out,"TAREFA        DESCRICAO           DURACAO\n\n");
		
		//---------------C�lculo dos TMT (Tempo M�nimo de T�rmino) dos V�rtices----------------
		
		// Primeiro devemos atribuir os TMT dos v�rtices sem precedentes como sendo suas respectivas dura��es
		for (i = 1; i <= gr.contSemPrev; i++){
			printf("	%d", gr.iniciais[i]);
		}
		system("PAUSE");
		for (i = 1; i <= gr.contSemPrev; i++){
			gr.TMT[i] = gr.ver[gr.iniciais[i]].prazo;			// atribui��o a dura��o de cada n� sem pr�-requisito ao TMT
			gr.ver[gr.iniciais[i]].anterior = '.';				// indicando que � um v�rtice sem precedentes
		}
		
		
		// Agora, vamos calcular o TMT de cada v�rtice em ordem topol�gica
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
		
		// Basta agora descobrir o m�ximo TMT calculado, e esse ser� o TMT do projeto como um todo
		max = gr.TMT[1];
		for (i = 1; i <= gr.v; i++){
			if (max < gr.TMT[i]){
				max = gr.TMT[i];
				fim = i;
			}
				
		}
		
		inicializaPilha(&p);
		empilha(&p, fim);
		ant = gr.ver[fim].anterior;					// r�tulo anterior
		while( ant != '.'){							// caso n�o tenha chegado em um v�rtice sem precedentes, continua
			fim = buscaPorLetra(gr, ant);			// novo fim � o v�rtice anterior
			empilha(&p, fim);						// empilha a posi��o do anterior
			ant = gr.ver[fim].anterior;				// armazena o r�tulo anterior
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
    // Fechamento dos arquivos de entrada e sa�da
	fclose(in);
    fclose(out); 
    
    return 0;
}	

void inicializaGrafo(struct grafo *gr){		// inicializa a matriz de adjac�ncia com todas as posi��es zero, at� que sejam inseridas arestas
	int i, j;								// vari�veis auxiliares de la�os
	gr->v = 0;								// n�o h� v�rtices no in�cio
	gr->a = 0;								// n�o h� arestas no in�cio
	gr->contSemPrev = 0;					// atribui zero � quantidade de v�rtices sem precedentes
	for (i = 1; i <= 50; i++){				// quantidade de tarefas: no m�ximo 50 
		for (j = 1; j <= 50; j++){			
			gr->adj[i][j] = 0;				// no in�cio, todos os custos s�o zero
		}
		gr->iniciais[i] = 0;				// n�o h� v�rtices sem precedentes
		gr->TMT[i] = 0;						// atribui zero a todos os TMT 
	}
}

void insereAresta (struct grafo *gr, int p1, int p2){	    	    // fun��o que insere aresta entre dois v�rtices do grafo
	gr->adj[p2][p1] = 1;											// atribui��o de custo na matriz de adjac�ncia
	gr->a++;														// incrementa a quantidade de arestas do grafo 
	// note que a matriz pode n�o ser sim�trica, como esperado para um grafo orientado
}

void insereVertice (struct grafo *gr, char letra, char desc[], int prazo){			// fun��o que insere novo v�rtice nos vetores de posi��es 
	
	if (buscaPorDesc (*gr, desc) == -1){								// caso a tarefa ainda n�o exista no vetor de descri��es
		printf(" letra = %c		", letra);
		if (buscaPorLetra (*gr, letra) == -1){							// caso a tarefa ainda n�o exista no vetor de r�tulos
			gr->v++;													// incremeta a quantidade v�rtices do grafo
			strcpy (gr->ver[gr->v].desc, desc);							// copia para o vetor de posi��es a descri��o da nova tarefa a ser inserida
			gr->ver[gr->v].letra = letra;								// copia para o vetor de posi��es o r�tulo da nova tarefa a ser inserida
			gr->ver[gr->v].prazo = prazo;								// copia o prazo da tarefa
		}
		else{															// caso a tarefa j� exista no vetor de r�tulos, por�m n�o existe no vetor de descri��es
			strcpy (gr->ver[buscaPorLetra (*gr, letra)].desc, desc);	// copia para o vetor de posi��es a descri��o da nova tarefa a ser inserida
			gr->ver[buscaPorLetra (*gr, letra)].prazo = prazo;			// copia o prazo da tarefa			
		}
	}
	// caso o v�rtice j� exista, n�o � feito nada
}

void inserePrecedente (struct grafo *gr, char letra){					// fun��o que insere o r�tulo do precedente de um v�rtice
	if (buscaPorLetra (*gr, letra) == -1){								// caso a tarefa ainda n�o exista no vetor de r�tulos 
		gr->v++;														// incremeta a quantidade v�rtices do grafo
		gr->ver[gr->v].letra = letra;									// copia para o vetor de posi��es o r�tulo da nova tarefa a ser inserida
	}
	// caso o v�rtice j� exista, n�o � feito nada
}

int buscaPorLetra (struct grafo gr, char letra){				// fun��o que retorna a posi��o de uma tarefa no vetor de r�tulos
	int i;														// vari�vel auxiliar para loop
	for (i = 1; i <= gr.v && gr.ver[i].letra != letra; i++);	// percorre o vetore de posi��es at� achar a posi��o do elemento ou at� o final
	if ( i == gr.v + 1)											// caso n�o tenha encontrado o elemento, retorna -1
		return -1;
	else 	return i;											// caso tenha encontrado o elemento, retorna sua posi��o
	// quando o la�o acaba, porque achou o elemento, i armazena a posi��o desejada	
}

int buscaPorDesc (struct grafo gr, char desc[]){						// fun��o que retorna a posi��o de uma tarefa no vetor de descri��es
	int i;																// vari�vel auxiliar para loop
	for (i = 1; i <= gr.v && strcmp(gr.ver[i].desc,desc) != 0; i++);	// percorre o vetore de posi��es at� achar a posi��o do elemento ou at� o final
	if ( i == gr.v + 1)													// caso n�o tenha encontrado o elemento, retorna -1
		return -1;
	else	return i;													// caso tenha encontrado o elemento, retorna sua posi��o
	// quando o la�o acaba porque achou o elemento, i armazena a posi��o desejada	
}

//-------------Valida��o de grafos ac�clicos------------------------

// Fun��o recursiva para descobrir se h� borda posterior na �rvore de sub-�rvore DFS enraizada com "noh"
int DFS (struct grafo *gr, int noh, struct pilha *p, int *begin, struct pilha *s)
{
    int i;			// vari�vel auxiliar para loop
    	
	// DESCOBERTO :  o v�rtice est� come�ando a ser analizado ( DFS para esse v�rtice come�ou, mas n�o acabou, ou esse v�rtice est� em fun��o da pilha de chamadas)
    gr->ver[noh].marca = DESCOBERTO;									// marca como DESCOBERTO o v�rtice "noh"
    empilha(p,noh);														// empilha o "noh"
    for(i = 1;i <= gr->v; i++){											// percorre todos os v�rtices adjacentes a "noh", ou seja, os vizinhos de "noh"
    	if( gr->adj[noh][i] == 1){
			if (gr->ver[i].marca == DESCOBERTO){						// condi��o de parada para o fechamento do ciclo, caso o DFS encontre um v�rtice DESCOBERTO, mas n�o VISITADO
				*begin = i;												// salva a posi��o do elemento que inicia o ciclo encontrado
    			return 1;												// retorna true, ou seja, achou ciclo
			}
			
    		if(gr->ver[i].marca == BRANCO && DFS(gr, i, p, begin, s) == 1){	// Se i n�o for processado e h� um ciclo na sub�rvore enraizada em i, retorna true
    			return 1;
			}
    	}
	}
	desempilha(p);														// quando acabar os vizinhos, desempilha
    gr->ver[noh].marca = VISITADO;										// marca como VISITADO, ap�s descobrir os vizinhos do "noh", conforme o DFS em P�s-Ordem
    empilha (s, noh);													// empilha na pilha de ordem topol�gica
    return 0;
}

// Retorna verdadeiro se houver um ciclo no gr�fico
int hasCycle (struct grafo *gr, struct pilha *p, int *begin, struct pilha *s){	
	int 	    i;							// vari�vel auxiliar para loop
	for (i = 1; i <= gr->v; i++)
    	gr->ver[i].marca = BRANCO;				// todos os v�rtices come�am marcados como BRANCO
	for (i = 1; i <= gr->v; i++) {  		    // Fa�a um percurso DFS come�ando com todos v�rtices
		if (gr->ver[i].marca ==  BRANCO)		// fazer DFS em apenas v�rtices BRANCOS
    		if (DFS(gr,i, p, begin, s) == 1) 	// caso o DFS retorne true, o gr�fico tem ciclo
				return 1;
	}
	return 0;
}

//-------------Fun��es de Pilha------------------------

void inicializaPilha (struct pilha *p){	   // fun��o que inicializa pilha
	p->topo = 0;						   // basta colocar o topo no zero, j� que n�o h� elementos
	// o vetor de elementos j� est� alocado na mem�ria, vide a declara��o na main sem ponteiros de "struct pilha p"
}

void empilha (struct pilha *p, int novo){	// fun��o que emepilha novo elemento
	p->vet[++p->topo] = novo;				// basta colocar no novo elemento no topo+1
}

int desempilha (struct pilha *p){			// fun��o que desempilha
	return p->vet[p->topo--];				// basta retornar o elemento do topo e, em seguida, decrementar o topo
}

int pilhaVazia (struct pilha *p){			// fun��o que verifica se a pilha est� vazia
	if (p->topo == 0) return 1;				// basta verificar se o topo � zero, caso verdade, retorna 1 (true)
	else return 0;							// caso contr�rio, retorna 0 (false)
}

void esvaziarPilha  (struct pilha *p){		// fun��o que finaliza a pilha
	p->topo = 0;							// basta colocar o topo no zero, j� que considera-se que n�o h� mais elementos
}


