#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
	char ** vet;
	int tamanhoAtual;
} heap;

void inicializarHeap(heap * h, int tamanhoMax){
	if(!(h->vet = (char**) malloc(sizeof(char *)*(tamanhoMax+1)))){
		exit(1);
	}
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
  
  }
}

void pop(heap * h){

	int pos = 1;
	char * str;

	// Imprime o menor valor (raiz) e substitui pelo ultimo valor
	if (h->vet[1] != NULL){
		printf("%s\n", h->vet[1]);

		h->vet[1] = h->vet[h->tamanhoAtual];
		
		free(h->vet[h->tamanhoAtual]);
		h->vet[h->tamanhoAtual] = NULL;
		h->tamanhoAtual--;
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
void heapSort(heap * h){
	while(h->tamanhoAtual != 0){
		pop(h);
	}

	free(h->vet);
}





int main(){
	heap * h = (heap *) malloc(sizeof(heap));
	inicializarHeap(h, 50);	
	char str[100];

	while(fgets(str, 100, stdin)){
		str[strlen(str) - 1] = '\0';

		insertHeap(h, str);
	}

	/*for(int i = 1; i <= h->tamanhoAtual; i++){
		printf("%s\n",h->vet[i]);
	}*/

	/*insertHeap(h, "aabcdefghijklmonpqrstuvwxyzabcdefghijkrtuvwxyzdefghijklmonpqrstuvwxyzabcdefghijklmn");
	insertHeap(h, "babcdefghijklmonpqrstuvwxyzabcdefghijkrtuvwxyzdefghijklmonpqrstuvwxyzabcdefghijklmn");
	insertHeap(h, "cabcdefghijklmonpqrstuvwxyzabcdefghijkrtuvwxyzdefghijklmonpqrstuvwxyzabcdefghijklmn");
	insertHeap(h, "fabcdefghijklmonpqrstuvwxyzabcdefghijkrtuvwxyzdefghijklmonpqrstuvwxyzabcdefghijklmn");
	insertHeap(h, "dabcdefghijklmonpqrstuvwxyzabcdefghijkrtuvwxyzdefghijklmonpqrstuvwxyzabcdefghijklmn");
	insertHeap(h, "eabcdefghijklmonpqrstuvwxyzabcdefghijkrtuvwxyzdefghijklmonpqrstuvwxyzabcdefghijklmn");
	insertHeap(h, "gabcdefghijklmonpqrstuvwxyzabcdefghijkrtuvwxyzdefghijklmonpqrstuvwxyzabcdefghijklmn");
	*/


	heapSort(h);
	free(h);


}
