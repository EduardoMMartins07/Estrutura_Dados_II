// Eduardo Moisés Martins
#include <stdio.h>
#include <stdlib.h>

// Definição das cores para os nós da árvore Red-Black
#define VERMELHO 0
#define PRETO 1

// Estrutura de um nó da árvore Red-Black
typedef struct No {
    int valor;               // Valor armazenado no nó
    int cor;                 // Cor do nó (VERMELHO ou PRETO)
    struct No *esq, *dir, *pai; // Ponteiros para o nó esquerdo, direito e o pai
} No;

// Função para criar um novo nó com um valor
No* criarNo(int valor) {
    No *novo = (No*)malloc(sizeof(No));
    novo->valor = valor;
    novo->cor = VERMELHO;    // Novo nó sempre começa como vermelho
    novo->esq = novo->dir = novo->pai = NULL;
    return novo;
}

// Função para realizar a rotação à esquerda em torno do nó x
void rotacaoEsquerda(No **raiz, No *x) {
    No *y = x->dir; // y será o filho direito de x
    x->dir = y->esq; // y->esq agora se torna filho direito de x

    if (y->esq != NULL) {
        y->esq->pai = x; // Se y->esq existe, atualiza o pai
    }

    y->pai = x->pai; // Atualiza o pai de y

    if (x->pai == NULL) {
        *raiz = y; // Se x era a raiz, agora y é a nova raiz
    } else if (x == x->pai->esq) {
        x->pai->esq = y; // Se x era filho esquerdo, y toma o lugar de x
    } else {
        x->pai->dir = y; // Se x era filho direito, y toma o lugar de x
    }

    y->esq = x; // x se torna o filho esquerdo de y
    x->pai = y; // y se torna o novo pai de x
}

// Função para realizar a rotação à direita em torno do nó x
void rotacaoDireita(No **raiz, No *x) {
    No *y = x->esq; // y será o filho esquerdo de x
    x->esq = y->dir; // y->dir agora se torna filho esquerdo de x

    if (y->dir != NULL) {
        y->dir->pai = x; // Se y->dir existe, atualiza o pai
    }

    y->pai = x->pai; // Atualiza o pai de y

    if (x->pai == NULL) {
        *raiz = y; // Se x era a raiz, agora y é a nova raiz
    } else if (x == x->pai->dir) {
        x->pai->dir = y; // Se x era filho direito, y toma o lugar de x
    } else {
        x->pai->esq = y; // Se x era filho esquerdo, y toma o lugar de x
    }

    y->dir = x; // x se torna o filho direito de y
    x->pai = y; // y se torna o novo pai de x
}

// Função para corrigir possíveis violações da árvore Red-Black após a inserção
void corrigirViolacao(No **raiz, No *z) {
    while (z != *raiz && z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esq) {
            No *tio = z->pai->pai->dir; // O "tio" de z (filho direito do avô)

            if (tio != NULL && tio->cor == VERMELHO) {
                // Caso 1: O pai e o tio são vermelhos, basta recolorir
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                // Caso 2: z é filho direito, rotação à esquerda
                if (z == z->pai->dir) {
                    z = z->pai;
                    rotacaoEsquerda(raiz, z);
                }

                // Caso 3: z é filho esquerdo, rotação à direita e recolorir
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(raiz, z->pai->pai);
            }
        } else {
            // Simétrico ao caso anterior, agora z está à direita do avô
            No *tio = z->pai->pai->esq;

            if (tio != NULL && tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacaoDireita(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = PRETO; // A raiz da árvore sempre deve ser preta
}

// Função para inserir um novo valor na árvore Red-Black
void inserir(No **raiz, int valor) {
    No *novo = criarNo(valor); // Cria um novo nó com o valor
    No *pai = NULL;
    No *atual = *raiz;

    // Procura a posição correta para o novo nó
    while (atual != NULL) {
        pai = atual;
        if (novo->valor < atual->valor) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    novo->pai = pai; // Define o pai do novo nó

    if (pai == NULL) {
        *raiz = novo; // Se não há pai, a árvore estava vazia e o novo nó é a raiz
    } else if (novo->valor < pai->valor) {
        pai->esq = novo; // Se o valor é menor, é filho esquerdo
    } else {
        pai->dir = novo; // Se o valor é maior, é filho direito
    }

    corrigirViolacao(raiz, novo); // Corrige as violações da árvore Red-Black
}

// Função para imprimir a árvore Red-Black em formato pré-ordenado (esquerda, raiz, direita)
void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        if (raiz->cor == VERMELHO) {
            printf("%d (Vermelho) ", raiz->valor); // Nó vermelho
        } else {
            printf("%d (Preto) ", raiz->valor); // Nó preto
        }
        emOrdem(raiz->dir);
    }
}

// Função para imprimir a árvore em formato hierárquico
void imprimeArvore(No *raiz, int nivel) {
    if (raiz != NULL) {
        imprimeArvore(raiz->dir, nivel + 1); // Imprime subárvore direita

        for (int i = 0; i < nivel; i++) {
            printf("    "); // Espaçamento para simular a profundidade
        }

        if (raiz->cor == VERMELHO) {
            printf("\033[31m%d\033[0m\n", raiz->valor); // Nó vermelho
        } else {
            printf("%d\n", raiz->valor); // Nó preto
        }

        imprimeArvore(raiz->esq, nivel + 1); // Imprime subárvore esquerda
    }
}

// Função principal para testar a árvore Red-Black
int main() {
    No *raiz = NULL; // Inicializa a árvore vazia

    // Inserção de valores de exemplo na árvore
    int valores[] = {30, 20, 40, 10, 25, 35, 50, 5, 15, 27};
    int tamanho = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < tamanho; i++) {
        inserir(&raiz, valores[i]);
    }

    // Exibe a árvore Red-Black
    printf("Árvore Red-Black em ordem:\n");
    emOrdem(raiz);
    printf("\n\nEstrutura da árvore:\n");
    imprimeArvore(raiz, 0);

    return 0;
}
