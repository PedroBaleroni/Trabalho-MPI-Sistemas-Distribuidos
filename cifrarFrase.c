#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vigenere.h"

// Use este código para cifrar uma frase qualquer, escolhendo uma senha
// que esteja no dicionário. Com a frase cifrada, altere o arquivo do
// trabalho para fazer os testes.

int main() {

    char* mensagem = "agora vai";
    int tamMensagem = strlen(mensagem);

    char* senha = "rjydbfwdcjid";
    int tamSenha = strlen(senha);
        
    int* mensagemNumerica = textoParaNumero(mensagem, tamMensagem);
    int* senhaNumerica = textoParaNumero(senha, tamSenha);

    int* mensagemEncriptada = encriptar(mensagemNumerica, tamMensagem, 
                                        senhaNumerica, tamSenha);

    char* mensagemCifrada = numeroParaTexto(mensagemEncriptada, tamMensagem);
    printf("Mensagem original: %s\nMensagem cifrada: %s\nTamanho da mensagem: %u\n", mensagem, mensagemCifrada, tamMensagem);

}