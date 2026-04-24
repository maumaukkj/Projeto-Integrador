#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Issue: Implementação do Timer (Telemetria)

// ==========================================================
// --- Issue #1: Definição da Estrutura de Dados (TAD) ---
// ==========================================================
typedef struct {
    int id;
    char nome[100];
    char categoria[100];
    float valor;
} Produto;

// ==========================================================
// --- Issue #2: Contagem para Alocação Dinâmica ---
// ==========================================================
int contar_total_registros(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("[ERRO] Nao foi possivel abrir o arquivo %s\n", nomeArquivo);
        return -1;
    }

    int linhas = 0;
    char buffer[1024];
    
    fgets(buffer, sizeof(buffer), arquivo); // Ignora o cabeçalho do CSV

    while (fgets(buffer, sizeof(buffer), arquivo)) {
        if (strlen(buffer) > 1) {
            linhas++;
        }
    }

    fclose(arquivo);
    return linhas;
}

// ==========================================================
// --- Issue #3: Leitura e Carga do Arquivo CSV ---
// ==========================================================
void carregar_dataset(const char *nomeArquivo, Produto *vetor, int total) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) return; 

    char buffer[1024];
    fgets(buffer, sizeof(buffer), arquivo);

    for (int i = 0; i < total; i++) {
        fscanf(arquivo, "%d,%[^,],%[^,],%f\n", 
               &vetor[i].id, 
               vetor[i].nome, 
               vetor[i].categoria, 
               &vetor[i].valor);
    }

    fclose(arquivo);
}

// ==========================================================
// --- Issue #4: Implementação da Busca Sequencial ---
// ==========================================================
int busca_sequencial(Produto *vetor, int total, int id_buscado, int *iteracoes) {
    *iteracoes = 0;
    for (int i = 0; i < total; i++) {
        (*iteracoes)++;
        if (vetor[i].id == id_buscado) {
            return i;
        }
    }
    return -1;
}

// ==========================================================
// --- Issue #5: Ordenação do Vetor (QuickSort) ---
// ==========================================================
void trocar(Produto *a, Produto *b) {
    Produto temp = *a;
    *a = *b;
    *b = temp;
}

int particionar(Produto *vetor, int inicio, int fim) {
    int pivo = vetor[fim].id;
    int i = (inicio - 1);
    for (int j = inicio; j < fim; j++) {
        if (vetor[j].id <= pivo) {
            i++;
            trocar(&vetor[i], &vetor[j]);
        }
    }
    trocar(&vetor[i + 1], &vetor[fim]);
    return (i + 1);
}

void quicksort(Produto *vetor, int inicio, int fim) {
    if (inicio < fim) {
        int pi = particionar(vetor, inicio, fim);
        quicksort(vetor, inicio, pi - 1);
        quicksort(vetor, pi + 1, fim);
    }
}

// ==========================================================
// --- Issue #6: Implementação da Busca Binária ---
// ==========================================================
int busca_binaria(Produto *vetor, int total, int id_buscado, int *iteracoes) {
    int inicio = 0;
    int fim = total - 1;
    *iteracoes = 0;

    while (inicio <= fim) {
        (*iteracoes)++;
        int meio = inicio + (fim - inicio) / 2;

        if (vetor[meio].id == id_buscado) {
            return meio; // Encontrou
        }
        if (vetor[meio].id < id_buscado) {
            inicio = meio + 1; // Busca na metade direita
        } else {
            fim = meio - 1; // Busca na metade esquerda
        }
    }
    return -1; // Não encontrou
}

// ==========================================================
// --- Issue #7: Impressão de Produtos em Ordem ---
// ==========================================================
void imprimir_em_ordem(Produto *vetor, int total) {
    printf("\n--- Listagem de Produtos (Ordenados por ID) ---\n");
    int limite = (total < 50) ? total : 50; 
    for (int i = 0; i < limite; i++) {
        printf("ID: %06d | %-35s | R$ %8.2f\n", 
               vetor[i].id, vetor[i].nome, vetor[i].valor);
    }
    if (total > 50) {
        printf("... (exibindo os primeiros %d de %d produtos)\n", limite, total);
    }
    printf("-----------------------------------------------\n");
}

// ==========================================================
// --- Issue #8: Menu Interativo e Ponto de Entrada ---
// ==========================================================
int main() {
    const char *path = "dataset4.csv"; 
    
    printf("==================================================\n");
    printf("    PROJETO INTEGRADOR: ESTRUTURAS DE DADOS\n");
    printf("==================================================\n");
    
    printf("[SISTEMA] Iniciando Fase de Carga...\n");

    int total = contar_total_registros(path);
    if (total <= 0) {
        printf("[ERRO] Arquivo '%s' nao encontrado ou vazio.\n", path);
        return 1;
    }

    Produto *meuVetor = (Produto *)malloc(total * sizeof(Produto));
    if (meuVetor == NULL) {
        printf("[ERRO] Falha de alocacao de memoria.\n");
        return 1;
    }

    carregar_dataset(path, meuVetor, total);
    printf("[SISTEMA] %d registros carregados com sucesso.\n", total);
    
    // Automatização do preparo (Issue #5)
    printf("[SISTEMA] Ordenando base de dados (QuickSort)...\n");
    quicksort(meuVetor, 0, total - 1);
    printf("[SISTEMA] Base ordenada. Pronto para buscas!\n");

    int opcao = 0;
    int id_busca, indice, iteracoes;
    clock_t inicio_tempo, fim_tempo;
    double tempo_gasto;

    while (opcao != 4) {
        printf("\n================ MENU PRINCIPAL ================\n");
        printf("1. Buscar Produto (Busca Sequencial)\n");
        printf("2. Buscar Produto (Busca Binaria)\n");
        printf("3. Imprimir Produtos em Ordem\n");
        printf("4. Sair do Sistema\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        switch (opcao) {
            case 1:
                printf("\nDigite o ID para Busca SEQUENCIAL: ");
                scanf("%d", &id_busca);
                
                inicio_tempo = clock();
                indice = busca_sequencial(meuVetor, total, id_busca, &iteracoes);
                fim_tempo = clock();
                
                tempo_gasto = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;

                if (indice != -1) {
                    printf("\n[ENCONTRADO] %s | Categoria: %s | R$ %.2f\n", 
                           meuVetor[indice].nome, meuVetor[indice].categoria, meuVetor[indice].valor);
                } else {
                    printf("\n[FALHA] Produto ID %d nao localizado.\n", id_busca);
                }
                printf(">>> TELEMETRIA: %d iteracoes | Tempo: %f segundos\n", iteracoes, tempo_gasto);
                break;

            case 2:
                printf("\nDigite o ID para Busca BINARIA: ");
                scanf("%d", &id_busca);
                
                inicio_tempo = clock();
                indice = busca_binaria(meuVetor, total, id_busca, &iteracoes);
                fim_tempo = clock();
                
                tempo_gasto = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;

                if (indice != -1) {
                    printf("\n[ENCONTRADO] %s | Categoria: %s | R$ %.2f\n", 
                           meuVetor[indice].nome, meuVetor[indice].categoria, meuVetor[indice].valor);
                } else {
                    printf("\n[FALHA] Produto ID %d nao localizado.\n", id_busca);
                }
                printf(">>> TELEMETRIA: %d iteracoes | Tempo: %f segundos\n", iteracoes, tempo_gasto);
                break;

            case 3:
                imprimir_em_ordem(meuVetor, total);
                break;

            case 4:
                printf("\n[SISTEMA] Liberando memoria e encerrando... Ate logo!\n");
                break;

            default:
                printf("\n[ERRO] Opcao invalida. Tente novamente.\n");
        }
    }

    free(meuVetor);
    return 0;
}
