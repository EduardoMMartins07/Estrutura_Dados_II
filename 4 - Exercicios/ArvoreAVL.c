// Eduardo Moisés Martins
#include <stdio.h>
#include <stdlib.h>

// Estrutura de nó da árvore AVL
// Define um nó de uma árvore AVL com um valor inteiro, ponteiros para filhos e a altura.
struct NoAVL {
    int valor;            // Valor armazenado no nó
    struct NoAVL *esq;    // Ponteiro para o filho à esquerda
    struct NoAVL *dir;    // Ponteiro para o filho à direita
    int altura;           // Altura do nó (usada para verificar o balanceamento)
};

// Função para criar um novo nó na árvore AVL
// Aloca memória e inicializa o nó com o valor fornecido, altura inicial 1.
struct NoAVL* criarNovoNo(int valor) {
    struct NoAVL* no = (struct NoAVL*) malloc(sizeof(struct NoAVL));
    no->valor = valor;
    no->esq = NULL;
    no->dir = NULL;
    no->altura = 1;  // Nó folha tem altura 1
    return no;
}

// Função para obter a altura de um nó
// Retorna 0 se o nó for NULL, caso contrário, retorna sua altura.
int obterAltura(struct NoAVL* no) {
    if (no == NULL)
        return 0;
    return no->altura;
}

// Função para obter o maior valor entre dois números
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para realizar rotação à direita
// Move o nó à esquerda para cima e ajusta os ponteiros.
struct NoAVL* rotacaoDireita(struct NoAVL* y) {
    struct NoAVL* x = y->esq;
    struct NoAVL* T2 = x->dir;

    // Realiza a rotação
    x->dir = y;
    y->esq = T2;

    // Atualiza as alturas
    y->altura = max(obterAltura(y->esq), obterAltura(y->dir)) + 1;
    x->altura = max(obterAltura(x->esq), obterAltura(x->dir)) + 1;

    // Retorna a nova raiz
    return x;
}

// Função para realizar rotação à esquerda
// Move o nó à direita para cima e ajusta os ponteiros.
struct NoAVL* rotacaoEsquerda(struct NoAVL* x) {
    struct NoAVL* y = x->dir;
    struct NoAVL* T2 = y->esq;

    // Realiza a rotação
    y->esq = x;
    x->dir = T2;

    // Atualiza as alturas
    x->altura = max(obterAltura(x->esq), obterAltura(x->dir)) + 1;
    y->altura = max(obterAltura(y->esq), obterAltura(y->dir)) + 1;

    // Retorna a nova raiz
    return y;
}

// Função para calcular o fator de balanceamento
// Diferença entre as alturas das subárvores esquerda e direita.
int obterFatorBalanceamento(struct NoAVL* no) {
    if (no == NULL)
        return 0;
    return obterAltura(no->esq) - obterAltura(no->dir);
}

// Função para inserir um valor na árvore AVL
// Insere o valor e ajusta o balanceamento da árvore.
struct NoAVL* inserirNo(struct NoAVL* no, int valor) {
    // Passo básico de inserção em árvore binária
    if (no == NULL)
        return criarNovoNo(valor);

    if (valor < no->valor)
        no->esq = inserirNo(no->esq, valor);
    else if (valor > no->valor)
        no->dir = inserirNo(no->dir, valor);
    else
        return no;  // Valores duplicados não são permitidos

    // Atualiza a altura do nó atual
    no->altura = 1 + max(obterAltura(no->esq), obterAltura(no->dir));

    // Verifica o balanceamento do nó atual
    int balance = obterFatorBalanceamento(no);

    // Caso 1: Rotação à direita (desbalanceado à esquerda)
    if (balance > 1 && valor < no->esq->valor)
        return rotacaoDireita(no);

    // Caso 2: Rotação à esquerda (desbalanceado à direita)
    if (balance < -1 && valor > no->dir->valor)
        return rotacaoEsquerda(no);

    // Caso 3: Rotação dupla (esquerda-direita)
    if (balance > 1 && valor > no->esq->valor) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }

    // Caso 4: Rotação dupla (direita-esquerda)
    if (balance < -1 && valor < no->dir->valor) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no;  // Retorna o nó ajustado
}

// Função para encontrar o nó de menor valor na subárvore
// Percorre a subárvore à esquerda para encontrar o menor valor.
struct NoAVL* menorValorNo(struct NoAVL* no) {
    struct NoAVL* atual = no;

    // Nó mais à esquerda é o menor
    while (atual->esq != NULL)
        atual = atual->esq;

    return atual;
}

// Função para remover um nó da árvore AVL
// Remove o valor e ajusta o balanceamento da árvore.
struct NoAVL* removerNo(struct NoAVL* raiz, int valor) {
    if (raiz == NULL)
        return raiz;

    // Busca o nó a ser removido
    if (valor < raiz->valor)
        raiz->esq = removerNo(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = removerNo(raiz->dir, valor);
    else {
        // Caso o nó tenha 0 ou 1 filhos
        if ((raiz->esq == NULL) || (raiz->dir == NULL)) {
            struct NoAVL* temp = raiz->esq ? raiz->esq : raiz->dir;

            // Caso sem filhos
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else  // Caso com 1 filho
                *raiz = *temp;

            free(temp);
        } else {
            // Caso com 2 filhos: encontra o sucessor
            struct NoAVL* temp = menorValorNo(raiz->dir);

            // Substitui o valor do nó pelo valor do sucessor
            raiz->valor = temp->valor;

            // Remove o sucessor
            raiz->dir = removerNo(raiz->dir, temp->valor);
        }
    }

    if (raiz == NULL)
        return raiz;

    // Atualiza a altura do nó atual
    raiz->altura = 1 + max(obterAltura(raiz->esq), obterAltura(raiz->dir));

    // Verifica o balanceamento e ajusta se necessário
    int balance = obterFatorBalanceamento(raiz);

    // Rotação à direita
    if (balance > 1 && obterFatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);

    // Rotação esquerda-direita
    if (balance > 1 && obterFatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    // Rotação à esquerda
    if (balance < -1 && obterFatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);

    // Rotação direita-esquerda
    if (balance < -1 && obterFatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Função para imprimir a árvore AVL em ordem crescente
void imprimirEmOrdem(struct NoAVL* raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esq);
        printf("%d ", raiz->valor);
        imprimirEmOrdem(raiz->dir);
    }
}

// Função principal
int main() {
    struct NoAVL* raiz = NULL;

    // Insere valores na árvore
    raiz = inserirNo(raiz, 20);
    raiz = inserirNo(raiz, 10);
    raiz = inserirNo(raiz, 30);
    raiz = inserirNo(raiz, 5);
    raiz = inserirNo(raiz, 15);
    raiz = inserirNo(raiz, 25);
    raiz = inserirNo(raiz, 35);

    // Remove um valor
    raiz = removerNo(raiz, 10);

    // Imprime a árvore AVL em ordem
    printf("Árvore AVL em ordem:\n");
    imprimirEmOrdem(raiz);
    printf("\n");

    return 0;
}
