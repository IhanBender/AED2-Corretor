// Autor: Ihan Bender
// Algoritmos adaptados: Distancia de levenshtein, incluindo MIN3(fonte: wikipedia); hash => python(fonte: vista em aula).
	// A distancia de levenshtein nao se adaptava a todas as especificacoes do trabalho, tendo sido necessaria uma mudanca.
	// A Funcao hash sofreu esperimentacoes e alteracoes, tendo chegado ao estado atual

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c))) // Usado para calculo da distancia de levenshtein (descobre o menor dos valores)

/* ------------------------------------------------ Structs usadas ----------------------------------------------- */

typedef struct {	// Cabecario da heap
  char ** vet;
  int tamanhoAtual;
} heap;

typedef struct node{		// nodos com as strings (lista encadeada)
	struct node * prox;
	char * word;
} node;

typedef struct hashHead{	// Cabecario da hash
	int fCarga;			// Fator de carga máximo. Poderá ser alterado na funcao que cria a tabela
	int qnt;			// Quantidade de Strings armazenadas na estrutura
	int tam;			// Controla o tamanho atual do vetor (para nao precisar calcular percorrendo)
	node ** vetor;		// Vetor de ponteiros para nodos
} hashHead;

/* --------------------------------------- Header das Funções -------------------------------------------------- */

//Operações hash
unsigned int hash(char * str);				// Calcula um indice para a tabela hash 
hashHead * createHash(hashHead * head);		// Cria um cabecario para uma tabela hash
hashHead * reHash(hashHead * head);			// Dobra o tamanho do vetor e rearmazena todos os valores
bool insert(hashHead * head, char * str);	// Insere uma string da tabela
bool delete(hashHead * head, char * str);	// Deleta uma string da tabela
bool search(hashHead * head, char * str);	// Procura uma string na tabela que seja igual ao parametro. Retorna NULL se nao encontrar
void freeHash(hashHead * head);				// Libera toda memoria da hash

// Manipulaçao de strings
void toLowerCase(char * str);					// Deixa uma string apenas com letras minusculas

// Sugestoes
int levenshtein_pp(char *s1, char *s2);			// Retorna 1 caso as strings sejam "semelhantes" (de acordo com as especificacoes), 0 caso sejam iguais e 2 ou mais caso sejam "muito diferentes"
heap * buscaSugestoes(hashHead * head, char * str);	// Busca as palavras ordenadas e armazena em uma heap

// Heap
void inicializarHeap(heap * h, int tamanhoMax);
int pai(int i);
int filhoEsquerda(int i);
int filhoDireita(int i);
void insertHeap(heap * h, char * str);
void pop(heap * h);
void heapSort(heap * h);

/* ---------------------------------------- MAIN ---------------------------------------------------------- */

int main(){
	
	hashHead * head;			// Cabecario da hash
	head = createHash(head);	// Cria a tabela hash (50 posicoes inicialmente)
	char str[100], analizedString[100];	// Strings utilizada para leitura das palavras
	heap * heapHead = (heap *) malloc(sizeof(heap));

	analizedString[0] = '\0';
	head = createHash(head);


	while(fgets(str, 100, stdin)){

		if (str[0] == '*')
		{
			freeHash(head);		// Libera memoria e encerra a execucao quando o valor lido for *
			return 0;
		}
		
		toLowerCase(str);
		str[strlen(str) - 1] = '\0';

		if (str[0] == '+'){		// Caso tenha lido um +
			if (analizedString[0] == '\0'){		// Verifica se existe uma palavra sendo analizada
				printf("fail %s\n", analizedString);
			}
			else{
				if (insert(head, analizedString)){
					printf("ok %s\n",analizedString);
				}
				else{
					printf("fail %s\n", analizedString);
				}
			}
		}
		else{
			if (str[0] == '-'){		// Caso tenha lido um -
				if (analizedString[0] == '\0'){
					printf("fail %s\n", analizedString);
				}
				else{
					if (delete(head, analizedString)){
						printf("ok %s\n",analizedString);
					}
					else{
						printf("fail %s\n", analizedString);
					}
				}
			}
			else{	// Quando uma palavra é lida

				strcpy(analizedString, str);		// Copia a string lida para a string auxiliar
				if (search(head, analizedString)){	// Caso exista, ok
					printf("ok %s\n",analizedString);
				}
				else{
					heapHead = buscaSugestoes(head, analizedString);	// Busca sugestoes
					if (heapHead->vet[1] == NULL){		// Caso nao haja
						printf("not found\n");	
					}
					else{							// Caso haja						
						while(heapHead->tamanhoAtual != 0){
							pop(heapHead);
						}
					}
				}
			}
		}
	}

}

/* ------------------------------------------------ Funcoes -------------------------------------------------- */

unsigned int hash(char * str){
	char p = str[0];
	unsigned int x = p << 1;
	int i, size = strlen(str);

	for(i=1; i<size; i++){
		x = (1000003 * x) ^ p;
		p = str[i];
		x = x * 19 ^ p;
	}

	x = x ^ size;
	return x;	
}

hashHead * createHash(hashHead * head){			// Cria um cabecario para uma tabela hash

	if(!(head = (hashHead *) malloc(sizeof(hashHead))))
		return NULL;
	
	head->fCarga = 2;	// Valor decidido como fator de carga máximo
	head->qnt = 0;		// Valor inicial de strings
	head->tam = 50;		// Numero de posições do vetor
	 		
	if(!(head->vetor = (node **) malloc(sizeof(node *) * head->tam)))	// Aloca 50 posicoes de ponteiros para nodos
		return NULL;

	return head;
}

bool insert(hashHead * head, char * str){		// Insere uma string da tabela

	
	node * percorre;
	node * newNode;

	if(head->qnt+1 >= head->fCarga * head->tam){
		head = reHash(head);
	}

	int h = hash(str)%head->tam;	

	if(head->vetor[h] == NULL){

		head->vetor[h] = (node *) malloc(sizeof(node));
		head->vetor[h]->word = (char *) malloc(sizeof(char) * strlen(str) + 1);	// Aloca espaco suficiente para a string
		strcpy(head->vetor[h]->word, str);		// Copia a string para o nodo
		head->vetor[h]->prox = NULL;			// Aponta prox para NULL

	}
	else{
		percorre = head->vetor[h];
		while(percorre->prox != NULL){			// Verifica se a palavra ja existe
			if(strcmp(percorre->word, str) == 0)
				return false;

			percorre = percorre->prox;
		}
		if(strcmp(percorre->word, str) == 0)
			return false;

		newNode = (node *) malloc(sizeof(node));
		newNode->word = (char *) malloc(sizeof(char) * strlen(str) + 1);	// Aloca espaco suficiente para a string
		strcpy(newNode->word, str);	// Copia a string para o nodo
		newNode->prox = NULL;			// Aponta prox para NULL
		percorre->prox = newNode;		// Coloca newNode para o fim da lista encadeada

	}
	
	head->qnt++;
	return true;	
}

hashHead * reHash(hashHead * head){				// Dobra o tamanho do vetor e rearmazena todos os valores

	int i = 0;
	node * percorreNode, * deadNode;
	node ** newVetor, **oldVetor;

	if(!(newVetor = (node **) malloc(sizeof(node *) * head->tam * 2))){		// Tenta criar um vetor de ponteiros com o dobro do tamanho atual
		return NULL;
	}

	// Atualiza o valor qnt da hash (durante o rehash, todos os dados serao reinseridos)
	head->qnt = 0;
	oldVetor = head->vetor;		// Guarda o vetor antigo em um ponteiro
	head->vetor = newVetor;		// Hash recebe novo vetor (vazio)
	head->tam = head->tam * 2;	// Dobra a variavel que controla o tamanho do vetor


	while(i < head->tam/2){
		percorreNode = oldVetor[i];
		while(percorreNode != NULL){
			deadNode = percorreNode;
			percorreNode = percorreNode->prox;
			insert(head, deadNode->word);		// Insere a string do antigo vetor no novo
			free(deadNode->word);			// Libera memoria da string
			free(deadNode);				// Libera memoria do nodo
		}
		i++;
	}

	free(oldVetor);
	
	return head;
}

bool search(hashHead * head, char * str){		// Procura uma string na tabela que seja igual ao parametro. Retorna NULL se nao encontrar

	node * percorre;
	int h = hash(str) % head->tam;	// Variavel criada para realizar funcao hash apenas uma vez (devido ao seu custo alto para strings grandes)
	

	if(head->vetor[h] == NULL)	// Se a posicao no vetor estiver vazia, a palavra ainda nao foi inserida
		return false;

	percorre = head->vetor[h];	// Caso a posicao do vetor nao seja nula, deve-se procurar a palavra na lista encadeada

	while(percorre != NULL){
		if(strcmp(str, percorre->word) == 0)	// Testa igualdade das strings e retorna em caso positivo (strcmp() == 0)
			return true;

		percorre = percorre->prox;	// Incrementa o ponteiro percorre caso nao sejam iguais
	}


	return false;	// Caso o loop termine e nao seja possivel encontrar o valor, retorna-se falso indicando que a string nao esta na ED
}

bool delete(hashHead * head, char * str){		// Deleta uma string da tabela

	int h = hash(str) % head->tam;
	node * percorre, * dead;

	if(head->vetor[h] == NULL)	// Caso a posicao em que a string devesse estar seja nula
		return false;

	percorre = head->vetor[h];

	if(strcmp(head->vetor[h]->word, str) == 0){	// Caso encontre na primeira posicao
		head->vetor[h] = percorre->prox;
		free(percorre->word);
		free(percorre);
		head->qnt--;
		return true;
	}
	
	while(percorre->prox != NULL){		// Analisa sempre o proximo valor
		if (strcmp(percorre->prox->word, str) == 0){
			dead = percorre->prox;
			percorre->prox = dead->prox;
			free(dead->word);
			free(dead);
			head->qnt--;
			return true;
		}

		percorre = percorre->prox;
	}

	return false;
}	

void freeHash(hashHead * head){					// Libera toda memoria da hash
	node * percorreNode, * percorreVetor, * deadNode;
	int i = 0;
	
	while(i < head->tam){
		percorreVetor = head->vetor[i];		// PercorreVetor passa a apontar para a posicao seguinte no vetor
		percorreNode = percorreVetor;			// PercorreNode passa a apontar para a posicao atual do vetor
		while(percorreNode != NULL){			// Enquanto houverem nodos na lista encadeada, segue o loop
			deadNode = percorreNode;			// deadNode aponta para o nodo atual
			percorreNode = percorreNode->prox;	// Incrementa o ponteiro percorreNode
			free(deadNode->word);
			free(deadNode);
		}
		i++;	
	}

	free(head->vetor);
	free(head);
}	

void inicializarHeap(heap * h, int tamanhoMax){
  h->vet = (char**) malloc(sizeof(char *)*(tamanhoMax+1));
  h->tamanhoAtual = 0;
}


int pai(int i){
  return i/2;
}

int filhoEsquerda(int i){
  return 2*i;
}

int filhoDireita(int i){
  return 2*i + 1;
}


void pop(heap * h){
	printf("%s\n",h->vet[1]);
	int pos = 1;
	int value;
	char auxStr[100];

	strcpy(h->vet[1], h->vet[h->tamanhoAtual]);	
	free(h->vet[h->tamanhoAtual]);
	h->tamanhoAtual--;


	while(filhoEsquerda(pos) <= h->tamanhoAtual || filhoEsquerda(pos) <= h->tamanhoAtual){
		
		value = strcmp(h->vet[filhoEsquerda(pos)], h->vet[filhoDireita(pos)]);
		if(value < 0){
			if(strcmp(h->vet[filhoEsquerda(pos)], h->vet[pos]) < 0){
				strcpy(auxStr, h->vet[pos]);					// Troca as strings entre os dois
				strcpy(h->vet[pos], h->vet[filhoEsquerda(pos)]);
				strcpy(h->vet[filhoEsquerda(pos)], auxStr);
				pos = filhoEsquerda(pos);
			}
			else{
				break;
			}
		}
		else if(value < 0){
			if(strcmp(h->vet[filhoDireita(pos)], h->vet[pos]) < 0){
				strcpy(auxStr, h->vet[pos]);					// Troca as strings entre os dois
				strcpy(h->vet[pos], h->vet[filhoDireita(pos)]);
				strcpy(h->vet[filhoDireita(pos)], auxStr);
				pos = filhoDireita(pos);
			}	
		}
		else{
			break;
		}

	}
}

void insertHeap(heap * h, char * str){

	char auxStr[100];
	int position = h->tamanhoAtual + 1;

	h->vet[position] = (char *) malloc(sizeof(char) * strlen(str));
	strcpy(h->vet[position], str);	// Copia string passada para a heap
	h->tamanhoAtual++;

	while(pai(position) != 0){	// Enquanto o pai for maior q o filho
		if(strcmp(h->vet[pai(position)], h->vet[position]) > 0 ){
			strcpy(auxStr, h->vet[position]);					// Troca as strings entre os dois
			strcpy(h->vet[position], h->vet[pai(position)]);
			strcpy(h->vet[pai(position)], auxStr);
			position = pai(position);
		}
		else
			break;		
	}
}


void toLowerCase(char * str){					// Deixa uma string apenas com letras minusculas

	int i, size = strlen(str);

	for(i=0; i < size; i++){
		if(str[i] >= 65 && str[i] <= 90)
			str[i] = str[i] + 32;
	}

	return;
}	


// Otimizar para sair antes
int levenshtein_pp(char *s1, char *s2) {
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    short int flag = 0;

    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            if( s1[y-1] == s2[x] && s1[y] == s2[x-1] && flag == 1){
            	column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));	
            }
            else{
            	column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : s1[y-1] == s2[x] && s1[y] == s2[x-1] ? 0 : 1));
            	if (s1[y-1] == s2[x] && s1[y] == s2[x-1]){
            		flag = 1;
            	}
            }
     
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
}

heap * buscaSugestoes(hashHead * head, char * str){		// imprime as palavras sugeridas ordenadas
	heap * heapHead = (heap *) malloc(sizeof(heap));
	inicializarHeap(heapHead, head->qnt);
	node * percorre;
	int i = 0;

	for (int i = 0; i < head->tam; i++){		// Percorre toda a hash
		percorre = head->vetor[i];
		while(percorre != NULL){
			if (levenshtein_pp(percorre->word, str) == 1){	// Seleciona os valores que devem ser sugeridos
				insertHeap(heapHead, percorre->word);		// Insere os valores na heap
			}

			percorre = percorre->prox;
		}
	}

	return heapHead;
}


void heapSort(heap * h){
	while(h->tamanhoAtual != 0){
		pop(h);
	}

	free(h->vet);
}
