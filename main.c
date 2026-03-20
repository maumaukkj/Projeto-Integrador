#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Issue #1: Definição da Estrutura de Dados (TAD) ---
typedef struct {
    int id;
    char nome[100];
    char categoria[100];
    float valor;
} Produto;

// --- Issue #3: Contagem para Alocação Dinamica ---
int contar_total_registros(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nomeArquivo);
        return -1;
    }

    int linhas = 0;
    char buffer[1024];
    
    // Ignora cabeçalho
    fgets(buffer, sizeof(buffer), arquivo); 

    while (fgets(buffer, sizeof(buffer), arquivo)) {
        if (strlen(buffer) > 1) {
            linhas++;
        }
    }

    fclose(arquivo);
    return linhas;
}

// --- Issue #2: Leitura do Arquivo CSV ---
void carregar_dataset(const char *nomeArquivo, Produto *vetor, int total) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    
    // Segurança extra
    if (!arquivo) return; 

    char buffer[1024];
    fgets(buffer, sizeof(buffer), arquivo);

    for (int i = 0; i < total; i++) {
        // fscanf lê o id, pula a vírgula, lê até a próxima vírgula, e assim vai...
        fscanf(arquivo, "%d,%[^,],%[^,],%f\n", 
               &vetor[i].id, 
               vetor[i].nome, 
               vetor[i].categoria, 
               &vetor[i].valor);
    }

    fclose(arquivo);
}

int main() {
    const char *path = "dataset4.csv"; 
    
    printf("Iniciando Fase I - Projeto Integrador...\n");

    int total = contar_total_registros(path);
    if (total <= 0) {
        printf("Erro: Arquivo nao encontrado ou vazio.\n");
        return 1;
    }

    printf("Total de registros detectados: %d\n", total);

    // Alocação Dinâmica
    Produto *meuVetor = (Produto *)malloc(total * sizeof(Produto));
    if (meuVetor == NULL) {
        printf("Erro de memoria\n");
        return 1;
    }

    printf("Carregando dados na memoria...\n");
    carregar_dataset(path, meuVetor, total);

    // Teste de Verificação
    printf("\n--- Teste de Carga (Primeiros 3 itens) ---\n");
    for (int i = 0; i < 3; i++) {
        printf("ID: %d | Nome: %s | Valor: R$%.2f\n", 
                meuVetor[i].id, meuVetor[i].nome, meuVetor[i].valor);
    }

    printf("\nPronto para iniciar a Fase de Busca.\n");

    free(meuVetor);
    return 0;
}