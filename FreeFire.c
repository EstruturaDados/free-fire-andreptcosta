#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire

// Capacidade máxima da mochila
#define CAPACIDADE 10

// Struct Item: representa um componente na mochila.
typedef struct {
    char nome[50];
    char tipo[30];
    int quantidade;
    int prioridade; // 1 a 5 (1 = baixa, 5 = altíssima)
} Item;

// Enum para critérios de ordenação
typedef enum {
    ORDENAR_POR_NOME = 1,
    ORDENAR_POR_TIPO = 2,
    ORDENAR_POR_PRIORIDADE = 3
} CriterioOrdenacao;

// Vetor estático da mochila e variáveis de controle
static Item mochila[CAPACIDADE];
static int numItens = 0; // quantidade atual
static bool ordenadaPorNome = false; // controla se está ordenada por nome

int main() {
    int escolha;
    do {
        exibirMenu();
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada inválida. Encerrando.\n");
            break;
        }
        limparTela();
        switch (escolha) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 6:
                buscarSequencialPorNome();
                break;
            case 0:
                printf("Saindo... Até a próxima fuga!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n\n");
        }
    } while (escolha != 0);
    return 0;
}

void limparTela() {
    // Imprime várias quebras de linha (portável)
    for (int i = 0; i < 40; i++) {
        putchar('\n');
    }
}

void exibirMenu() {
    printf("================= MOCHILA FREE FIRE =================\n");
    printf("Itens armazenados: %d / %d\n", numItens, CAPACIDADE);
    printf("Ordenada por nome? %s\n", ordenadaPorNome ? "SIM" : "NÃO");
    printf("-----------------------------------------------------\n");
    printf("1 - Inserir item\n");
    printf("2 - Remover item\n");
    printf("3 - Listar itens\n");
    printf("4 - Ordenar itens (nome/tipo/prioridade)\n");
    printf("5 - Busca binária por nome\n");
    printf("6 - Busca sequencial por nome\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

void listarItens() {
    if (numItens == 0) {
        printf("\n[Mochila vazia]\n\n");
        return;
    }
    printf("\n%-3s | %-20s | %-12s | %-10s | %-10s\n", "#", "Nome", "Tipo", "Qtd", "Prioridade");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < numItens; i++) {
        printf("%-3d | %-20s | %-12s | %-10d | %-10d\n", i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("\n");
}

void inserirItem() {
    if (numItens >= CAPACIDADE) {
        printf("\n[Erro] Mochila cheia. Remova algo antes de inserir.\n\n");
        return;
    }
    Item novo;
    printf("\n== Inserir Item ==\n");
    printf("Nome: ");
    scanf(" %49[^\n]", novo.nome);
    printf("Tipo: ");
    scanf(" %29[^\n]", novo.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    printf("Prioridade (1 a 5): ");
    scanf("%d", &novo.prioridade);
    if (novo.prioridade < 1 || novo.prioridade > 5) {
        printf("Prioridade inválida, ajustada para 1.\n");
        novo.prioridade = 1;
    }
    mochila[numItens++] = novo;
    ordenadaPorNome = false; // após inserir, perde ordenação por nome
    printf("Item inserido com sucesso!\n\n");
}

void removerItem() {
    if (numItens == 0) {
        printf("\n[Erro] Mochila vazia, nada para remover.\n\n");
        return;
    }
    char alvo[50];
    printf("\n== Remover Item ==\nNome do item a remover: ");
    scanf(" %49[^\n]", alvo);
    int indice = -1;
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, alvo) == 0) {
            indice = i;
            break;
        }
    }
    if (indice == -1) {
        printf("Item '%s' não encontrado.\n\n", alvo);
        return;
    }
    for (int j = indice; j < numItens - 1; j++) {
        mochila[j] = mochila[j + 1];
    }
    numItens--;
    ordenadaPorNome = false; // estrutura mudou
    printf("Item removido com sucesso.\n\n");
}

void buscarSequencialPorNome() {
    if (numItens == 0) {
        printf("\n[Mochila vazia]\n\n");
        return;
    }
    char alvo[50];
    printf("\n== Busca Sequencial ==\nNome do item: ");
    scanf(" %49[^\n]", alvo);
    bool encontrado = false;
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, alvo) == 0) {
            printf("\nItem encontrado:\n");
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\nPrioridade: %d\n\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
            encontrado = true;
            break; // para primeiro encontrado
        }
    }
    if (!encontrado) {
        printf("Item '%s' não está na mochila.\n\n", alvo);
    }
}

void insertionSort(CriterioOrdenacao criterio, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;
        // Escolhe comparação baseada no critério
        while (j >= 0) {
            bool deveMover = false;
            (*comparacoes)++; // conta tentativa de comparação
            if (criterio == ORDENAR_POR_NOME) {
                if (strcmp(mochila[j].nome, chave.nome) > 0) deveMover = true;
            } else if (criterio == ORDENAR_POR_TIPO) {
                if (strcmp(mochila[j].tipo, chave.tipo) > 0) deveMover = true;
            } else if (criterio == ORDENAR_POR_PRIORIDADE) {
                // prioridade maior vem primeiro
                if (mochila[j].prioridade < chave.prioridade) deveMover = true;
            }
            if (!deveMover) break;
            mochila[j + 1] = mochila[j];
            j--;
        }
        mochila[j + 1] = chave;
    }
    if (criterio == ORDENAR_POR_NOME) {
        ordenadaPorNome = true;
    } else {
        ordenadaPorNome = false; // só consideramos válida quando ordenada explicitamente por nome
    }
}

void menuDeOrdenacao() {
    if (numItens == 0) {
        printf("Nada para ordenar.\n\n");
        return;
    }
    printf("\n== Ordenar Itens ==\n");
    printf("1 - Por Nome (A-Z)\n");
    printf("2 - Por Tipo (A-Z)\n");
    printf("3 - Por Prioridade (5->1)\n");
    printf("Escolha: ");
    int op; scanf("%d", &op);
    int comparacoes = 0;
    switch (op) {
        case 1:
            insertionSort(ORDENAR_POR_NOME, &comparacoes);
            printf("Ordenado por nome. Comparações: %d\n\n", comparacoes);
            break;
        case 2:
            insertionSort(ORDENAR_POR_TIPO, &comparacoes);
            printf("Ordenado por tipo. Comparações: %d\n\n", comparacoes);
            break;
        case 3:
            insertionSort(ORDENAR_POR_PRIORIDADE, &comparacoes);
            printf("Ordenado por prioridade (desc). Comparações: %d\n\n", comparacoes);
            break;
        default:
            printf("Opção inválida.\n\n");
    }
}

void buscaBinariaPorNome() {
    if (numItens == 0) {
        printf("Mochila vazia!");
        return;
    }
    if (!ordenadaPorNome) {
        printf("A mochila não está ordenada por nome. Ordene primeiro (opção 4 -> por nome).\n\n");
        return;
    }
    char alvo[50];
    printf("\n== Nome do item: ");
    scanf(" %49[^\n]", alvo);
    int inicio = 0, fim = numItens - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(mochila[meio].nome, alvo);
        if (cmp == 0) {
            printf("\nItem encontrado (posição %d):\n", meio + 1);
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\nPrioridade: %d\n\n", mochila[meio].nome, mochila[meio].tipo, mochila[meio].quantidade, mochila[meio].prioridade);
            return;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    printf("Item '%s' não encontrado na busca.\n\n", alvo);
}