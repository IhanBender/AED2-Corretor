# AED2-Corretor
Trabalho de AED2. Corretor ortográfico/hash

		* IDENTIFICAÇÃO: (aparentemente resolvido)	
		- Casos 1, 2, 4 Tem distancia 1 na distancia de levenshtein
		- Caso 3 pode ser implementado na analise da mesma matriz. - O valor tera distancia 2, e sera apenas necessario permutar os 			caracteres vizinhos
	
	* ORDENAÇÃO:

	- 1: Podemos criar uma lista encadeada de nodos contendo a string, sendo que a lista se mantem ordenada pela inserção
	- 2: Podemos criar um vetor/lista encadeada com os valores dos indices (ordenados por insercao em relacao à string, nao à chave). Neste
caso, os conflitos atrapalham e teriamos que armazenar um inteiro identificando quantas casas deverao ser percorridas
	- 3: Podemos criar um vetor, colocar os valores sugeridos e ordenar o vetor.
		-3.2: MergeSort - Usa um novo vetor (pode, no pior caso, "triplicar" a memoria utilizada, assim como a rehash)	

	* MEMÓRIA:

	- No caso 1, usaremos memoria suficiente para repetir informaçao existente
	- No caso 2, usaremos apenas dois valores de inteiros para cada palavra (vale a pena para strings com mais de 8 caracteres). unsigned short int pode ser uma opção valida para o segundo valor.
	- Em termos de velocidade, no segundo caso a unica perda é em percorrer a lista encadeada da posicao, que se mostrou nao ser maior que
4 nodos.

	* CONSIDERAÇÕES SOBRE O RESTO DO PROGRAMA:

	- De acordo com o aumento do vetor, a funcao parece perder a efetividade, talvez pelo tamanho maximo assumido por "long int"


NOTAS: 

* levenshtein_pp pode ser otimizado para sair no momento em q nao for possivel ter valor 1

		strcmp() * Implementar (ou achar) funcao de comparacao de strings para poder ordenar

* Implementar funcao sugestoes



