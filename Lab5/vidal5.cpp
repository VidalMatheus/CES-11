/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 05: Índice de Livro                      */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------Definição da estrutura de dados---------------------
struct noh{
	char name[29]; 		// guarda o nome da seção (uma única palavra)
	struct noh 	* pai, 	// aponta para o pai
				* fesq, // aponta para o filho mais a esquerda
				* idir; // aponta para o irmão a direita
};

//-------------Declaração global do ponteiro para o vetor de structs-------
struct noh *raiz = (struct noh*) NULL;

//-------------Declaração global das variáveis para o listamento da árvore-------
int count = 0; 						// Contabiliza a quantidade de listas mostradas
int indice[8] = {1,1,1,1,1,1,1}; 	// String para indexação dos elementos da árvore

//-------------Protótipos de funções--------------------------------
void visita (struct noh *x, FILE *out, int nivel);				  // Mostra o conteúdo do nó da árvore
void preOrdem (struct noh *x, FILE *out, int nivel); 			  // Percurso em pré-Ordem de uma árvore
struct noh *Busca (char elemento[], struct noh *x); 			  // Busca recursivamente o elemento na sub-árvore cuja raiz é apontada por x
void titulo (char name[]); 										  // Cria árvore a partir do nó raiz
void inserir (FILE *in, FILE *out, char name[], char how[]); 	  // Insere novo nó na árvore
void remover (struct noh* aux, FILE *in);						  // Remove nó da árvore
void killTree(struct noh* aux);									  // Exclui uma árvore ou sub-árvore da memória
void transferir (FILE *in, FILE *out, char name[], char how[]);	  // Transfere um nó de um local para outro na árvore
void lista (FILE *out);											  // Mostrar a árvore em formato ao mesmo tempo indentado e itemizado
int largura (char name[]);  									  // Captura a distância entre o nó com nome de seção "name" e o filho mais esquerdo

int main(){
	struct noh* aux = (struct noh*) NULL;	// ponteiro auxiliar que apontar para o pai do novo nó
	char  line[79],       					// string line é auxiliar para pular linha ou pular espaçoes em branco
		action[29],     				    // string action é auxiliar para identificar as ações: TITULO, INSERIR, REMOVER, TRANSFERIR e LISTA
		  name[29],        					// string name é auxilar para guardar o nome da seção a qual será aplicada a ação
		   how[29];   						// string how é auxiliar para guardar como o novo nó será inserido
    int i;			 						// inteiro i é variável auxiliar que serve para iterações (laços)
    FILE *in, *out; 	 					// variáveis para os aqrquivos de entrada e saída
	printf("\t\t----------Indice de Livro----------\n");
    in = fopen ("C:\\Lab5\\entrada5.txt", "r");
    if (in == NULL)
	{
	   	printf("\n\nPROBLEMAS NO ARQUIVO DE ENTRADA!\n");
	   	return 1;
	}
    out = fopen ("C:\\Lab5\\vidal5.txt", "w");
    if (out == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA!\n");
        return 1;
    }
    //Criação do Cabeçalho do arquivo de saída
    fprintf(out,"Indice de Livro \n");
    fprintf(out,"Simple and efficient!\n");
    fprintf(out,"All books organized!\n");
    fprintf(out,"--------------------------------------------------");
    //Fim do cabeçalho do arquivo de saída
    
    for(i = 1; i <= 4; i++){// Pular as 4 primeiras linhas
        fgets(line,59,in);
    }
    
    fscanf(in,"%s ",action); // Captura a ação a ser realizada 

	while (strcmp(action,"FIM") != 0){
        if (!strcmp(action,"TITULO")){		// Realiza a criação do nó raiz da árvore
        	fscanf(in,"%s ", name); 		// Captura o nome da seção do nó raiz da árvore
        	titulo (name);					// Chamada de função titulo para criação da árvore	
	    }
        if (!strcmp(action,"INSERIR")){			// Realiza o fechamento da conta corrente
        	fscanf(in,"%s ", name);				// Captura o nome da seção do novo nó da árvore
			fscanf(in,"%s ", how); 				// Captura o modo de inserção
        	inserir (in, out, name, how);// Chamada de função inserir 
        }
        if (!strcmp(action,"REMOVER")){			// Realiza o depósito na conta corrente
        	fscanf(in,"%s ", name); 			// Captura o nome da seção a ser removido do nó da árvore
        	aux =  Busca (name, raiz);
			if (aux == (struct noh*)NULL){ 		// A função Busca(), retorna o endereço do elemento a ser procurado. Caso, não o encontre, retorna NULL.
				fprintf(out,"\nERRO: nao encontrado item %s", name);
				fprintf(out,"\n--------------------------------------------------");
        	}
        	else{ 								// Caso exista o elemento na árvore
        		remover (aux, in);				// Chamada de função remover para remoção de nó da árvore
    		}
    	}
    	if (!strcmp(action,"TRANSFERIR")){// Realiza a transferência de nó para outra posição da árvore
    		fscanf(in,"%s ", name);						// Captura o nome da seção do nó da árvore a ser transferido
			fscanf(in,"%s ", how); 						// Captura o modo de trasnferência
        	transferir (in, out, name, how);			// Chamada de função transferir 
    	}
    	if (!strcmp(action,"LISTA")){
    		//-----------Lista de elementos-----------
   			lista (out);
    	}
    	
		fscanf(in,"%s",action); // Captura a próxima ação a ser realizada 
    }
    
    // Fechamento dos arquivos de entrada e saída
	fclose(in);
    fclose(out);
    
    return 0;
}

void visita (struct noh *x, FILE *out, int nivel){// Mostra o conteúdo do nó da árvore
	int i;
	if ( x == raiz) 
		fprintf(out,"      %s\n", x->name);
	else{
		for (i=1; i<=nivel; i++) 			// A quantidade de dígitos e pontos são o valor de nivel
			fprintf(out,"%d.",indice[i-1]); // Escreve no arquivo a indexação
		fprintf(out,"      %s\n", x->name); // Escreve no arquivo o nome do nó
	}
}

void preOrdem (struct noh *x, FILE *out, int nivel){// Percurso em pré-Ordem de uma árvore
	// nivel comça com 0, pois o nivel da raiz é zero
	// largura começa com 1, pois é o primeiro nó como filho 
	struct noh*p = (struct noh*) NULL;
	visita (x, out, nivel); 				// Visita o nó da árvore
	p = x->fesq;							// Desce de nível na árvore
	if ( p != (struct noh*) NULL) {			// Caso tem mais nós "para baixo", contabiliza o nivel
		nivel++;
	}
	while (p != (struct noh*) NULL){		// Enquanto não chegar numa folha
		indice[nivel-1] = largura(p->name);	// Salva o índice para a indexação, isto é, necessário para a itemização
		preOrdem(p, out, nivel);			// Chamada recursiva de acordo com notação pré-fixada
		p = p->idir;						// Caminhando para a direita na lista encadeada de filhos
	}
}

int largura (char name[]){ // Captura a distância entre o nó com nome de seção "name" e o filho mais esquerdo
	struct noh*pai = (struct noh*) NULL;							// Ponteiro que apontará para o pai do elemento interessado
	struct noh*p = (struct noh*) NULL;								// Ponteiro auxiliar para o loop
	int L = 1; 														// largura começa com 1, pois é o primeiro nó como filho 
	pai = (Busca (name, raiz))->pai;								// Apontamento para o pai do elemento sobre o qual se quer determinar a ordem da esquerda da direita na lista encadeada de filhos
	for(p = pai->fesq; strcmp (p->name, name) != 0; p = p->idir)	// Loop para contabilizar a posição desejada
		L++;														
	return L;
}

struct noh *Busca (char elemento[], struct noh *x){						// Busca recursivamente o elemento na sub-árvore cuja raiz é apontada por x
	struct noh *p; 														// Ponteiro auxiliar de posição a ser percorrida
	struct noh* resposta;												// Ponteiro que guardará o elemento buscado
	if ( raiz == (struct noh*) NULL)									// Caso árvore vazia, retorna NULL, já que não é possível achar nenhum elemento 
		return (struct noh*) NULL;
	if( strcmp (elemento, x->name) == 0)								// Caso de parada da recursão
		return x;
	resposta = (struct noh*)NULL;										// Caso não ache, retorna NULL
	p = x->fesq;
	while (p != (struct noh*) NULL && resposta == (struct noh*) NULL){ 	// Loop para buscar o elemento na lista encadeada de filhos 
		resposta = Busca(elemento, p);
		p = p->idir;													// Passo do loop
	}
	return resposta;													// Caso não se ache o elemento
}

void titulo (char name[]){									// Cria árvore a partir do nó raiz
		struct noh * aux = (struct noh*) NULL;				// Ponteiro auxiliar para guardar as informações do novo nó raiz da árvore
        aux = (struct noh*) malloc (sizeof(struct noh));	// Alocação dinâmica
        strcpy(aux->name, name); 							// Guarda o nome da seção na raiz
        //------ Apontamento para NULL--------
        aux->fesq = (struct noh*) NULL;						
        aux->idir = (struct noh*) NULL;
        aux->pai = (struct noh*) NULL;
        if (raiz == (struct noh*) NULL){ 					// Árvore vazia
        	raiz = aux;
		}
		else{ 												// Caso já haja árvore, nesse caso todo o restante ficará como sub-árvore do novo título.
			raiz->pai = aux;
			aux->fesq = raiz;
			raiz = aux;
		}
}

void inserir (FILE *in, FILE *out, char name[], char how[]){// Insere novo nó na árvore
	struct noh* novo = (struct noh*) NULL;	// ponteiro auxiliar para um novo nó do tipo struct noh
	struct noh* ant = (struct noh*) NULL;	// ponteiro auxiliar para o nó do tipo struct noh anterior àquele a ser escluído ou transferido na lista encadeada  de filhos
	struct noh* aux = (struct noh*) NULL;	// ponteiro auxiliar que apontar para o pai do novo nó
	char	dad[29],         				// string dad é auxiliar para guardar o nome do pai ao qual o "name" será filho
		brother[29];      					// string brother é auxiliar para guardar o nome do irmao ao qual o "name" será irmão direito
	if ( !strcmp (how, "SUB") ){ 				 	          // Inserir como filho
		fscanf(in,"%s ", dad); 							      // Guarda o nome da seção pai
		aux = Busca (dad, raiz);							  // A função Busca(), retorna o endereço do elemento a ser procurado. Caso, não o encontre, retorna NULL. Buscando o nó pai.
		if ( aux != (struct noh*)NULL){ 	  				  
			novo = (struct noh*) malloc (sizeof(struct noh)); // Alocação dinâmica do novo nó
			strcpy(novo->name,name);			 			  // Guardando o nome da nova seção a ser adicionada na árvore
			novo->pai = aux;                 				  // Fazendo o novo nó ser filho do pai desejado
			novo->idir = aux->fesq;                       	  // Fazendo o encadeamento entre irmãos
			aux->fesq = novo;                             	  // Colocando o novo filho como filho mais esquerdo
			novo->fesq = (struct noh*)NULL;          		 	// Apontameto para NULL do ponteiro anterior, ao final da operação
		}
		else { 												  // Caso o pai não encontrado
			fprintf(out,"\nERRO: nao encontrado item %s", dad);
			fprintf(out,"\n--------------------------------------------------");
		}
	}
	if ( !strcmp (how, "SEGUINTE_A") ){ 				 	 // Inserir como irmão
		fscanf(in,"%s ", brother); 							 // Guarda o nome da seção irmão
		if ( !strcmp (brother, raiz->name)){
			fprintf(out, "\nERRO: operacao invalida");
			fprintf(out,"\n--------------------------------------------------");
		}
		else if (Busca (brother, raiz) != (struct noh*)NULL){// A função Busca(), retorna o endereço do elemento a ser procurado. Caso, não o encontre, retorna NULL.
			novo = (struct noh*) malloc (sizeof(struct noh));	// Alocação dinâmica do novo nó
			strcpy(novo->name,name);						 	// Guardando o nome da nova seção a ser adicionada na árvore
			ant = Busca (brother, raiz);					 	// Ponteiro ant aponta para o irmão ao qual 
			novo->pai = ant->pai;							 	// Apontamento do novo nó para o pai desejado
			novo->idir = ant->idir;							 	// Encadeamento do novo nó na lista de filhos
			ant->idir = novo;								 	// Encadeamento do nó anterior para o novo nó na lista de filhos
			novo->fesq = (struct noh*)NULL;          		 	// Apontameto para NULL do ponteiro anterior, ao final da operação
		}
		else{												    // Caso o pai não encontrado
			fprintf(out,"\nERRO: nao encontrado item %s", brother);	
			fprintf(out,"\n--------------------------------------------------");
		}
		
	}
	//------ Apontamento para NULL--------
	novo = (struct noh*) NULL;
	ant = (struct noh*) NULL;
	aux = (struct noh*) NULL;	
}

void remover (struct noh* aux, FILE *in){	// Remove um nó da árvore
	struct noh* ant = (struct noh*) NULL;	// Ponteiro auxiliar para o nó do tipo struct noh anterior àquele a ser escluído ou transferido na lista encadeada  de filhos
	if (raiz != (struct noh*) NULL){		// Só remover caso não seja árvore vazia
		if (raiz->fesq == (struct noh*) NULL){	// Caso a árvore tenha só o nó raiz
			free(raiz); 						// Exclui da memória
			raiz = (struct noh*) NULL;			// Apontamento para NULL, árvore vazia
		}
	    else if (aux->pai->fesq == aux ){	// Caso o elemento seja o primeiro da lista encadeada de filhos
	    	aux->pai->fesq = aux->idir;    	// Apontamento do pai para o segundo da lista encadeada 
	    	aux->idir = (struct noh*) NULL; // Apontamento para NULL do campo irmão direito do nó a ser removido da árvore
	    	aux->pai = (struct noh*) NULL;  // Apontamento para NULL do campo pai do nó a ser removido da árvore
    		killTree (aux);					// Remoção da sub-árvore do espaço de memória 
		}
		else if (aux->idir != (struct noh*) NULL){							  // Caso o elemento esteja no meio da lista encadeada de filhos
			for (ant = aux->pai->fesq; ant->idir != aux ; ant = ant->idir);   // Posicionamento do ponteiro anterior ao nó a ser removido da árvore
			ant->idir = aux->idir;											  // Encadeamento do nó anterior para o próximo, em relação ao nó a ser removido da árvore
			aux->idir = (struct noh*) NULL;									  // Apontamento para NULL do campo irmão direito do nó a ser removido da árvore
				aux->pai = (struct noh*) NULL;								  // Apontamento para NULL do campo pai do nó a ser removido da árvore
			killTree (aux);												      // Remoção da sub-árvore do espaço de memória 
		}
		else{ // Caso o elemento estaja no fim da lista encadeada de filhos
			for (ant = aux->pai->fesq; ant->idir != aux ; ant = ant->idir);   // Posicionamento do ponteiro anterior ao nó a ser removido da árvore
			ant->idir = (struct noh*) NULL;									  // Apontamnto para NULL do campo irmão direito do nó anterior em relação ao nó a ser removido da árvore (o último da lista)
				aux->pai = (struct noh*) NULL;								  // Apontamento para NULL do campo pai do nó a ser removido da árvore
			killTree (aux);											  		  // Remoção da sub-árvore do espaço de memória 
		}	
	}
				
	//------ Apontamento para NULL--------
	aux = (struct noh*) NULL;
	ant = (struct noh*) NULL;
}

void killTree(struct noh*aux){
	struct noh* p = (struct noh*) NULL;		// Criação de um ponteiro auxiliar para o loop de exclusão
    aux = aux->fesq;						// Desce para os filhos do elemento a ser removido da árvore
    while ( aux != (struct noh*) NULL){		// Loop para excluir todos os elementos da lista encadeada de filhos do nó ou sub-árvore 
    	for (p = aux; p != (struct noh*) NULL ;p = p->idir){
    		aux = aux->idir;				// Iteração para o próximo irmão a direita
    		killTree (p);					// Chamada de função recusiva para remover um elemento da lista encadeada de filhos
		}
	}
	free(aux);
	p = (struct noh*) NULL;
}

void transferir (FILE *in, FILE *out, char name[], char how[]){// Transfere um nó de um local para outro na árvore
	struct noh* trans = (struct noh*) NULL; // ponteiro auxiliar para o nó do tipo struct noh que será transferido
	struct noh* aux = (struct noh*) NULL;	// ponteiro auxiliar que apontar para o pai do nó a ser transferido
	struct noh* ant = (struct noh*) NULL;	// ponteiro auxiliar para o nó do tipo struct noh anterior àquele a ser escluído ou transferido na lista encadeada  de filhos
	char	dad[29],         				// string dad é auxiliar para guardar o nome do pai ao qual o "name" será filho
		brother[29];      					// string brother é auxiliar para guardar o nome do irmao ao qual o "name" será irmão direito
	if ( !strcmp (how, "SUB") ){ 				 	          // Transferir como filho
		fscanf(in,"%s ", dad); 							      // Guarda o nome da seção pai
		aux = Busca (dad, raiz);							  // A função Busca(), retorna o endereço do elemento a ser procurado. Caso, não o encontre, retorna NULL. Buscando o nó pai.
		trans = Busca (name, raiz);						  	  // Buscando a posição na árvore do nó a ser transferido	
		if ( aux != (struct noh*)NULL && trans != (struct noh*)NULL){
			if (trans->pai->fesq == trans){ 				  // Caso o elemento a ser transferido seja o primeiro da lista encadeada de filhos onde se encontra
				trans->pai->fesq = trans->idir; 
			}
			else {
				for (ant = trans->pai->fesq; ant->idir != trans ; ant = ant->idir); // Posicionamento do ponteiro anterior ao nó a ser transferido da árvore 	
				ant->idir = trans->idir;	
			}
			trans->pai = aux;                 				  // Fazendo o nó ser filho do pai desejado
			trans->idir = aux->fesq;                       	  // Fazendo o encadeamento entre irmãos
			aux->fesq = trans;                             	  // Colocando o filho como filho mais esquerdo
		}
		else { 												  // Caso o pai não encontrado ou nó a ser transferido
			if (aux == (struct noh*)NULL){
				fprintf(out,"\nERRO: nao encontrado item %s", dad);
				fprintf(out,"\n--------------------------------------------------");
			}
			else{
				fprintf(out,"\nERRO: nao encontrado item %s\n", name);
				fprintf(out,"\n--------------------------------------------------");
			}
		}
	}
	if ( !strcmp (how, "SEGUINTE_A") ){ 				 	 // Transferir como irmão
		fscanf(in,"%s ", brother); 							 // Guarda o nome da seção irmão
		ant = Busca (brother, raiz);
		trans = Busca (name, raiz);
		if ( !strcmp (brother, raiz->name)){
			fprintf(out, "\nERRO: operacao invalida");
			fprintf(out,"\n--------------------------------------------------");
		}
		else if (ant != (struct noh*)NULL && trans != (struct noh*) NULL){  // A função Busca(), retorna o endereço do elemento a ser procurado. Caso, não o encontre, retorna NULL.
			if (trans->pai->fesq == trans){ 				    			// Caso o elemento a ser transferido seja o primeiro da lista encadeada de filhos onde se encontra
				trans->pai->fesq = trans->idir; 
			}
			else {
				for (ant = trans->pai->fesq; ant->idir != trans ; ant = ant->idir); // Posicionamento do ponteiro anterior ao nó a ser transferido da árvore 	
				ant->idir = trans->idir;	
			}
			ant = Busca (brother, raiz);
			trans->pai = ant->pai;							 	// Apontamento do novo nó para o pai desejado
			trans->idir = ant->idir;							// Encadeamento do novo nó na lista de filhos
			ant->idir = trans;								 	// Encadeamento do nó anterior para o novo nó na lista de filhos
		}
		else{												 // Caso o irmão não encontrado
			if (ant == (struct noh*)NULL){
				fprintf(out,"\nERRO: nao encontrado item %s", brother);
				fprintf(out,"--------------------------------------------------");
			}
			else{
				fprintf(out,"\nERRO: nao encontrado item %s", name);
				fprintf(out,"--------------------------------------------------");
			}
		}
	}
	//------ Apontamento para NULL--------
	trans = (struct noh*) NULL;
	aux = (struct noh*) NULL;
	ant = (struct noh*) NULL;
}

void lista (FILE *out){// Mostrar a árvore em formato ao mesmo tempo indentado e itemizado
	fprintf(out, "\nLISTA %d\n\n", ++count);
	int nivel = 0;						// Contabiliza o nível no qual se está na árvore
	int largura = 1;					// Contabiliza em termos de largura da árvore
	if (raiz != (struct noh*) NULL)		// Caso a ávore esteja vazia, não é mostrado nada
		preOrdem (raiz, out, nivel);
	fprintf(out, "--------------------------------------------------");
}
