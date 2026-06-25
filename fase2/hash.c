#include "hash.h"

void inicializarTabela(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->tabela[i] = NULL;
    }
    ht->total_colisoes = 0;
}

// Função Hash F4: Meio do Quadrado Simplificado (Mid-Square)
int funcaoHash(int id) {
    long long quadrado = (long long)id * id;
    int indice = (int)((quadrado / 100) % TABLE_SIZE);
    if (indice < 0) indice += TABLE_SIZE;
    return indice;
}

void inserirHash(HashTable* ht, Produto p) {
    int indice = funcaoHash(p.id);
    
    Node* novoNo = (Node*)malloc(sizeof(Node));
    novoNo->produto = p;
    novoNo->next = NULL;

    if (ht->tabela[indice] == NULL) {
        ht->tabela[indice] = novoNo;
    } else {
        ht->total_colisoes++; // Colisão!
        Node* atual = ht->tabela[indice];
        while (atual->next != NULL) {
            atual = atual->next;
        }
        atual->next = novoNo;
    }
}

// Nova função: Busca na Tabela Hash
Produto* buscarHash(HashTable* ht, int id_buscado, int* comparacoes) {
    int indice = funcaoHash(id_buscado);
    Node* atual = ht->tabela[indice];
    *comparacoes = 0;

    while (atual != NULL) {
        (*comparacoes)++;
        if (atual->produto.id == id_buscado) {
            return &(atual->produto); // Encontrou
        }
        atual = atual->next; // Procura no próximo nó da lista encadeada
    }
    
    return NULL; // Não encontrou
}
