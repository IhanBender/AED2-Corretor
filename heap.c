#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char ** vet;
	int tamanhoAtual;
} heap;

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





int main(){
	heap * h = (heap *) malloc(sizeof(heap));
	inicializarHeap(h, 50);	


	insertHeap(h, "strap");
	insertHeap(h, "tap");
	insertHeap(h, "wrap");
	insertHeap(h, "tray");
	insertHeap(h, "trip");
	/*insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");
	insertHeap(h, "AbdelAziz");*/



	while(h->tamanhoAtual != 0){
		pop(h);
	}


}
