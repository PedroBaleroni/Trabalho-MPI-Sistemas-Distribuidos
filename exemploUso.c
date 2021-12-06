#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi/mpi.h>
#include "vigenere.h"

// Tamanho das palavras de senha
#define tamSenha 12

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);
    
    // Estrutura do MPI para leitura de arquivo
    MPI_File dicionario;
    MPI_File_open(MPI_COMM_WORLD, "dicionario.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &dicionario);

    // Mensagem em texto aberto, e sua versão cifrada.
    // A versão em texto aberto serve apenas para testar se a chave deu certo
    char mensagem[] = "agora vai";
    int tamMensagem = strlen(mensagem);
    char mensagemCifrada[] = "rphubamdk";

    // Usado para determinar o total de palavras no dicionário
    // Pode ser usado para determinar o espaço de dados de cada processo
    MPI_Offset tamDicionario;
    MPI_File_get_size(dicionario, &tamDicionario);
    int qtdPalavras = tamDicionario/(tamSenha + 1);
    
    // Espaço de memória para armazenar a senha lida do arquivo
    char *senha = (char*) malloc(sizeof(char)*(tamSenha + 1));

    // Todo o algoritmo de cifragem trabalha com o valor numérico do texto
    // A mensagem original e a cifrada precisam ser convertidas apenas uma
    // vez, enquanto as versões de teste precisam ser convertidas sempre
    int* mensagemNumerica = textoParaNumero(mensagem, tamMensagem);
    int* mensagemCifradaNumerica = textoParaNumero(mensagemCifrada, tamMensagem);

    // Essa parte permite pular para uma palavra qualquer no arquivo. O salto
    // é feito em relação à posição 0. No exemplo, desejo ler a palavra na linha
    // 81 (supondo que a primeira linha é zero).
    // Cada linha possui uma senha e uma quebra de linha
    int linhaDesejada = 9999;
    linhaDesejada--;
    MPI_File_seek(dicionario, linhaDesejada*(tamSenha + 1), MPI_SEEK_SET);

    // Lê uma senha e a quebra de linha, por isso o +1 na leitura
    // Depois converte o último caractere para fim de string
    MPI_Status s;
    MPI_File_read(dicionario, senha, tamSenha+1, MPI_CHAR, &s);
    senha[tamSenha] = '\0';
    
    // Converte a nova senha para número, e tenta decriptar a mensagem com
    // a senha lida do arquivo
    int* senhaNumerica = textoParaNumero(senha, tamSenha);
    int* mensagemDecriptada = decriptar(mensagemCifradaNumerica, tamMensagem, 
                                        senhaNumerica, tamSenha);

    // Compara a mensagem decifrada com a mensagem em texto puro, para saber
    // se deu certo
    if(compararMensagem(mensagemNumerica, mensagemDecriptada, tamMensagem))
        printf("Senha encontrada! %s\n", senha);
    else
        printf("A senha %s não é a correta.\n", senha);

    // Lembre-se de liberar as variáveis de teste a cada fim de tentativa
    free(senhaNumerica);
    free(mensagemDecriptada);

    // Ao terminar os testes, feche o arquivo 
    MPI_File_close(&dicionario);

    MPI_Finalize();

    // E apague as variáveis criadas dinamicamente
    free(mensagemNumerica);
    free(mensagemCifradaNumerica);

}