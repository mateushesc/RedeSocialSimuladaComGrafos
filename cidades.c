#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CIDADES 100
#define TAM_NOME 50
#define INFINITO 1000000

typedef struct Rota {
    int destino;            
    int custo;               
    struct Rota* prox;      
} Rota;


typedef struct {
    char nome[TAM_NOME];     
    Rota* rotas;            
} Cidade;


typedef struct {
    Cidade cidades[MAX_CIDADES];
    int totalCidades;            
} Mapa;

// Função para buscar índice pelo nome
int buscarIndiceCidade(Mapa* mapa, const char* nome) {
    for (int i = 0; i < mapa->totalCidades; i++) {
        if (strcmp(mapa->cidades[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;  // Cidade não encontrada
}

// Adiciona cidade
void adicionarCidade(Mapa* mapa, const char* nome) {
    if (mapa->totalCidades >= MAX_CIDADES) {
        printf("Limite máximo de cidades atingido.\n");
        return;
    }

    // Verifica se a cidade já existe
    if (buscarIndiceCidade(mapa, nome) != -1) {
        printf("Cidade '%s' já cadastrada.\n", nome);
        return;
    }

    // Adiciona a nova cidade
    strcpy(mapa->cidades[mapa->totalCidades].nome, nome);
    mapa->cidades[mapa->totalCidades].rotas = NULL;
    mapa->totalCidades++;

    printf("Cidade '%s' adicionada com sucesso.\n", nome);
}

// Função auxiliar para adicionar rota na lista de adjacência de uma cidade
void inserirRota(Cidade* origem, int destinoIndice, int custo) {
    Rota* novaRota = (Rota*) malloc(sizeof(Rota));
    novaRota->destino = destinoIndice;
    novaRota->custo = custo;
    novaRota->prox = origem->rotas;
    origem->rotas = novaRota;
}

// Função para cadastrar rota entre duas cidades
void cadastrarRota(Mapa* mapa, const char* nomeOrigem, const char* nomeDestino, int custo) {
    int iOrigem = buscarIndiceCidade(mapa, nomeOrigem);
    int iDestino = buscarIndiceCidade(mapa, nomeDestino);

    if (iOrigem == -1 || iDestino == -1) {
        printf("Erro: uma ou ambas as cidades não existem.\n");
        return;
    }

    // Adiciona rota da origem para destino
    inserirRota(&mapa->cidades[iOrigem], iDestino, custo);

    // Adiciona rota da destino para origem (ambas direções)
    inserirRota(&mapa->cidades[iDestino], iOrigem, custo);

    printf("Rota entre '%s' e '%s' adicionada com sucesso.\n", nomeOrigem, nomeDestino);
}

void exibirCidades(Mapa* mapa) {
    printf("\nCidades cadastradas:\n");
    if (mapa->totalCidades == 0) {
        printf("Nenhuma cidade cadastrada.\n");
        return;
    }

    for (int i = 0; i < mapa->totalCidades; i++) {
        printf(" - %s\n", mapa->cidades[i].nome);
    }
}

void exibirRotas(Mapa* mapa) {
    char nome[TAM_NOME];
    printf("Digite o nome da cidade para ver suas rotas: ");
    scanf(" %[^\n]", nome);

    int indice = buscarIndiceCidade(mapa, nome);
    if (indice == -1) {
        printf("Cidade não encontrada.\n");
        return;
    }

    Cidade* cidade = &mapa->cidades[indice];
    Rota* rota = cidade->rotas;

    printf("\nRotas a partir de '%s':\n", cidade->nome);

    if (rota == NULL) {
        printf("Nenhuma rota cadastrada para esta cidade.\n");
        return;
    }

    while (rota != NULL) {
        printf(" -> %s (Custo: %d)\n", mapa->cidades[rota->destino].nome, rota->custo);
        rota = rota->prox;
    }
}

void dijkstra(Mapa* mapa, const char* nomeOrigem, const char* nomeDestino) {
    int origem = buscarIndiceCidade(mapa, nomeOrigem);
    int destino = buscarIndiceCidade(mapa, nomeDestino);

    if (origem == -1 || destino == -1) {
        printf("Cidade de origem ou destino não encontrada.\n");
        return;
    }

    int dist[MAX_CIDADES];
    int visitado[MAX_CIDADES];
    int predecessores[MAX_CIDADES];

    for (int i = 0; i < mapa->totalCidades; i++) {
        dist[i] = INFINITO;
        visitado[i] = 0;
        predecessores[i] = -1;
    }

    dist[origem] = 0;

    for (int count = 0; count < mapa->totalCidades - 1; count++) {
        int u = -1;
        int menorDist = INFINITO;

        // Encontrar o vértice com a menor distância ainda não visitado
        for (int i = 0; i < mapa->totalCidades; i++) {
            if (!visitado[i] && dist[i] < menorDist) {
                menorDist = dist[i];
                u = i;
            }
        }

        if (u == -1) break;  // Nenhum vértice alcançável restante

        visitado[u] = 1;

        // Atualizar distâncias vizinhos
        Rota* rota = mapa->cidades[u].rotas;
        while (rota != NULL) {
            int v = rota->destino;
            if (!visitado[v] && dist[u] + rota->custo < dist[v]) {
                dist[v] = dist[u] + rota->custo;
                predecessores[v] = u;
            }
            rota = rota->prox;
        }
    }

    // Verificar se o destino foi alcançado
    if (dist[destino] == INFINITO) {
        printf("Não existe caminho entre '%s' e '%s'.\n", nomeOrigem, nomeDestino);
        return;
    }

    // Reconstruir o caminho
    int caminho[MAX_CIDADES];
    int tamanho = 0;
    for (int v = destino; v != -1; v = predecessores[v]) {
        caminho[tamanho++] = v;
    }

    printf("\nMenor caminho de '%s' até '%s':\n", nomeOrigem, nomeDestino);
    for (int i = tamanho - 1; i >= 0; i--) {
        printf("%s", mapa->cidades[caminho[i]].nome);
        if (i > 0) printf(" -> ");
    }

    printf("\nCusto total: %d\n", dist[destino]);
}

void menu(Mapa* mapa) {
    int opcao;
    char origem[TAM_NOME], destino[TAM_NOME];
    char nome[TAM_NOME];
    int custo;

    do {
        printf("\nMENU\n");
        printf("1. Cadastrar cidade\n");
        printf("2. Cadastrar rota\n");
        printf("3. Visualizar cidades\n");
        printf("4. Visualizar rotas de uma cidade\n");
        printf("5. Calcular menor caminho (Dijkstra)\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                printf("Digite o nome da cidade: ");
                scanf(" %[^\n]", nome);
                adicionarCidade(mapa, nome);
                break;
            case 2:
                printf("Digite o nome da cidade de origem: ");
                scanf(" %[^\n]", origem);
                printf("Digite o nome da cidade de destino: ");
                scanf(" %[^\n]", destino);
                printf("Digite o custo da rota: ");
                scanf("%d", &custo);
                cadastrarRota(mapa, origem, destino, custo);
                break;
            case 3:
                exibirCidades(mapa);
                break;
            case 4:
                exibirRotas(mapa);
                break;
            case 5:
                printf("Cidade de origem: ");
                scanf(" %[^\n]", origem);
                printf("Cidade de destino: ");
                scanf(" %[^\n]", destino);
                dijkstra(mapa, origem, destino);
                break;
            case 6:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while(opcao != 6);
}

int main() {
    Mapa mapa;
    mapa.totalCidades = 0;

    // Cadastro de cidades 
    adicionarCidade(&mapa, "A");
    adicionarCidade(&mapa, "B");
    adicionarCidade(&mapa, "C");
    adicionarCidade(&mapa, "D");
    adicionarCidade(&mapa, "E");

    // Cadastro de rotas 
    cadastrarRota(&mapa, "A", "B", 4);
    cadastrarRota(&mapa, "A", "C", 2);
    cadastrarRota(&mapa, "B", "C", 1);
    cadastrarRota(&mapa, "B", "D", 5);
    cadastrarRota(&mapa, "C", "D", 8);
    cadastrarRota(&mapa, "C", "E", 10);
    cadastrarRota(&mapa, "D", "E", 2);

    // Teste do menor caminho
    dijkstra(&mapa, "A", "E");

    menu(&mapa);

    return 0;
}