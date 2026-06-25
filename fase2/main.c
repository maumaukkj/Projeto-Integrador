#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

int main() {
    // 1. Instancia e inicializa a Tabela Hash
    HashTable ht;
    inicializarTabela(&ht);

    printf("Tabela Hash inicializada com tamanho: %d\n", TABLE_SIZE);

    // 2. Lógica de Parsing do CSV (A ser implementada com strchr/strrchr)
    // FILE* file = fopen("../dataset4.csv", "r");
    // ... loop de leitura ...
    // inserirHash(&ht, produto_lido);
    
    // 3. Exibição parcial de resultados
    printf("Total de colisoes registradas: %d\n", ht.total_colisoes);

    return 0;
}
