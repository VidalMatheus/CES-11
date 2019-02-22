/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 06: Agenda Eletr�nica                    */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------Defini��o da estrutura de dados---------------------
struct noh{				// cada n� do heap � uma estrutura com import�ncia e descri��o
	char desc[79];		// string description � auxiliar para guardar a descri��o da tarefa
	int value;			// inteiro que guarda a import�ncia de cada tarefa numa escala de 1 a 99
};

struct heap{			// fila de prioridade com heap
	struct noh * elem; 	// ponteiro do tipo struct noh para alocar o vetor de elementos do heap
	int quant,			// inteiro quant serve para armazenar a quantidade de elementos do heap
		  MAX;			// inteiro MAX serve para guardar a quantidade m�xima poss�vel de elementos no heap
};

//-------------Prot�tipos de fun��es--------------------------------
void Inicializar (struct heap * h, int MAX);			// fun��o que inicializa o heap
void Insert (struct heap * h, struct noh elem);			// fun��o que insere um novo n� no heap
void Extract (struct heap * h);							// fun��o que extrair o elemento de maior prioridade
void Finalizar (struct heap * h);						// fun��o para finalizar o heap
bool HeapVazio (struct heap h);							// fun��o que verifica se o hep est� vazio
bool HeapCheio (struct heap h);							// fun��o que verifica se o heap est� cheio, ou seja, se quant == MAX

int main(){
	struct heap h; 							// struct heap h � a vari�vel que armazena toda a estrutura do heap
	char  line[79],       					// string line � auxiliar para pular linha 
		  task[29];     				    // string action � auxiliar para identificar as a��es: NOVA, PROXIMA
    int 	  MAX,                          // inteiro que guarda o n�mero m�ximo de elementos no heap
				i;			 				// inteiro i � vari�vel auxiliar que serve para itera��es (la�os)
	struct noh elem;						// struct noh elem � auxiliar para ser inerido no heap	
    FILE *in, *out; 	 					// vari�veis para os aqrquivos de entrada e sa�da
	printf("\t\t----------Agenda Eletronica----------\n");
    in = fopen ("C:\\Lab6\\entrada6.txt", "r");
    if (in == NULL)
	{
	   	printf("\n\nPROBLEMAS NO ARQUIVO DE ENTRADA!\n");
	   	return 1;
	}
    out = fopen ("C:\\Lab6\\vidal6.txt", "w");
    if (out == NULL)
    {
        printf("\n\nPROBLEMAS NO ARQUIVO DE SAIDA!\n");
        return 1;
    }
    //Cria��o do Cabe�alho do arquivo de sa�da
    fprintf(out,"*AGENDA ELETRONICA*\n");
    fprintf(out,"Simple and efficient!\n");
    fprintf(out,"All tasks organized!\n");
    fprintf(out,"CES-11 - ITA\n");
    fprintf(out,"Prof.: Armando\n");
    fprintf(out,"--------------------------------------------------\n");
    fprintf(out, "RESPOSTAS DAS CONSULTAS\n");
    fprintf(out,"--------------------------------------------------\n");
    //Fim do cabe�alho do arquivo de sa�da
    
    for(i = 1; i <= 5; i++){// Pular as 5 primeiras linhas
        fgets(line,79,in);
    }
 
    fscanf(in, "%d ", &MAX); // Captura o n�mero m�ximo de elementos no heap
    
    //---------------Inicializa��o do Heap--------------
    Inicializar (&h, MAX);

	for(i = 1; i <= 3; i++){// Pular as 3 pr�ximas linhas
        fgets(line,79,in);
    }

    fscanf(in,"%s ", task); // Captura o comando a ser realizado

	while (strcmp(task,"FIM") != 0){
        if (!strcmp(task,"NOVA")){			// Adiciona nova tarefa a ser realizada na agenda eletr�nica
        	fscanf(in,"%d ", &elem.value); 	// Guarda a import�ncia da tarefa
			fgets(elem.desc, 79, in);  		// Guarda a descri��o da tarefa
			if (HeapCheio(h))				// Caso heap cheio, ou seja, h->quant == h->MAX, mensagem de ERRO
				fprintf(out, "ERRO     Agenda cheia. Impossivel inserir.\n");
			else Insert (&h, elem);			// Insere elem no heap
	    } 
        if (!strcmp(task,"PROXIMA")){		// Extrai o elemento de maior prioridade (na raiz da �rvore bin�ria) e escreve no arquivo de sa�da 
			if (HeapVazio(h))				// Caso heap vazio, mensangem de AVISO
				fprintf(out, "AVISO    Nao ha tarefas na agenda  :-)\n");
			else {
				fprintf(out, "%2d       %s", h.elem[1].value, h.elem[1].desc);		// escreve a raiz no arquivo de sa�da
				Extract (&h);														// extrai do heap a raiz
			}
        }
        // Caso o arquivo de entrada n�o tenha nenhum comando PROXIMA, o RELAT�RIO DE CONSULTAS ficar� vazio, isto �, nada ser� mostrado

		fscanf(in,"%s ", task); // Captura o pr�ximo comando a ser realizada 
    }
    // Tarefas que ficaram para o dia seguinte
    fprintf(out,"\n--------------------------------------------------\n");
    fprintf(out, "FICA PARA O DIA SEGUINTE\n");
    fprintf(out,"--------------------------------------------------\n");
    if (h.quant == 0) // Caso heap vazia
    	fprintf(out, "Agenda vazia! Nao restam tarefas para o dia seguinte.\n");
    else{ // Caso hep n�o vazia
    	// mostrar em ordem decrescente de import�ncia as tarefas
    	while (!HeapVazio(h)){  												// enquanto o heap n�o estiver vazio, escrever no arquivo de sa�da, em seguida retirar do heap para todos os elementos
			fprintf(out, "%2d       %s", h.elem[1].value, h.elem[1].desc);		// escreve a raiz no arquivo de sa�da
			Extract (&h);														// extrai do heap a raiz    		
		}
	}
    
    // finaliza��o
    Finalizar (&h); // libera espa�o de mem�ria alocado para o vetor de elementos do heap
    
    // Fechamento dos arquivos de entrada e sa�da
	fclose(in);
    fclose(out); 
    
    return 0;
}	

void Inicializar (struct heap * h, int MAX){						// fun��o que inicializa o heap
	h->elem = (struct noh*) malloc ((MAX+1)*sizeof (struct noh)); 	// aloca na mem�ria espa�o para o vetor de elementos
	h->quant = 0;													// a quantidade come�a com zero, j� que o heap est� vazio
	h->MAX = MAX;													// o m�ximo de elementos � guardado na vari�vel da struct do heap
}

void Insert (struct heap * h, struct noh elem){			// fun��o que insere um novo n� no heap
	struct noh temp;				// struct temp � auxiliar para a troca de elementos no heap
	int pai,						// inteiro pai guarda a posi��o do pai da sub�rvore em destaque 
		  i,						// inteiro i � auxuliar para o loop que faz varrer a �rvore de baixo para cima (bottom-up)
	  	pos;						// inteiro pos � auxiliar para a posi��o do elemento cuja posi��o varia at� se achar a posi��o correta no heap 
	pos = (h->quant) + 1;			// posi��o inicial do elemento adicionado � no final da fila
	h->quant++;						// incrementa a quantidade
	h->elem[pos] = elem;			// adiciona ao final da fila 
	i = pos/2; 						// posi��o do pai do novo elemento
	pai = i;						// posi��o do pai recebe o valor de pai do novo elemento
	while ( i >= 1){				// enqunto n�o se chegar na raiz, continua o loop (bottom-up)
		// Se n�o vale a propriedade estrutural
		if (h->elem[pai].value < h->elem[pos].value){		// caso o pai tenha menos import�ncia do que o filho, troca, j� que � heap de m�ximo
			temp = h->elem[pai];							// n� do pai � salvo na vari�vel tempor�ria temp
			h->elem[pai] = h->elem[pos];					// n� pai passa a ser o n� filho
			h->elem[pos] = temp;							// n� filho passa a ser o n� pai
			// troca realizada
		}
		i = i/2;  		// sobe na �rvore para o pai anterior
		pos = pai;		// posi��o do filho passa a ser a do pai
		pai = i;		// o pai sobe posi��o
	}
}

void Extract (struct heap * h){		// fun��o que extrair o elemento de maior prioridade
	struct noh temp;				// struct temp � auxiliar para a troca de elementos no heap
	int pai,						// inteiro pai guarda a posi��o do pai da sub�rvore em destaque 
		esq,			            // inteiro esq guarda a posi��o do filho esquerdo da sub�rvore em destaque 
		dir,						// inteiro dir guarda a posi��o do filho direito da sub�rvore em destaque 
		  i,						// inteiro i � auxuliar para o loop que faz varrer a �rvore de cima para baixo (top-bottom)
	  maior;						// inteiro maior guarda a posi��o do filho de maior import�ncia da sub�rvore em destaque
	h->elem[1] = h->elem[h->quant];	// coloca o �ltimo na raiz
	h->quant--;						// incrementa a quantidade
	i = 1;							// posi��o do elemento que foi colocado na raiz
	// situa��o inicial 
	pai = i;			// posi��o inicial do n� pai
	esq = 2*i;			// posi��o inicial do n� filho esquerdo
	dir = 2*i + 1;		// posi��o inicial do n� filho direito
	while ( i <= (h->quant)/2 ){						// enquanto n�o chegar no �ltimo n� "pai" da �rvore bin�ria, continua o loop (top-bottom)
		// Qual o maior filho?
		if (h->elem[esq].value > h->elem[dir].value)	// caso o filho esquerdo tenha maior import�ncia que o filho direito
			maior = esq;								// o filho esquerdo passa a ser o novo maior
		else maior = dir;								// caso contr�rio, o filho direito passa a ser o novo maior
		
		// se n�o vale a propriedade estrutural
		if (h->elem[pai].value < h->elem[maior].value){		// caso o pai seja menor do que o filho de maior import�ncia
			temp = h->elem[pai];							// n� do pai � salvo na vari�vel tempor�ria temp 
			h->elem[pai] = h->elem[maior];					// n� pai passa a ser o n� filho de maior import�ncia
			h->elem[maior] = temp;							// n� filho de maior import�ncia passa a ser o n� pai
		}
		
		// descida na �rvore bin�ria
		i = maior;  		// posi��o do novo pai
		pai = i;			// posi��o do filho passa a ser a do pai
		esq = 2*i;			// a posi��o do filho esquerdo, de acordo com a estrutura e l�gica adotada, passa a ser filho esquerdo do novo pai
		dir = 2*i + 1;		// a posi��o do filho direito, de acordo com a estrutura e l�gica adotada, passa a ser filho direito do novo pai
	}
}

void Finalizar (struct heap * h){	// fun��o para finalizar o heap
	free (h->elem);					// apaga o vetor de elementos
	h->elem = (struct noh*) NULL;	// apontamento para NULL
}
bool HeapVazio (struct heap h){		// fun��o que verifica se o hep est� vazio
	if (h.quant == 0)				// condi��o para heap vazio, n�o h� elementos
		return true;				
	else return false;
}
bool HeapCheio (struct heap h){		// fun��o que verifica se o heap est� cheio, ou seja, se quant == MAX
	if (h.quant == h.MAX)			// condi��o para heap cheio, a quantidade chegou ao limite MAX
		return true;
	else return false;
}
