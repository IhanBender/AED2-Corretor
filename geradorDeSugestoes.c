#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 97 - 122

int main(){

	char str[] = "abcdefghijklmonpqrstuvwxyzabdasfasffslmnopqrstuvwxysklmonpqrstuvwxyzabcdefghijklmn";
//abcdefghijklmonpqrstuvwxyzabcdefghijkrtuvwxyzdefghijklmonpqrstuvwxyzabcdefghijklmn
//abcdefghijklmonpqrstuvwxyzabcdefghijklmnopqrstuvwxyjklmonpqrstuvwxyzabcdefghijklmn
//abcdefghijklmonpqrstuvwxyzabdasfasffslmnopqrstuvwxysklmonpqrstuvwxyzabcdefghijklmn

	int tam = strlen(str), i, j;
	char str2[tam+2], letra, aux;

	str2[tam+1] = '\0';

	
	// Gera caso 1
	for(i=0; i<=tam; i++){
		for(j=0; j<i; j++){
			str2[j] = str[j];
		}		
		str2[j] =  'a';
		for(j=j+1; j<=tam; j++){
			str2[j] = str[j-1];
		}
		printf("%s\n+\n",str2);
		for(letra = 'b'; letra <= 'z'; letra++){
			str2[i] = letra;
			printf("%s\n+\n",str2);
		}
	}
	
	// Gera caso 2
	str2[tam-1] = '\0';
	
	for(i=0; i<tam; i++){
		for(j=0; j<i; j++){
			str2[j] = str[j];
		}
		for(j=i; j<=tam; j++){
			str2[j] = str[j+1];
		}
		printf("%s\n+\n", str2);
	}

	

	// Gera caso 3

	for(i=0; i<tam-1; i++){
		for(j=0; j<i; j++){
			str2[j] = str[j];
		}		
		
		if(str[j] != str[j+1]){
			str2[j+1] = str[j];
			str2[j] = str[j+1];
			for(j=i+2; j<=tam; j++){
				str2[j] = str[j];
			}
			printf("%s\n+\n",str2);
		}
	}

	// Gera caso 4
	for(i=0; i<tam; i++){
		for(j=0; j<i; j++){
			str2[j] = str[j];
		}		
		str2[j] =  'a';
		for(j=j+1; j<=tam; j++){
			str2[j] = str[j];
		}
		printf("%s\n",str2);
		for(letra = 'b'; letra < str[i]; letra++){
			str2[i] = letra;
			printf("%s\n+\n",str2);
		}
		for(letra = str[i] + 1; letra <= 'z'; letra++){
			str2[i] = letra;
			printf("%s\n+\n",str2);
		}
	}

}
