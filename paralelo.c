#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi/mpi.h>
#include "vigenere.h"

// Tamanho das palavras de senha
#define tamSenha 12


int encontraLinha(int* vetorParcial, int tamanhoParcial, char* mensagem, char* mensagemCifrada ){
    MPI_File dicionario;
    MPI_File_open(MPI_COMM_WORLD, "dicionario.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &dicionario);

    int i;
    int linhaDesejada;
    
    int tamMensagem = strlen(mensagem);

    int* senhaNumerica;
    int* mensagemDecriptada;

    int* mensagemNumerica = textoParaNumero(mensagem, tamMensagem);
    int* mensagemCifradaNumerica = textoParaNumero(mensagemCifrada, tamMensagem);

    char *senha = (char*) malloc(sizeof(char)*(tamSenha + 1));

    for(i=0;i<tamanhoParcial;i++)
    {
        linhaDesejada = vetorParcial[i];
        MPI_File_seek(dicionario, linhaDesejada*(tamSenha + 1), MPI_SEEK_SET);

        // Lê uma senha e a quebra de linha, por isso o +1 na leitura
        // Depois converte o último caractere para fim de string
        MPI_Status s;
        MPI_File_read(dicionario, senha, tamSenha+1, MPI_CHAR, &s);
        senha[tamSenha] = '\0';
        
        // Converte a nova senha para número, e tenta decriptar a mensagem com
        // a senha lida do arquivo
        senhaNumerica = textoParaNumero(senha, tamSenha);
        mensagemDecriptada = decriptar(mensagemCifradaNumerica, tamMensagem, 
                                            senhaNumerica, tamSenha);

        // Compara a mensagem decifrada com a mensagem em texto puro, para saber
        // se deu certo
        if(compararMensagem(mensagemNumerica, mensagemDecriptada, tamMensagem))
        {
            printf("Senha encontrada na linha %d! %s\n",vetorParcial[i], senha);
            return vetorParcial[i];
        }
            
    }
    return -1;
    free(senhaNumerica);
    free(mensagemDecriptada);
    free(mensagemNumerica);
    free(mensagemCifradaNumerica);
}

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);
    
    // Estrutura do MPI para leitura de arquivo
    MPI_File dicionario;
    MPI_File_open(MPI_COMM_WORLD, "dicionario.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &dicionario);

    // Mensagem em texto aberto, e sua versão cifrada.
    // A versão em texto aberto serve apenas para testar se a chave deu certo
    char mensagem[] = "agora vai";
    char mensagemCifrada[] = "rphubamdk";


    // Usado para determinar o total de palavras no dicionário
    // Pode ser usado para determinar o espaço de dados de cada processo
    MPI_Offset tamDicionario;
    MPI_File_get_size(dicionario, &tamDicionario);
    int qtdPalavras = tamDicionario/(tamSenha + 1);
    int vetorTotal[qtdPalavras];
    // Espaço de memória para armazenar a senha lida do arquivo
    

    // Todo o algoritmo de cifragem trabalha com o valor numérico do texto
    // A mensagem original e a cifrada precisam ser convertidas apenas uma
    // vez, enquanto as versões de teste precisam ser convertidas sempre
    

    // Essa parte permite pular para uma palavra qualquer no arquivo. O salto
    // é feito em relação à posição 0. No exemplo, desejo ler a palavra na linha
    // 81 (supondo que a primeira linha é zero).
    // Cada linha possui uma senha e uma quebra de linha
    int i;
    int tamanhoParcial;
    int numProcesso, qtdProcessos;

    MPI_Comm_size(MPI_COMM_WORLD, &qtdProcessos);
    MPI_Comm_rank(MPI_COMM_WORLD, &numProcesso);

    if(numProcesso == 0)
    {
        for(i=0; i<qtdPalavras; i++)
        {
            vetorTotal[i] = i;
            tamanhoParcial = qtdPalavras/qtdProcessos;
        }
           
    }

    MPI_Bcast(&tamanhoParcial, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int vetorParcial[tamanhoParcial];

    MPI_Scatter(vetorTotal, tamanhoParcial, MPI_INT, vetorParcial, tamanhoParcial, MPI_INT, 0, MPI_COMM_WORLD);

    int linhaEncontradaParcial = encontraLinha(vetorParcial,tamanhoParcial, mensagem, mensagemCifrada);
    int linhaEncontradaFinal;

    MPI_Reduce(&linhaEncontradaParcial,&linhaEncontradaFinal, 1, MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);

    if(numProcesso == 0)
    {
        linhaEncontradaFinal += qtdProcessos;
        linhaEncontradaFinal--;
        if(linhaEncontradaFinal <= 0){
            printf("Senha Não Encontrada\n");
        }
    }    

    // Ao terminar os testes, feche o arquivo 
    MPI_File_close(&dicionario);

    MPI_Finalize();

    // E apague as variáveis criadas dinamicamente
  

}
