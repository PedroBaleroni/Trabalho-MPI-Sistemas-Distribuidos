#include "vigenere.h"
#include <stdlib.h>
#include <string.h>

const char alfabeto[] = "abcdefghijklmnopqrstuvwxyz .,!?";

int* textoParaNumero(char* mensagem, int tamMensagem)
{
    int tamAlfabeto = strlen(alfabeto);
    int* valorMensagem = (int*) malloc(sizeof(int) * tamMensagem);
    int caracterAtual;
    for (caracterAtual = 0; caracterAtual < tamMensagem; caracterAtual++) {
        int letraAlfabeto;
        for (letraAlfabeto = 0; letraAlfabeto < tamAlfabeto; letraAlfabeto++) {
            if (alfabeto[letraAlfabeto] == mensagem[caracterAtual]) {
                valorMensagem[caracterAtual] = letraAlfabeto;
                break;
            }
        }
    }
    return valorMensagem;
}

char* numeroParaTexto(int* mensagem, int tamMensagem)
{
    int tamAlfabeto = strlen(alfabeto);
    char* texto = (char*) malloc(sizeof(char) * (tamMensagem + 1));
    int caracterAtual;
    for (caracterAtual = 0; caracterAtual < tamMensagem; caracterAtual++) {
        texto[caracterAtual] = alfabeto[mensagem[caracterAtual]];
    }
    texto[tamMensagem] = '\0';
    return texto;
}

int* decriptar(int* mensagem, int tamMensagem, int* senha, int tamSenha) {
    int tamAlfabeto = strlen(alfabeto);
    int caracterAtual;
    int* msgDecriptada = (int*) malloc(sizeof(int) * tamMensagem);
    for (caracterAtual = 0; caracterAtual < tamMensagem; caracterAtual++) {
        msgDecriptada[caracterAtual] = mensagem[caracterAtual] - senha[caracterAtual % tamSenha];
        if (msgDecriptada[caracterAtual] < 0) {
            msgDecriptada[caracterAtual] += tamAlfabeto;
        }
    }
    return msgDecriptada;
}

int* encriptar(int* mensagem, int tamMensagem, int* senha, int tamSenha) {
    int tamAlfabeto = strlen(alfabeto);
    int caracterAtual;
    int* msgEncriptada = (int*) malloc(sizeof(int) * tamMensagem);
    for (caracterAtual = 0; caracterAtual < tamMensagem; caracterAtual++) {
        msgEncriptada[caracterAtual] = mensagem[caracterAtual] + senha[caracterAtual % tamSenha];
        msgEncriptada[caracterAtual] = msgEncriptada[caracterAtual] % tamAlfabeto;
    }
    return msgEncriptada;
}

int compararMensagem(int* mensagemOriginal, int* mensagemDecifrada, int tamMensagem)
{
    if (memcmp(mensagemDecifrada, mensagemOriginal, sizeof(mensagemDecifrada[0])*tamMensagem) == 0)
        return 1;
    else
        return 0;
}