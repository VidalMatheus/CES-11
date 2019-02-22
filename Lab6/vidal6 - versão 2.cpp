/*  Copyright by Matheus Vidal de Menezes        */
/*  Turma 4                                  	 */
/*  Lab 06: Agenda Eletrônica                    */
/*  Programa compilado com Dev-C++ 5.11          */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------Definição da estrutura de dados---------------------
struct noh{				// cada nó do heap é uma estrutura com importância e descrição
	char desc[79];		// string description é auxiliar para guardar a descrição da tarefa
	int value;			// inteiro que guarda a importância de cada tarefa numa escala de 1 a 99
};

struct heap{			// fila de prioridade com heap
	struct noh * elem; 	// ponteiro do tipo struct noh para alocar o vetor de elementos do heap
	int quant,			// inteiro quant serve para armazenar a quantidade de elementos do heap
		  MAX;			// inteiro MAX serve para guardar a quantidade máxima possível de elementos no heap
};

//-------------Protótipos de funções--------------------------------
void Inicializar (struct heap * h, int MAX);			// função que inicializa o heap
void Insert (struct heap * h, struct noh elem);			// função que insere um novo nó no heap
struct noh Extract (struct heap * h);					// função que extrair o elemento de maior prioridade, retornando o nó raiz
void Finalizar (struct heap * h);						// função para finalizar o heap
bool HeapVazio (struct heap h);							// função que verifica se o hep está vazio
bool HeapCheio (struct heap h);							// função que verifica se o heap está cheio, ou seja, se quant == MAX

int main(){
	struct heap h; 							// struct heap h é a variável que armazena toda a estrutura do heap
	struct noh aux;							// struct noh aux é auxiliar para receber o nó raiz da função Extract() para escrevê-lo no arquivo de saída
	char  line[79],       					// string line é auxiliar para pular linha 
		  task[29];     				    // string action é auxiliar para identificar as ações: NOVA, PROXIMA
    int 	  MAX,                          // inteiro que guarda o número máximo de elementos no heap
				i;			 				// inteiro i é variável auxiliar que serve para iterações (laços)
	struct noh elem;						// struct noh elem é auxiliar para ser inerido no heap	
    FILE *in, *out; 	 					// variáveis para os aqrquivos de entrada e saída
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
    //Criação do Cabeçalho do arquivo de saída
    fprintf(out,"*AGENDA ELETRONICA*\n");
    fprintf(out,"Simple and efficient!\n");
    fprintf(out,"All tasks organized!\n");
    fprintf(out,"CES-11 - ITA\n");
    fprintf(out,"Prof.: Armando\n");
    fprintf(out,"--------------------------------------------------\n");
    fprintf(out, "RESPOSTAS DAS CONSULTAS\n");
    fprintf(out,"--------------------------------------------------\n");
    //Fim do cabeçalho do arquivo de saída
    
    for(i = 1; i <= 5; i++){// Pular as 5 primeiras linhas
        fgets(line,79,in);
    }
 
    fscanf(in, "%d ", &MAX); // Captura o número máximo de elementos no heap
    
    //---------------Inicialização do Heap--------------
    Inicializar (&h, MAX);

	for(i = 1; i <= 3; i++){// Pular as 3 próximas linhas
        fgets(line,79,in);
    }

    fscanf(in,"%s ", task); // Captura o comando a ser realizado

	while (strcmp(task,"FIM") != 0){
        if (!strcmp(task,"NOVA")){			// Adiciona nova tarefa a ser realizada na agenda eletrônica
        	fscanf(in,"%d ", &elem.value); 	// Guarda a importância da tarefa
			fgets(elem.desc, 79, in);  		// Guarda a descrição da tarefa
			if (HeapCheio(h))				// Caso heap cheio, ou seja, h->quant == h->MAX, mensagem de ERRO
				fprintf(out, "ERRO     Agenda cheia. Impossivel inserir.\n");
			else Insert (&h, elem);			// Insere elem no heap
	    } 
        if (!strcmp(task,"PROXIMA")){		// Extrai o elemento de maior prioridade (na raiz da árvore binária) e escreve no arquivo de saída 
			if (HeapVazio(h))				// Caso heap vazio, mensangem de AVISO
				fprintf(out, "AVISO    Nao ha tarefas na agenda  :-)\n");
			else {
				aux = Extract (&h);										// extrai do heap a raiz 
				fprintf(out, "%2d       %s", aux.value,aux.desc);		// escreve a raiz no arquivo de saída, depois de extrair do heap a raiz 
			}
        }
        // Caso o arquivo de entrada não tenha nenhum comando PROXIMA, o RELATÓRIO DE CONSULTAS ficará vazio, isto é, nada será mostrado

		fscanf(in,"%s ", task); // Captura o próximo comando a ser realizada 
    }
    // Tarefas que ficaram para o dia seguinte
    fprintf(out,"\n--------------------------------------------------\n");
    fprintf(out, "FICA PARA O DIA SEGUINTE\n");
    fprintf(out,"--------------------------------------------------\n");
    if (h.quant == 0) // Caso heap vazia
    	fprintf(out, "Agenda vazia! Nao restam tarefas para o dia seguinte.\n");
    else{ // Caso hep não vazia
    	// mostrar em ordem decrescente de importância as tarefas
    	while (!HeapVazio(h)){  										// enquanto o heap não estiver vazio, escrever no arquivo de saída, em seguida retirar do heap para todos os elementos
				aux = Extract (&h);										// extrai do heap a raiz 
				fprintf(out, "%2d       %s", aux.value,aux.desc);		// escreve a raiz no arquivo de saída, depois de extrair do heap a raiz  		
		}
	}
    
    // finalização
    Finalizar (&h); // libera espaço de memória alocado para o vetor de elementos do heap
    
    // Fechamento dos arquivos de entrada e saída
	fclose(in);
    fclose(out); 
    
    return 0;
}	

void Inicializar (struct heap * h, int MAX){						// função que inicializa o heap
	h->elem = (struct noh*) malloc ((MAX+1)*sizeof (struct noh)); 	// aloca na memória espaço para o vetor de elementos
	h->quant = 0;													// a quantidade começa com zero, já que o heap está vazio
	h->MAX = MAX;													// o máximo de elementos é guardado na variável da struct do heap
}

void Insert (struct heap * h, struct noh elem){			// função que insere um novo nó no heap
	struct noh temp;				// struct temp é auxiliar para a troca de elementos no heap
	int pai,						// inteiro pai guarda a posição do pai da subárvore em destaque 
		  i,						// inteiro i é auxuliar para o loop que faz varrer a árvore de baixo para cima (bottom-up)
	  	pos;						// inteiro pos é auxiliar para a posição do elemento cuja posição varia até se achar a posição correta no heap 
	pos = (h->quant) + 1;			// posição inicial do elemento adicionado é no final da fila
	h->quant++;						// incrementa a quantidade
	h->elem[pos] = elem;			// adiciona ao final da fila 
	i = pos/2; 						// posição do pai do novo elemento
	pai = i;						// posição do pai recebe o valor de pai do novo elemento
	while ( i >= 1){				// enqunto não se chegar na raiz, continua o loop (bottom-up)
		// Se não vale a propriedade estrutural
		if (h->elem[pai].value < h->elem[pos].value){		// caso o pai tenha menos importância do que o filho, troca, já que é heap de máximo
			temp = h->elem[pai];							// nó do pai é salvo na variável temporária temp
			h->elem[pai] = h->elem[pos];					// nó pai passa a ser o nó filho
			h->elem[pos] = temp;							// nó filho passa a ser o nó pai
			// troca realizada
		}
		i = i/2;  		// sobe na árvore para o pai anterior
		pos = pai;		// posição do filho passa a ser a do pai
		pai = i;		// o pai sobe posição
	}
}

struct noh Extract (struct heap * h){		// função que extrair o elemento de maior prioridade, retornando-o 
	struct noh raiz,				// struct raiz é auxiliar para guardar o elemnento de maior prioridade do heap, que está na raiz da árvore binária
			   temp;				// struct temp é auxiliar para a troca de elementos no heap
	int pai,						// inteiro pai guarda a posição do pai da subárvore em destaque 
		esq,			            // inteiro esq guarda a posição do filho esquerdo da subárvore em destaque 
		dir,						// inteiro dir guarda a posição do filho direito da subárvore em destaque 
		  i,						// inteiro i é auxuliar para o loop que faz varrer a árvore de cima para baixo (top-bottom)
	  maior;						// inteiro maior guarda a posição do filho de maior importância da subárvore em destaque
	raiz = h->elem[1];				// guarda a raiz, que é o nó da posição 1 no vetor de elementos
	h->elem[1] = h->elem[h->quant];	// coloca o último na raiz
	h->quant--;						// incrementa a quantidade
	i = 1;							// posição do elemento que foi colocado na raiz
	// situação inicial 
	pai = i;			// posição inicial do nó pai
	esq = 2*i;			// posição inicial do nó filho esquerdo
	dir = 2*i + 1;		// posição inicial do nó filho direito
	while ( i <= (h->quant)/2 ){						// enquanto não chegar no último nó "pai" da árvore binária, continua o loop (top-bottom)
		// Qual o maior filho?
		if (h->elem[esq].value > h->elem[dir].value)	// caso o filho esquerdo tenha maior importância que o filho direito
			maior = esq;								// o filho esquerdo passa a ser o novo maior
		else maior = dir;								// caso contrário, o filho direito passa a ser o novo maior
		
		// se não vale a propriedade estrutural
		if (h->elem[pai].value < h->elem[maior].value){		// caso o pai seja menor do que o filho de maior importância
			temp = h->elem[pai];							// nó do pai é salvo na variável temporária temp 
			h->elem[pai] = h->elem[maior];					// nó pai passa a ser o nó filho de maior importância
			h->elem[maior] = temp;							// nó filho de maior importância passa a ser o nó pai
		}
		
		// descida na árvore binária
		i = maior;  		// posição do novo pai
		pai = i;			// posição do filho passa a ser a do pai
		esq = 2*i;			// a posição do filho esquerdo, de acordo com a estrutura e lógica adotada, passa a ser filho esquerdo do novo pai
		dir = 2*i + 1;		// a posição do filho direito, de acordo com a estrutura e lógica adotada, passa a ser filho direito do novo pai
	}
	return raiz;			// função Extract() retorna o elemento de maior importância
}

void Finalizar (struct heap * h){	// função para finalizar o heap
	free (h->elem);					// apaga o vetor de elementos
	h->elem = (struct noh*) NULL;	// apontamento para NULL
}
bool HeapVazio (struct heap h){		// função que verifica se o hep está vazio
	if (h.quant == 0)				// condição para heap vazio, não há elementos
		return true;				
	else return false;
}
bool HeapCheio (struct heap h){		// função que verifica se o heap está cheio, ou seja, se quant == MAX
	if (h.quant == h.MAX)			// condição para heap cheio, a quantidade chegou ao limite MAX
		return true;
	else return false;
}
