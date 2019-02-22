/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 05: �ndice de Livro                      */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------Defini��o da estrutura de dados---------------------
struct noh{
	char name[29]; 		// guarda o nome da se��o (uma �nica palavra)
	struct noh 	* pai, 	// aponta para o pai
				* fesq, // aponta para o filho mais a esquerda
				* idir; // aponta para o irm�o a direita
};

//-------------Declara��o global do ponteiro para o vetor de structs-------
struct noh *raiz = (struct noh*) NULL;

//-------------Declara��o global das vari�veis para o listamento da �rvore-------
int count = 0; 						// Contabiliza a quantidade de listas mostradas
int indice[8] = {1,1,1,1,1,1,1}; 	// String para indexa��o dos elementos da �rvore

//-------------Prot�tipos de fun��es--------------------------------
void visita (struct noh *x, FILE *out, int nivel);				  // Mostra o conte�do do n� da �rvore
void preOrdem (struct noh *x, FILE *out, int nivel); 			  // Percurso em pr�-Ordem de uma �rvore
struct noh *Busca (char elemento[], struct noh *x); 			  // Busca recursivamente o elemento na sub-�rvore cuja raiz � apontada por x
void titulo (char name[]); 										  // Cria �rvore a partir do n� raiz
void inserir (FILE *in, FILE *out, char name[], char how[]); 	  // Insere novo n� na �rvore
void remover (struct noh* aux, FILE *in);						  // Remove n� da �rvore
void killTree(struct noh* aux);									  // Exclui uma �rvore ou sub-�rvore da mem�ria
void transferir (FILE *in, FILE *out, char name[], char how[]);	  // Transfere um n� de um local para outro na �rvore
void lista (FILE *out);											  // Mostrar a �rvore em formato ao mesmo tempo indentado e itemizado
int largura (char name[]);  									  // Captura a dist�ncia entre o n� com nome de se��o "name" e o filho mais esquerdo

int main(){
	struct noh* aux = (struct noh*) NULL;	// ponteiro auxiliar que apontar para o pai do novo n�
	char  line[79],       					// string line � auxiliar para pular linha ou pular espa�oes em branco
		action[29],     				    // string action � auxiliar para identificar as a��es: TITULO, INSERIR, REMOVER, TRANSFERIR e LISTA
		  name[29],        					// string name � auxilar para guardar o nome da se��o a qual ser� aplicada a a��o
		   how[29];   						// string how � auxiliar para guardar como o novo n� ser� inserido
    int i;			 						// inteiro i � vari�vel auxiliar que serve para itera��es (la�os)
    FILE *in, *out; 	 					// vari�veis para os aqrquivos de entrada e sa�da
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
    //Cria��o do Cabe�alho do arquivo de sa�da
    fprintf(out,"Indice de Livro \n");
    fprintf(out,"Simple and efficient!\n");
    fprintf(out,"All books organized!\n");
    fprintf(out,"--------------------------------------------------");
    //Fim do cabe�alho do arquivo de sa�da
    
    for(i = 1; i <= 4; i++){// Pular as 4 primeiras linhas
        fgets(line,59,in);
    }
    
    fscanf(in,"%s ",action); // Captura a a��o a ser realizada 

	while (strcmp(action,"FIM") != 0){
        if (!strcmp(action,"TITULO")){		// Realiza a cria��o do n� raiz da �rvore
        	fscanf(in,"%s ", name); 		// Captura o nome da se��o do n� raiz da �rvore
        	titulo (name);					// Chamada de fun��o titulo para cria��o da �rvore	
	    }
        if (!strcmp(action,"INSERIR")){			// Realiza o fechamento da conta corrente
        	fscanf(in,"%s ", name);				// Captura o nome da se��o do novo n� da �rvore
			fscanf(in,"%s ", how); 				// Captura o modo de inser��o
        	inserir (in, out, name, how);// Chamada de fun��o inserir 
        }
        if (!strcmp(action,"REMOVER")){			// Realiza o dep�sito na conta corrente
        	fscanf(in,"%s ", name); 			// Captura o nome da se��o a ser removido do n� da �rvore
        	aux =  Busca (name, raiz);
			if (aux == (struct noh*)NULL){ 		// A fun��o Busca(), retorna o endere�o do elemento a ser procurado. Caso, n�o o encontre, retorna NULL.
				fprintf(out,"\nERRO: nao encontrado item %s", name);
				fprintf(out,"\n--------------------------------------------------");
        	}
        	else{ 								// Caso exista o elemento na �rvore
        		remover (aux, in);				// Chamada de fun��o remover para remo��o de n� da �rvore
    		}
    	}
    	if (!strcmp(action,"TRANSFERIR")){// Realiza a transfer�ncia de n� para outra posi��o da �rvore
    		fscanf(in,"%s ", name);						// Captura o nome da se��o do n� da �rvore a ser transferido
			fscanf(in,"%s ", how); 						// Captura o modo de trasnfer�ncia
        	transferir (in, out, name, how);			// Chamada de fun��o transferir 
    	}
    	if (!strcmp(action,"LISTA")){
    		//-----------Lista de elementos-----------
   			lista (out);
    	}
    	
		fscanf(in,"%s",action); // Captura a pr�xima a��o a ser realizada 
    }
    
    // Fechamento dos arquivos de entrada e sa�da
	fclose(in);
    fclose(out);
    
    return 0;
}

void visita (struct noh *x, FILE *out, int nivel){// Mostra o conte�do do n� da �rvore
	int i;
	if ( x == raiz) 
		fprintf(out,"      %s\n", x->name);
	else{
		for (i=1; i<=nivel; i++) 			// A quantidade de d�gitos e pontos s�o o valor de nivel
			fprintf(out,"%d.",indice[i-1]); // Escreve no arquivo a indexa��o
		fprintf(out,"      %s\n", x->name); // Escreve no arquivo o nome do n�
	}
}

void preOrdem (struct noh *x, FILE *out, int nivel){// Percurso em pr�-Ordem de uma �rvore
	// nivel com�a com 0, pois o nivel da raiz � zero
	// largura come�a com 1, pois � o primeiro n� como filho 
	struct noh*p = (struct noh*) NULL;
	visita (x, out, nivel); 				// Visita o n� da �rvore
	p = x->fesq;							// Desce de n�vel na �rvore
	if ( p != (struct noh*) NULL) {			// Caso tem mais n�s "para baixo", contabiliza o nivel
		nivel++;
	}
	while (p != (struct noh*) NULL){		// Enquanto n�o chegar numa folha
		indice[nivel-1] = largura(p->name);	// Salva o �ndice para a indexa��o, isto �, necess�rio para a itemiza��o
		preOrdem(p, out, nivel);			// Chamada recursiva de acordo com nota��o pr�-fixada
		p = p->idir;						// Caminhando para a direita na lista encadeada de filhos
	}
}

int largura (char name[]){ // Captura a dist�ncia entre o n� com nome de se��o "name" e o filho mais esquerdo
	struct noh*pai = (struct noh*) NULL;							// Ponteiro que apontar� para o pai do elemento interessado
	struct noh*p = (struct noh*) NULL;								// Ponteiro auxiliar para o loop
	int L = 1; 														// largura come�a com 1, pois � o primeiro n� como filho 
	pai = (Busca (name, raiz))->pai;								// Apontamento para o pai do elemento sobre o qual se quer determinar a ordem da esquerda da direita na lista encadeada de filhos
	for(p = pai->fesq; strcmp (p->name, name) != 0; p = p->idir)	// Loop para contabilizar a posi��o desejada
		L++;														
	return L;
}

struct noh *Busca (char elemento[], struct noh *x){						// Busca recursivamente o elemento na sub-�rvore cuja raiz � apontada por x
	struct noh *p; 														// Ponteiro auxiliar de posi��o a ser percorrida
	struct noh* resposta;												// Ponteiro que guardar� o elemento buscado
	if ( raiz == (struct noh*) NULL)									// Caso �rvore vazia, retorna NULL, j� que n�o � poss�vel achar nenhum elemento 
		return (struct noh*) NULL;
	if( strcmp (elemento, x->name) == 0)								// Caso de parada da recurs�o
		return x;
	resposta = (struct noh*)NULL;										// Caso n�o ache, retorna NULL
	p = x->fesq;
	while (p != (struct noh*) NULL && resposta == (struct noh*) NULL){ 	// Loop para buscar o elemento na lista encadeada de filhos 
		resposta = Busca(elemento, p);
		p = p->idir;													// Passo do loop
	}
	return resposta;													// Caso n�o se ache o elemento
}

void titulo (char name[]){									// Cria �rvore a partir do n� raiz
		struct noh * aux = (struct noh*) NULL;				// Ponteiro auxiliar para guardar as informa��es do novo n� raiz da �rvore
        aux = (struct noh*) malloc (sizeof(struct noh));	// Aloca��o din�mica
        strcpy(aux->name, name); 							// Guarda o nome da se��o na raiz
        //------ Apontamento para NULL--------
        aux->fesq = (struct noh*) NULL;						
        aux->idir = (struct noh*) NULL;
        aux->pai = (struct noh*) NULL;
        if (raiz == (struct noh*) NULL){ 					// �rvore vazia
        	raiz = aux;
		}
		else{ 												// Caso j� haja �rvore, nesse caso todo o restante ficar� como sub-�rvore do novo t�tulo.
			raiz->pai = aux;
			aux->fesq = raiz;
			raiz = aux;
		}
}

void inserir (FILE *in, FILE *out, char name[], char how[]){// Insere novo n� na �rvore
	struct noh* novo = (struct noh*) NULL;	// ponteiro auxiliar para um novo n� do tipo struct noh
	struct noh* ant = (struct noh*) NULL;	// ponteiro auxiliar para o n� do tipo struct noh anterior �quele a ser esclu�do ou transferido na lista encadeada  de filhos
	struct noh* aux = (struct noh*) NULL;	// ponteiro auxiliar que apontar para o pai do novo n�
	char	dad[29],         				// string dad � auxiliar para guardar o nome do pai ao qual o "name" ser� filho
		brother[29];      					// string brother � auxiliar para guardar o nome do irmao ao qual o "name" ser� irm�o direito
	if ( !strcmp (how, "SUB") ){ 				 	          // Inserir como filho
		fscanf(in,"%s ", dad); 							      // Guarda o nome da se��o pai
		aux = Busca (dad, raiz);							  // A fun��o Busca(), retorna o endere�o do elemento a ser procurado. Caso, n�o o encontre, retorna NULL. Buscando o n� pai.
		if ( aux != (struct noh*)NULL){ 	  				  
			novo = (struct noh*) malloc (sizeof(struct noh)); // Aloca��o din�mica do novo n�
			strcpy(novo->name,name);			 			  // Guardando o nome da nova se��o a ser adicionada na �rvore
			novo->pai = aux;                 				  // Fazendo o novo n� ser filho do pai desejado
			novo->idir = aux->fesq;                       	  // Fazendo o encadeamento entre irm�os
			aux->fesq = novo;                             	  // Colocando o novo filho como filho mais esquerdo
			novo->fesq = (struct noh*)NULL;          		 	// Apontameto para NULL do ponteiro anterior, ao final da opera��o
		}
		else { 												  // Caso o pai n�o encontrado
			fprintf(out,"\nERRO: nao encontrado item %s", dad);
			fprintf(out,"\n--------------------------------------------------");
		}
	}
	if ( !strcmp (how, "SEGUINTE_A") ){ 				 	 // Inserir como irm�o
		fscanf(in,"%s ", brother); 							 // Guarda o nome da se��o irm�o
		if ( !strcmp (brother, raiz->name)){
			fprintf(out, "\nERRO: operacao invalida");
			fprintf(out,"\n--------------------------------------------------");
		}
		else if (Busca (brother, raiz) != (struct noh*)NULL){// A fun��o Busca(), retorna o endere�o do elemento a ser procurado. Caso, n�o o encontre, retorna NULL.
			novo = (struct noh*) malloc (sizeof(struct noh));	// Aloca��o din�mica do novo n�
			strcpy(novo->name,name);						 	// Guardando o nome da nova se��o a ser adicionada na �rvore
			ant = Busca (brother, raiz);					 	// Ponteiro ant aponta para o irm�o ao qual 
			novo->pai = ant->pai;							 	// Apontamento do novo n� para o pai desejado
			novo->idir = ant->idir;							 	// Encadeamento do novo n� na lista de filhos
			ant->idir = novo;								 	// Encadeamento do n� anterior para o novo n� na lista de filhos
			novo->fesq = (struct noh*)NULL;          		 	// Apontameto para NULL do ponteiro anterior, ao final da opera��o
		}
		else{												    // Caso o pai n�o encontrado
			fprintf(out,"\nERRO: nao encontrado item %s", brother);	
			fprintf(out,"\n--------------------------------------------------");
		}
		
	}
	//------ Apontamento para NULL--------
	novo = (struct noh*) NULL;
	ant = (struct noh*) NULL;
	aux = (struct noh*) NULL;	
}

void remover (struct noh* aux, FILE *in){	// Remove um n� da �rvore
	struct noh* ant = (struct noh*) NULL;	// Ponteiro auxiliar para o n� do tipo struct noh anterior �quele a ser esclu�do ou transferido na lista encadeada  de filhos
	if (raiz != (struct noh*) NULL){		// S� remover caso n�o seja �rvore vazia
		if (raiz->fesq == (struct noh*) NULL){	// Caso a �rvore tenha s� o n� raiz
			free(raiz); 						// Exclui da mem�ria
			raiz = (struct noh*) NULL;			// Apontamento para NULL, �rvore vazia
		}
	    else if (aux->pai->fesq == aux ){	// Caso o elemento seja o primeiro da lista encadeada de filhos
	    	aux->pai->fesq = aux->idir;    	// Apontamento do pai para o segundo da lista encadeada 
	    	aux->idir = (struct noh*) NULL; // Apontamento para NULL do campo irm�o direito do n� a ser removido da �rvore
	    	aux->pai = (struct noh*) NULL;  // Apontamento para NULL do campo pai do n� a ser removido da �rvore
    		killTree (aux);					// Remo��o da sub-�rvore do espa�o de mem�ria 
		}
		else if (aux->idir != (struct noh*) NULL){							  // Caso o elemento esteja no meio da lista encadeada de filhos
			for (ant = aux->pai->fesq; ant->idir != aux ; ant = ant->idir);   // Posicionamento do ponteiro anterior ao n� a ser removido da �rvore
			ant->idir = aux->idir;											  // Encadeamento do n� anterior para o pr�ximo, em rela��o ao n� a ser removido da �rvore
			aux->idir = (struct noh*) NULL;									  // Apontamento para NULL do campo irm�o direito do n� a ser removido da �rvore
				aux->pai = (struct noh*) NULL;								  // Apontamento para NULL do campo pai do n� a ser removido da �rvore
			killTree (aux);												      // Remo��o da sub-�rvore do espa�o de mem�ria 
		}
		else{ // Caso o elemento estaja no fim da lista encadeada de filhos
			for (ant = aux->pai->fesq; ant->idir != aux ; ant = ant->idir);   // Posicionamento do ponteiro anterior ao n� a ser removido da �rvore
			ant->idir = (struct noh*) NULL;									  // Apontamnto para NULL do campo irm�o direito do n� anterior em rela��o ao n� a ser removido da �rvore (o �ltimo da lista)
				aux->pai = (struct noh*) NULL;								  // Apontamento para NULL do campo pai do n� a ser removido da �rvore
			killTree (aux);											  		  // Remo��o da sub-�rvore do espa�o de mem�ria 
		}	
	}
				
	//------ Apontamento para NULL--------
	aux = (struct noh*) NULL;
	ant = (struct noh*) NULL;
}

void killTree(struct noh*aux){
	struct noh* p = (struct noh*) NULL;		// Cria��o de um ponteiro auxiliar para o loop de exclus�o
    aux = aux->fesq;						// Desce para os filhos do elemento a ser removido da �rvore
    while ( aux != (struct noh*) NULL){		// Loop para excluir todos os elementos da lista encadeada de filhos do n� ou sub-�rvore 
    	for (p = aux; p != (struct noh*) NULL ;p = p->idir){
    		aux = aux->idir;				// Itera��o para o pr�ximo irm�o a direita
    		killTree (p);					// Chamada de fun��o recusiva para remover um elemento da lista encadeada de filhos
		}
	}
	free(aux);
	p = (struct noh*) NULL;
}

void transferir (FILE *in, FILE *out, char name[], char how[]){// Transfere um n� de um local para outro na �rvore
	struct noh* trans = (struct noh*) NULL; // ponteiro auxiliar para o n� do tipo struct noh que ser� transferido
	struct noh* aux = (struct noh*) NULL;	// ponteiro auxiliar que apontar para o pai do n� a ser transferido
	struct noh* ant = (struct noh*) NULL;	// ponteiro auxiliar para o n� do tipo struct noh anterior �quele a ser esclu�do ou transferido na lista encadeada  de filhos
	char	dad[29],         				// string dad � auxiliar para guardar o nome do pai ao qual o "name" ser� filho
		brother[29];      					// string brother � auxiliar para guardar o nome do irmao ao qual o "name" ser� irm�o direito
	if ( !strcmp (how, "SUB") ){ 				 	          // Transferir como filho
		fscanf(in,"%s ", dad); 							      // Guarda o nome da se��o pai
		aux = Busca (dad, raiz);							  // A fun��o Busca(), retorna o endere�o do elemento a ser procurado. Caso, n�o o encontre, retorna NULL. Buscando o n� pai.
		trans = Busca (name, raiz);						  	  // Buscando a posi��o na �rvore do n� a ser transferido	
		if ( aux != (struct noh*)NULL && trans != (struct noh*)NULL){
			if (trans->pai->fesq == trans){ 				  // Caso o elemento a ser transferido seja o primeiro da lista encadeada de filhos onde se encontra
				trans->pai->fesq = trans->idir; 
			}
			else {
				for (ant = trans->pai->fesq; ant->idir != trans ; ant = ant->idir); // Posicionamento do ponteiro anterior ao n� a ser transferido da �rvore 	
				ant->idir = trans->idir;	
			}
			trans->pai = aux;                 				  // Fazendo o n� ser filho do pai desejado
			trans->idir = aux->fesq;                       	  // Fazendo o encadeamento entre irm�os
			aux->fesq = trans;                             	  // Colocando o filho como filho mais esquerdo
		}
		else { 												  // Caso o pai n�o encontrado ou n� a ser transferido
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
	if ( !strcmp (how, "SEGUINTE_A") ){ 				 	 // Transferir como irm�o
		fscanf(in,"%s ", brother); 							 // Guarda o nome da se��o irm�o
		ant = Busca (brother, raiz);
		trans = Busca (name, raiz);
		if ( !strcmp (brother, raiz->name)){
			fprintf(out, "\nERRO: operacao invalida");
			fprintf(out,"\n--------------------------------------------------");
		}
		else if (ant != (struct noh*)NULL && trans != (struct noh*) NULL){  // A fun��o Busca(), retorna o endere�o do elemento a ser procurado. Caso, n�o o encontre, retorna NULL.
			if (trans->pai->fesq == trans){ 				    			// Caso o elemento a ser transferido seja o primeiro da lista encadeada de filhos onde se encontra
				trans->pai->fesq = trans->idir; 
			}
			else {
				for (ant = trans->pai->fesq; ant->idir != trans ; ant = ant->idir); // Posicionamento do ponteiro anterior ao n� a ser transferido da �rvore 	
				ant->idir = trans->idir;	
			}
			ant = Busca (brother, raiz);
			trans->pai = ant->pai;							 	// Apontamento do novo n� para o pai desejado
			trans->idir = ant->idir;							// Encadeamento do novo n� na lista de filhos
			ant->idir = trans;								 	// Encadeamento do n� anterior para o novo n� na lista de filhos
		}
		else{												 // Caso o irm�o n�o encontrado
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

void lista (FILE *out){// Mostrar a �rvore em formato ao mesmo tempo indentado e itemizado
	fprintf(out, "\nLISTA %d\n\n", ++count);
	int nivel = 0;						// Contabiliza o n�vel no qual se est� na �rvore
	int largura = 1;					// Contabiliza em termos de largura da �rvore
	if (raiz != (struct noh*) NULL)		// Caso a �vore esteja vazia, n�o � mostrado nada
		preOrdem (raiz, out, nivel);
	fprintf(out, "--------------------------------------------------");
}
