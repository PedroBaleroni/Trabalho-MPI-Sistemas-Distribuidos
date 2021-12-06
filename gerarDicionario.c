#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Use esse programa para gerar o dicionário.
// Passe o número de palavras como argumento de execução
// Sugestão: comece com 2*3*4*5*7 = 840 palavras, para poder
// variar o número de processos entre 2 e 8 sem muitos
// problemas.

char* gerarPalavra()
{
    int tamPalavra = 12;
    char* palavra = (char*) malloc(sizeof(char) * (tamPalavra+1));
    int i;
    for (i = 0; i < tamPalavra; i++) {
        palavra[i] = (rand() % 26) + 97;
    }
    palavra[tamPalavra] = '\0';
    return palavra;
}


int main(int argc, char* argv[])
{
    srand(time(NULL));
    FILE* dc;
    int numPalavras;

    if (argc == 1) {
        printf("Necessário argumento de número de palavras");
        exit(-1);
    }
    else {
        numPalavras = atoi(argv[1]);
        if (numPalavras <= 0) {
            printf("Número de palavras no argumento é invalido.");
            exit(-1);
        }
    }

    dc = fopen("dicionario.txt", "w");
    int contPalavra;
    for (contPalavra = 0; contPalavra < numPalavras; contPalavra++) {
        char* palavra = gerarPalavra();
        fputs(palavra, dc);
        fputs("\n",dc);
        free(palavra);
    }
    fclose(dc);

}