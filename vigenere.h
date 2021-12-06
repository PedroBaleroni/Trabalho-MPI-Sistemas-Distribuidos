#ifndef VIGENERE_H_
#define VIGENERE_H_

int* textoParaNumero(char* mensagem, int tamMensagem);

char* numeroParaTexto(int* mensagem, int tamMensagem);

int* decriptar(int* mensagem, int tamMensagem, int* senha, int tamSenha);

int* encriptar(int* mensagem, int tamMensagem, int* senha, int tamSenha);

int compararMensagem(int* mensagemOriginal, int* mensagemDecifrada, int tamMensagem);

#endif