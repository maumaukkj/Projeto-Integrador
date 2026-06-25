#include "hash.h"

// Inicializa todos os ponteiros da tabela com NULL e zera as colisões
void inicializarTabela(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->tabela[i] = NULL;
    }
    ht->total_colisoes = 0;
}

// Função Hash provisória (Método da Divisão)
int funcaoHash(int id) {
    return id % TABLE_SIZE;
}

// Esboço da função de inserção com tratamento de colisão por encadeamento
void inserirHash(HashTable* ht, Produto p) {
    int indice = funcaoHash(p.id);
    
    // Aloca memória para o novo nó
    Node* novoNo = (Node*)malloc(sizeof(Node));
    novoNo->produto = p;
    novoNo->next = NULL;

    // Se o índice estiver vazio, insere direto
    if (ht->tabela[indice] == NULL) {
        ht->tabela[indice] = novoNo;
    } else {
        // Ocorreu uma colisão! Incrementa o contador geral
        ht->total_colisoes++;
        
        // Percorre a lista encadeada até o final para inserir o novo nó
        Node* atual = ht->tabela[indice];
        while (atual->next != NULL) {
            atual = atual->next;
        }
        atual->next = novoNo;
    }
}
