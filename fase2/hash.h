#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo o tamanho exato do dataset para a Tabela Hash
#define TABLE_SIZE 400010

// Estrutura para armazenar os dados do produto (Baseado na Fase 1)
typedef struct {
    int id;
    char nome[150];
    char categoria[100];
    float preco;
} Produto;

// Estrutura do Nó para a Lista Encadeada (Tratamento de Colisões)
typedef struct Node {
    Produto produto;
    struct Node* next;
} Node;

// Estrutura da Tabela Hash contendo o array de ponteiros e o contador de colisões
typedef struct {
    Node* tabela[TABLE_SIZE];
    int total_colisoes;
} HashTable;

// Assinaturas das funções
void inicializarTabela(HashTable* ht);
int funcaoHash(int id);
void inserirHash(HashTable* ht, Produto p);

#endif
