#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c))) // Usado para calculo da distancia de levenshtein (descobre o menor dos valores)


int levenshtein_pp(char *s1, char *s2) {
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    short int flag = 0;

    if(s1len - s2len >= 2 || s2len - s1len >= 2){
        return 3;
    }

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

int main(){

	printf("%d\n", levenshtein_pp("burdening", "dramatise"));
	return 0;
}


