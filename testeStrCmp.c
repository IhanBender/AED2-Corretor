#include <stdio.h>
#include <string.h>

int compareString(char *s1, char *s2){
	
	int i, j, flag = 0;
	unsigned int s1len = strlen(s1);
	unsigned int s2len = strlen(s2);
	int diff = s1len - s2len;

	if(diff >= 2 || diff <= -2){
        return 2;
    }

    if (diff == 0){ // Sem preocupacoes sobre tamanhos diferentes
        for (i = 0; i < s1len; i++){
            if (s1[i] != s2[i]){
                if (s1[i+1] == s2[i] && s1[i] == s2[i+1]){
                    i++;
                }
                flag++;
            }

            if (flag > 1){
                return 2;
            }
        }
    }
    else{   // Sabemos que so podemos aceitar sugestoes justificadas pelo tamanho
        if (diff < 0) diff = s2len; // Define o tamanho da maior string
        else diff = s1len;

        for (i = 0, j = 0; i < diff; j++, i++){
            if(s1[i] != s2[j]){
                if (diff == s2len){ // Caso a segunda seja maior
                    if (s1[i] == s2[j+1]){
                        j++;
                    }
                    else{
                        return 2;
                    }
                }
                else{
                    if (s2[j] == s1[i+1]){
                        i++;
                    }
                    else{
                        return 2;
                    }
                }

                flag++;
            }
        }

    }


   	return flag;
}


int main(int argc, char const *argv[])
{
	char s1[] = "iharoro";
	char s2[] = "ihaoror";

	printf("%d\n", compareString(s1, s2));

	return 0;
}
