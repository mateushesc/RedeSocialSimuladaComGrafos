#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_USUARIOS 100
#define MAX_FILA 100

typedef struct NoAmigo{
    int idAmigo;
    struct NoAmigo* prox;
} NoAmigo;

typedef struct Usuario {
    int id;
    char nome[MAX_NOME];
    NoAmigo* listaAmigos;
    int visitado;
} Usuario;

typedef struct Grafo {
    Usuario usuarios[MAX_USUARIOS];
    int totalUsuarios;
} Grafo;

// Estrutura simples de fila para BFS
typedef struct {
    int itens[MAX_FILA];
    int frente;
    int tras;
} Fila;

// Função para adicionar um novo usuário ao grafo
void adicionarUsuario(Grafo* grafo, int id, const char* nome) {
    // Verifica se há espaço no vetor de usuários
    if (grafo->totalUsuarios >= MAX_USUARIOS) {
        printf("Erro: número máximo de usuários atingido.\n");
        return;
    }

    // Verifica se o ID já está em uso
    for (int i = 0; i < grafo->totalUsuarios; i++) {
        if (grafo->usuarios[i].id == id) {
            printf("Erro: ID de usuário já existente.\n");
            return;
        }
    }

    // 2.1 - Referência ao próximo espaço disponível
    Usuario* novo = &grafo->usuarios[grafo->totalUsuarios];

    // 2.2 - Inicialização dos dados
    novo->id = id;
    strncpy(novo->nome, nome, MAX_NOME);
    novo->listaAmigos = NULL;
    novo->visitado = 0;

    // 2.3 - Incrementar total de usuários
    grafo->totalUsuarios++;

    printf("Usuário '%s' (ID %d) adicionado com sucesso!\n", nome, id);
}

// Função auxiliar para adicionar um amigo à lista de adjacência de um usuário
void adicionarNaListaDeAmigos(Usuario* usuario, int idAmigo) {
    NoAmigo* novoAmigo = (NoAmigo*)malloc(sizeof(NoAmigo));
    if (novoAmigo == NULL) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    novoAmigo->idAmigo = idAmigo;
    novoAmigo->prox = usuario->listaAmigos;
    usuario->listaAmigos = novoAmigo;
}

// Função para encontrar índice de usuário no vetor (pelo ID)
int encontrarIndiceUsuario(Grafo* grafo, int id) {
    for (int i = 0; i < grafo->totalUsuarios; i++) {
        if (grafo->usuarios[i].id == id) {
            return i;
        }
    }
    return -1; // Não encontrado
}

// 3. Função para criar uma conexão (amizade) entre dois usuários
void criarConexao(Grafo* grafo, int id1, int id2) {
    if (id1 == id2) {
        printf("Erro: um usuário não pode ser amigo de si mesmo.\n");
        return;
    }

    int indice1 = encontrarIndiceUsuario(grafo, id1);
    int indice2 = encontrarIndiceUsuario(grafo, id2);

    // 3.1 - Verificar se ambos os usuários existem
    if (indice1 == -1 || indice2 == -1) {
        printf("Erro: um ou ambos os usuários não foram encontrados.\n");
        return;
    }

    Usuario* usuario1 = &grafo->usuarios[indice1];
    Usuario* usuario2 = &grafo->usuarios[indice2];

    // 3.2 - Adicionar um ao outro nas respectivas listas de amigos
    adicionarNaListaDeAmigos(usuario1, id2);
    adicionarNaListaDeAmigos(usuario2, id1);

    // 3.3 - Conexão criada com sucesso
    printf("Conexão criada entre '%s' (ID %d) e '%s' (ID %d).\n",
           usuario1->nome, id1, usuario2->nome, id2);
}

// 4. Função para exibir os amigos de um usuário
void visualizarAmigos(Grafo* grafo, int idUsuario) {
    int indice = encontrarIndiceUsuario(grafo, idUsuario);

    // 4.1 - Verificar se o usuário existe
    if (indice == -1) {
        printf("Usuário com ID %d não encontrado.\n", idUsuario);
        return;
    }

    Usuario* usuario = &grafo->usuarios[indice];
    NoAmigo* atual = usuario->listaAmigos;

    printf("Amigos de '%s' (ID %d):\n", usuario->nome, usuario->id);

    // 4.2 - Percorrer a lista de amigos e exibir nomes/IDs
    if (atual == NULL) {
        printf("  Nenhum amigo encontrado.\n");
        return;
    }

    while (atual != NULL) {
        int idAmigo = atual->idAmigo;
        int indiceAmigo = encontrarIndiceUsuario(grafo, idAmigo);
        if (indiceAmigo != -1) {
            Usuario* amigo = &grafo->usuarios[indiceAmigo];
            printf("  - %s (ID %d)\n", amigo->nome, amigo->id);
        }
        atual = atual->prox;
    }
}

// Inicializa a fila
void inicializarFila(Fila* f) {
    f->frente = 0;
    f->tras = 0;
}

// Verifica se a fila está vazia
int filaVazia(Fila* f) {
    return f->frente == f->tras;
}

// Enfileira um elemento
void enfileirar(Fila* f, int valor) {
    if (f->tras < MAX_FILA) {
        f->itens[f->tras++] = valor;
    }
}

// Desenfileira um elemento
int desenfileirar(Fila* f) {
    if (!filaVazia(f)) {
        return f->itens[f->frente++];
    }
    return -1;
}

// 5. Função de busca em largura (BFS)
void bfs(Grafo* grafo, int idInicial) {
    int indiceInicial = encontrarIndiceUsuario(grafo, idInicial);

    if (indiceInicial == -1) {
        printf("Usuário com ID %d não encontrado.\n", idInicial);
        return;
    }

    // 5.1 - Inicializa fila e visitados
    Fila fila;
    inicializarFila(&fila);

    for (int i = 0; i < grafo->totalUsuarios; i++) {
        grafo->usuarios[i].visitado = 0; // limpa marcações anteriores
    }

    // 5.2 - Marca o usuário inicial como visitado e enfileira
    grafo->usuarios[indiceInicial].visitado = 1;
    enfileirar(&fila, idInicial);

    printf("BFS a partir de '%s' (ID %d):\n", grafo->usuarios[indiceInicial].nome, idInicial);

    // 5.3 - Laço principal do BFS
    while (!filaVazia(&fila)) {
        int idAtual = desenfileirar(&fila);
        int indiceAtual = encontrarIndiceUsuario(grafo, idAtual);
        Usuario* atual = &grafo->usuarios[indiceAtual];

        printf("Visitando: %s (ID %d)\n", atual->nome, atual->id);

        NoAmigo* amigo = atual->listaAmigos;
        while (amigo != NULL) {
            int idAmigo = amigo->idAmigo;
            int indiceAmigo = encontrarIndiceUsuario(grafo, idAmigo);

            if (indiceAmigo != -1 && grafo->usuarios[indiceAmigo].visitado == 0) {
                grafo->usuarios[indiceAmigo].visitado = 1;
                enfileirar(&fila, idAmigo);
            }

            amigo = amigo->prox;
        }
    }
}

// Função recursiva de DFS
void dfsVisita(Grafo* grafo, int idUsuario) {
    int indice = encontrarIndiceUsuario(grafo, idUsuario);
    if (indice == -1) return;

    Usuario* usuario = &grafo->usuarios[indice];

    // 6.2 - Marcar como visitado
    usuario->visitado = 1;
    printf("Visitando: %s (ID %d)\n", usuario->nome, usuario->id);

    // 6.3 - Para cada amigo não visitado, chamar recursivamente
    NoAmigo* amigo = usuario->listaAmigos;
    while (amigo != NULL) {
        int idAmigo = amigo->idAmigo;
        int indiceAmigo = encontrarIndiceUsuario(grafo, idAmigo);

        if (indiceAmigo != -1 && grafo->usuarios[indiceAmigo].visitado == 0) {
            dfsVisita(grafo, idAmigo);
        }

        amigo = amigo->prox;
    }
}

// 6.1 - Função principal de DFS
void dfs(Grafo* grafo, int idInicial) {
    int indiceInicial = encontrarIndiceUsuario(grafo, idInicial);
    if (indiceInicial == -1) {
        printf("Usuário com ID %d não encontrado.\n", idInicial);
        return;
    }

    // Limpa os marcadores de visita
    for (int i = 0; i < grafo->totalUsuarios; i++) {
        grafo->usuarios[i].visitado = 0;
    }

    printf("DFS a partir de '%s' (ID %d):\n", grafo->usuarios[indiceInicial].nome, idInicial);
    dfsVisita(grafo, idInicial);
}

#define INFINITO 9999

// 7. Função para sugerir amigos com base em BFS
void sugerirAmigos(Grafo* grafo, int idUsuario) {
    int indiceInicial = encontrarIndiceUsuario(grafo, idUsuario);
    if (indiceInicial == -1) {
        printf("Usuário com ID %d não encontrado.\n", idUsuario);
        return;
    }

    // Vetor de níveis de distância
    int distancia[MAX_USUARIOS];
    for (int i = 0; i < grafo->totalUsuarios; i++) {
        distancia[i] = INFINITO;
        grafo->usuarios[i].visitado = 0;
    }

    Fila fila;
    inicializarFila(&fila);

    // 7.1 - Início do BFS
    distancia[indiceInicial] = 0;
    grafo->usuarios[indiceInicial].visitado = 1;
    enfileirar(&fila, idUsuario);

    while (!filaVazia(&fila)) {
        int idAtual = desenfileirar(&fila);
        int indiceAtual = encontrarIndiceUsuario(grafo, idAtual);
        Usuario* atual = &grafo->usuarios[indiceAtual];

        NoAmigo* amigo = atual->listaAmigos;
        while (amigo != NULL) {
            int idAmigo = amigo->idAmigo;
            int indiceAmigo = encontrarIndiceUsuario(grafo, idAmigo);
            if (indiceAmigo != -1 && grafo->usuarios[indiceAmigo].visitado == 0) {
                grafo->usuarios[indiceAmigo].visitado = 1;
                distancia[indiceAmigo] = distancia[indiceAtual] + 1;
                enfileirar(&fila, idAmigo);
            }
            amigo = amigo->prox;
        }
    }

    // 7.2 e 7.3 - Verificar usuários a distância 2 (amigos de amigos que não são amigos diretos)
    printf("Sugestões de amizade para '%s' (ID %d):\n", grafo->usuarios[indiceInicial].nome, idUsuario);
    int encontrou = 0;

    for (int i = 0; i < grafo->totalUsuarios; i++) {
        if (i != indiceInicial && distancia[i] == 2) {
            // Verifica se já é amigo direto
            int ehAmigoDireto = 0;
            NoAmigo* amigo = grafo->usuarios[indiceInicial].listaAmigos;
            while (amigo != NULL) {
                if (amigo->idAmigo == grafo->usuarios[i].id) {
                    ehAmigoDireto = 1;
                    break;
                }
                amigo = amigo->prox;
            }

            // 7.4 - Se não é amigo direto, sugerir
            if (!ehAmigoDireto) {
                printf("  - %s (ID %d)\n", grafo->usuarios[i].nome, grafo->usuarios[i].id);
                encontrou = 1;
            }
        }
    }

    if (!encontrou) {
        printf("  Nenhuma sugestão no momento.\n");
    }
}

int main() {
    Grafo grafo;
    grafo.totalUsuarios = 0;

    int opcao;
    do {
        printf("\n==== MENU ====\n");
        printf("1. Adicionar usuário\n");
        printf("2. Adicionar conexão (amizade)\n");
        printf("3. Ver amigos de um usuário\n");
        printf("4. Rodar BFS\n");
        printf("5. Rodar DFS\n");
        printf("6. Sugerir amigos\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            int id;
            char nome[50];
            printf("ID do usuário: ");
            scanf("%d", &id);
            printf("Nome do usuário: ");
            scanf(" %[^\n]", nome); // lê nome com espaços
            adicionarUsuario(&grafo, id, nome);
        }

        else if (opcao == 2) {
            int id1, id2;
            printf("ID do primeiro usuário: ");
            scanf("%d", &id1);
            printf("ID do segundo usuário: ");
            scanf("%d", &id2);
            criarConexao(&grafo, id1, id2);
        }

        else if (opcao == 3) {
            int id;
            printf("ID do usuário: ");
            scanf("%d", &id);
            visualizarAmigos(&grafo, id);
        }

        else if (opcao == 4) {
            int id;
            printf("ID do usuário para BFS: ");
            scanf("%d", &id);
            bfs(&grafo, id);
        }

        else if (opcao == 5) {
            int id;
            printf("ID do usuário para DFS: ");
            scanf("%d", &id);
            dfs(&grafo, id);
        }

        else if (opcao == 6) {
            int id;
            printf("ID do usuário para sugestão de amigos: ");
            scanf("%d", &id);
            sugerirAmigos(&grafo, id);
        }

        else if (opcao == 7) {
            printf("Saindo...\n");
        }

        else {
            printf("Opção inválida.\n");
        }

    } while (opcao != 7);

    return 0;
}
