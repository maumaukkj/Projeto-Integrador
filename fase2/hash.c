#include "hash.h"

// Inicializa todos os ponteiros da tabela com NULL e zera as colisões
void inicializarTabela(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->tabela[i] = NULL;
    }
    ht->total_colisoes = 0;
}

// Função Hash: Meio do Quadrado Simplificado (Mid-Square)
int funcaoHash(int id) {
    // 1. Cast para long long para evitar Overflow na multiplicação
    long long quadrado = (long long)id * id;
    
    // 2. Aplica a fórmula da equipe: (x * x / 100) % m
    // O m é o nosso TABLE_SIZE (400010)
    int indice = (int)((quadrado / 100) % TABLE_SIZE);
    
    // 3. Blindagem extra: garantir que o índice gerado seja sempre positivo
    if (indice < 0) {
        indice += TABLE_SIZE;
    }
    
    return indice;
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
