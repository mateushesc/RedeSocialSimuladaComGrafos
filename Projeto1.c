#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_USUARIOS 100

typedef struct Usuario {
    int id;
    char nome[MAX_NOME];
    NoAmigo* listaamigos;
} Usuario;

typedef struct NoAmigo {
    int idAmigo;
    struct NoAmigo* prox;
} NoAmigo;

typedef struct Grafo {
    Usuario usuarios[MAX_USUARIOS];
    int totalUsuarios;
} Grafo;

void adicionarUsuario(Grafo* grafo, int idm, const char* nome) {
    if (grafo->totalUsuarios >= MAX_USUARIOS) {
        printf("Número máximo de usuários atingido.\n");\
        return;
    }

    for (int i = 0 ; i < grafo-> totalUsuarios)
}


