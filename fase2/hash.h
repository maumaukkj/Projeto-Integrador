#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tamanho exato do dataset
#define TABLE_SIZE 400010

// Sua struct idêntica à Fase 1
typedef struct {
    int id;
    char nome[100];
    char categoria[100];
    float valor;
} Produto;

// Nó da Lista Encadeada (Tratamento de Colisão)
typedef struct Node {
    Produto produto;
    struct Node* next;
} Node;

// Tabela Hash
typedef struct {
    Node* tabela[TABLE_SIZE];
    int total_colisoes;
} HashTable;

void inicializarTabela(HashTable* ht);
int funcaoHash(int id);
void inserirHash(HashTable* ht, Produto p);
Produto* buscarHash(HashTable* ht, int id_buscado, int* comparacoes);

#endif
