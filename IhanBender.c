#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node{		// nodos com as strings
	struct node * prox;
	char * word;
} node;

typedef struct hashHead{
	int fCarga;		// Fator de carga máximo. Poderá ser alterado na funcao que cria a tabela
	int qnt;		// Quantidade de Strings armazenadas na estrutura
	int tam;		// Controla o tamanho atual do vetor (para nao precisar calcular percorrendo)
	node ** vetor;		// Vetor de ponteiros para nodos
} hashHead;

unsigned int hash(char * str);				// Calcula um indice para a tabela hash 

hashHead * createHash(hashHead * head);			// Cria um cabecario para uma tabela hash
hashHead * reHash(hashHead * head);			// Dobra o tamanho do vetor e rearmazena todos os valores

bool insert(hashHead * head, char * str);	// Insere uma string da tabela
bool delete(hashHead * head, char * str);	// Deleta uma string da tabela
bool search(hashHead * head, char * str);	// Procura uma string na tabela que seja igual ao parametro. Retorna NULL se nao encontrar
void freeHash(hashHead * head);				// Libera toda memoria da hash

void toLowerCase(char * str);				// Deixa uma string apenas com letras minusculas
int compareString(char * str);				// Compara strings e verifica se é igual ou se pode ser tratada como sugestão


int main(){
	
	hashHead * head;
	head = createHash(head);
	char str[100];
	node * percorre;
	int i = 0, j = 0;
	float vazio = 0, repeat = 0;

	while(fgets(str, 100, stdin)){
		str[strlen(str) - 1] = '\0';	// Substitui o '\n' por '\0'
		toLowerCase(str);
		if(insert(head, str) == false)
			repeat++;


	}
	if (search(head, str))
	{
		printf("%s\n", str);
	}

	while(i < head->tam){
		percorre = head->vetor[i];
		if(head->vetor[i] == NULL)
			vazio++;
		while(percorre != NULL){
			printf("%d,%d : %s\n",i, j, percorre->word);
			j++;
			percorre = percorre->prox;
		}
		i++;
		
		j = 0;	
	}

	if(search(head, "cigarette") == false)
		printf("Not found\n");
	
	printf("Tam: %d\nQnt: %d\n", head->tam, head->qnt);
	printf("Vazio: %f\n", vazio);
	printf("Repetidas: %0f\n", repeat);

	freeHash(head);
}

unsigned int hash(char * str){
	char p = str[0];
	unsigned int x = p << 1;
	int i, size = strlen(str);

	for(i=0; i<size; i++){
		x = 1000003 * x ^ p;
		p = str[i];
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

void toLowerCase(char * str){					// Deixa uma string apenas com letras minusculas

	int i, size = strlen(str);

	for(i=0; i < size; i++){
		if(str[i] >= 65 && str[i] <= 90)
			str[i] = str[i] + 32;
	}

	return;
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
