// Autor: Ihan Bender

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
void buscaSugestoes(hashHead * head, heap * heapHead, char * str);	// Busca as palavras ordenadas e armazena em uma heap
int compareString(char *s1, char *s2);

// Heap
void inicializarHeap(heap * h, int tamanhoMax);
int pai(int i);
int filhoEsquerda(int i);
int filhoDireita(int i);
void insertHeap(heap * h, char * chave);
void pop(heap * h);
void heapSort(heap * h);
void minHeapify(heap * h, int i);

/* ---------------------------------------- MAIN ---------------------------------------------------------- */

int main(){
	
	hashHead * head;			// Cabecario da hash
	head = createHash(head);	// Cria a tabela hash (50 posicoes inicialmente)
	char str[101], analizedString[101];	// Strings utilizada para leitura das palavras
	heap * heapHead = (heap *) malloc(sizeof(heap));

	analizedString[0] = '\0';


	while(fgets(str, 101, stdin)){

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
					buscaSugestoes(head, heapHead, analizedString);	// Busca sugestoes
					
					if (heapHead->tamanhoAtual == 0){		// Caso nao haja
						printf("not found\n");	
					}
					else{							// Caso haja
						heapSort(heapHead);
					}
				}
			}
		}
	}

}

/* ------------------------------------------------ Funcoes -------------------------------------------------- */


//	------------------------------------------------ Hash -------------------------------------------------------

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
	char * newStr = (char *) malloc(sizeof(char) * strlen(str) + 1);
	strcpy(newStr, str);

	if(head->qnt+1 >= head->fCarga * head->tam){
		head = reHash(head);
	}

	int h = hash(str)%head->tam;	

	if(head->vetor[h] == NULL){

		head->vetor[h] = (node *) malloc(sizeof(node));
		head->vetor[h]->word = newStr;		// Copia a string para o nodo
		head->vetor[h]->prox = NULL;			// Aponta prox para NULL

	}
	else{
		percorre = head->vetor[h];
		while(percorre->prox != NULL){			// Verifica se a palavra ja existe
			if(strcmp(percorre->word, str) == 0)
				return false;

			percorre = percorre->prox;
		}
		if(strcmp(percorre->word, str) == 0){
			free(newStr);
			return false;
		}

		newNode = (node *) malloc(sizeof(node));
		newNode->word = newStr;			// Copia a string para o nodo
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
	for (int i = 0; i < head->tam * 2; ++i)
	{
		newVetor[i] = NULL;
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

// Reajustar
bool delete(hashHead * head, char * str){		// Deleta uma string da tabela

	int h = hash(str) % head->tam;
	node * percorre, * dead;
	char * deadc;

	if(head->vetor[h] == NULL)	// Caso a posicao em que a string devesse estar seja nula
		return false;

	percorre = head->vetor[h];

	if(strcmp(head->vetor[h]->word, str) == 0){	// Caso encontre na primeira posicao
		head->vetor[h] = percorre->prox;
		deadc = percorre->word;
		free(deadc);
		free(percorre);
		head->qnt--;
		return true;
	}
	
	while(percorre->prox != NULL){		// Analisa sempre o proximo valor
		if (strcmp(percorre->prox->word, str) == 0){
			dead = percorre->prox;
			percorre->prox = dead->prox;
			deadc = dead->word;
			free(deadc);
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

// ------------------------------------------------ Heap -------------------------------------------------------------

void inicializarHeap(heap * h, int tamanhoMax){
	h->vet = (char**) malloc(sizeof(char *)*(tamanhoMax+1));
 	h->tamanhoAtual = 0;
	for (int i = 1; i <= h->tamanhoAtual; ++i){
  		h->vet[i] = NULL;
  	}
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

void minHeapify(heap * h, int i){
  int esq = filhoEsquerda(i);
  int dir = filhoDireita(i);
  char * temp;
  int menor = i;

  if ((esq <= h->tamanhoAtual) && strcmp(h->vet[esq],h->vet[i]) < 0) 
  	menor = esq;

  if ((dir <= h->tamanhoAtual) && strcmp(h->vet[dir],h->vet[menor]) < 0)
  	menor = dir;

  if (menor != i) {
  	temp = h->vet[i];
	h->vet[i] = h->vet[menor];
	h->vet[menor] = temp;
  	minHeapify(h, menor);
  }
}

void pop(heap * h){

	int pos = 1;
	char * str;
	char * dead = h->vet[1];

	// Imprime o menor valor (raiz) e substitui pelo ultimo valor
	if (h->tamanhoAtual != 0){
		printf("%s\n", h->vet[1]);
		h->vet[1] = h->vet[h->tamanhoAtual];
		h->vet[h->tamanhoAtual] = NULL;
		h->tamanhoAtual--;
		free(dead);
	}
		
		minHeapify(h, 1);
}


void insertHeap(heap * h, char * chave){
  int i;
  char * temp;
  char * str;

  (h->tamanhoAtual)++;
  i = h->tamanhoAtual;
  str = malloc(sizeof(char) * strlen(chave));
  strcpy(str, chave);
  h->vet[i] = str;

  while ((i>1) && (strcmp(h->vet[pai(i)],h->vet[i]) > 0)){

  	// troca vet[i] com vet[pai(i)] 
  	temp = h->vet[i];
  	h->vet[i] = h->vet[pai(i)];
  	h->vet[pai(i)] = temp;
 
	i = pai(i);
  }
  return;
}

void buscaSugestoes(hashHead * head, heap * heapHead, char * str){		// imprime as palavras sugeridas ordenadas
	
	inicializarHeap(heapHead, head->qnt);
	printf("%d\n", head->qnt);
	node * percorre;
	int i;

	for (i = 0; i < head->tam; i++){		// Percorre toda a hash
		percorre = head->vetor[i];
		while(percorre != NULL){
			if (compareString(percorre->word, str) == 1){	// Seleciona os valores que devem ser sugeridos
				insertHeap(heapHead, percorre->word);		// Insere os valores na heap
			}

			percorre = percorre->prox;
		}
	}
}


void heapSort(heap * h){
	while(h->tamanhoAtual != 0){
		pop(h);
	}

	free(h->vet);
}

// ----------------------------------------------- Manutençao de Strings ----------------------------------------------------

void toLowerCase(char * str){					// Deixa uma string apenas com letras minusculas

	int i, size = strlen(str);

	for(i=0; i < size; i++){
		if(str[i] >= 65 && str[i] <= 90)
			str[i] = str[i] + 32;
	}

	return;
}	


int compareString(char *s1, char *s2){
	
	int i, j, flag = 0;
	unsigned int s1len = strlen(s1);
	unsigned int s2len = strlen(s2);
	int diff = s1len - s2len;

	if(diff >= 2 || diff <= -2){
        return 2;
    }

    if (diff < 0) diff - s2len;
    else diff = s1len;

    // Primeiro valor de iguais
    
    for (int i = 0, j = 0; i < diff || j < diff; i++, j++){
    	if (s1[i] != s2[j]){
    		if (s1[i+1] == s2[j] && s1[i] == s2[j+1]){	// Caso da permutacao
    			i++;
    			j++;
    		}
    		else{	// Caso de uma var a mais ou menos
    			if (s1[i+1] == s2[j]){
    				i++;
    			}
    			if (s1[i] == s2[j+1]){
    				j++;
    			}
    		}

    		flag++;

			if (flag > 1){
				return 2;
  			}
   		}
   	}

   	return flag;
}