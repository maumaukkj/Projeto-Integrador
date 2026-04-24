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
    fgets(buffer, sizeof(buffer), arquivo); // Pula cabeçalho

    for (int i = 0; i < total; i++) {
        if (fgets(buffer, sizeof(buffer), arquivo)) {
            
            // 1. Limpa quebras de linha invisíveis
            buffer[strcspn(buffer, "\r")] = '\0';
            buffer[strcspn(buffer, "\n")] = '\0';
            
            // Professor, só um comentário meu aqui... Que dificuldade pra conseguir pegar o valor correto de todos os produtos ein... Tive que pedir ajuda pois não consegui resolver só.


            // ====================================================
            // PASSO 1: Pega o Preço pela ÚLTIMA vírgula da linha
            // ====================================================
            char *virgula_preco = strrchr(buffer, ','); 
            if (virgula_preco) {
                vetor[i].valor = atof(virgula_preco + 1); // Lê o número
                *virgula_preco = '\0'; // Corta a string aqui. Sobrou "ID,Nome,Categoria"
            }

            // ====================================================
            // PASSO 2: Pega o ID pela PRIMEIRA vírgula da linha
            // ====================================================
            char *virgula_id = strchr(buffer, ',');
            if (virgula_id) {
                *virgula_id = '\0'; // Corta a string aqui
                vetor[i].id = atoi(buffer); // O que sobrou no início é o ID
                
                // ====================================================
                // PASSO 3: Pega o Nome pela PRÓXIMA vírgula
                // ====================================================
                char *inicio_nome = virgula_id + 1; // O nome começa logo após a vírgula do ID
                char *virgula_nome = strchr(inicio_nome, ','); // Acha a vírgula depois do nome
                
                if (virgula_nome) {
                    *virgula_nome = '\0'; // Corta a string
                    strcpy(vetor[i].nome, inicio_nome);
                    
                    // ====================================================
                    // PASSO 4: O que sobrou é a Categoria!
                    // ====================================================
                    char *inicio_categoria = virgula_nome + 1;
                    
                    // Um "Lava Jato" rápido para tirar as aspas se elas existirem
                    int len = strlen(inicio_categoria);
                    if (inicio_categoria[0] == '"' && inicio_categoria[len-1] == '"') {
                        inicio_categoria[len-1] = '\0'; // Tira a última aspa
                        strcpy(vetor[i].categoria, inicio_categoria + 1); // Copia pulando a primeira
                    } else {
                        strcpy(vetor[i].categoria, inicio_categoria);
                    }
                }
            }
        }
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

    while (opcao != 2) {
        printf("\n================ MENU PRINCIPAL ================\n");
        printf("0. Executar Protocolo Oficial (3 Ciclos)\n");
        printf("1. Buscar Produto (Busca Sequencial - Manual)\n");
        printf("2. Sair do Sistema\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        switch (opcao) {
            case 0:
                printf("\n[SISTEMA] Iniciando Protocolo Oficial (3 Ciclos de 1000 IDs distintos)...\n");
                printf("[SISTEMA] Gerando 'alvos_fase1.txt' para o proximo bimestre...\n");

                FILE *flog = fopen("alvos_fase1.txt", "w");
                if (!flog) {
                    printf("[ERRO] Nao foi possivel criar o arquivo de log.\n");
                    break;
                }

                // Definição dos índices para os 1000 IDs (divididos em 3 blocos)
                int idx_inicio = 0;
                int idx_meio = total / 2;
                int idx_fim = total - 334;

                double t_inicio[3], t_meio[3], t_fim[3];

                for (int rep = 0; rep < 3; rep++) {
                    // --- BLOCO INICIO (333 IDs) ---
                    inicio_tempo = clock();
                    for(int k = 0; k < 333; k++) {
                        if (rep == 0) fprintf(flog, "%d\n", meuVetor[idx_inicio + k].id);
                        busca_sequencial(meuVetor, total, meuVetor[idx_inicio + k].id, &iteracoes);
                    }
                    fim_tempo = clock();
                    t_inicio[rep] = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;

                    // --- BLOCO MEIO (333 IDs) ---
                    inicio_tempo = clock();
                    for(int k = 0; k < 333; k++) {
                        if (rep == 0) fprintf(flog, "%d\n", meuVetor[idx_meio + k].id);
                        busca_sequencial(meuVetor, total, meuVetor[idx_meio + k].id, &iteracoes);
                    }
                    fim_tempo = clock();
                    t_meio[rep] = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;

                    // --- BLOCO FIM (334 IDs) ---
                    inicio_tempo = clock();
                    for(int k = 0; k < 334; k++) {
                        if (rep == 0) fprintf(flog, "%d\n", meuVetor[idx_fim + k].id);
                        busca_sequencial(meuVetor, total, meuVetor[idx_fim + k].id, &iteracoes);
                    }
                    fim_tempo = clock();
                    t_fim[rep] = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;
                }
                fclose(flog);

                // Médias de cada cenário
                double m_inicio = (t_inicio[0] + t_inicio[1] + t_inicio[2]) / 3.0;
                double m_meio = (t_meio[0] + t_meio[1] + t_meio[2]) / 3.0;
                double m_fim = (t_fim[0] + t_fim[1] + t_fim[2]) / 3.0;

                // Médias Gerais por Ciclo (Colunas)
                double m_rep1 = (t_inicio[0] + t_meio[0] + t_fim[0]) / 3.0;
                double m_rep2 = (t_inicio[1] + t_meio[1] + t_fim[1]) / 3.0;
                double m_rep3 = (t_inicio[2] + t_meio[2] + t_fim[2]) / 3.0;
                double m_final = (m_inicio + m_meio + m_fim) / 3.0;

                // --- TABELA 1: MÉTRICAS POR REPETIÇÃO ---
                printf("\n======================== RESULTADOS DO PROTOCOLO (3 CICLOS) ========================\n");
                printf("Registros: %d | Total de Alvos Distintos: 1000\n", total);
                printf("------------------------------------------------------------------------------------\n");
                printf("%-15s | %-10s | %-10s | %-10s | %-15s\n", "Cenario", "Rep 1 (s)", "Rep 2 (s)", "Rep 3 (s)", "Media Final (s)");
                printf("------------------------------------------------------------------------------------\n");
                printf("%-15s | %-10.4f | %-10.4f | %-10.4f | %-15.4f\n", "Busca Inicio", t_inicio[0], t_inicio[1], t_inicio[2], m_inicio);
                printf("%-15s | %-10.4f | %-10.4f | %-10.4f | %-15.4f\n", "Busca Meio", t_meio[0], t_meio[1], t_meio[2], m_meio);
                printf("%-15s | %-10.4f | %-10.4f | %-10.4f | %-15.4f\n", "Busca Fim", t_fim[0], t_fim[1], t_fim[2], m_fim);
                printf("------------------------------------------------------------------------------------\n");
                printf("%-15s | %-10.4f | %-10.4f | %-10.4f | %-15.4f\n", "Media Ciclo", m_rep1, m_rep2, m_rep3, m_final);
                printf("====================================================================================\n\n");

                // --- TABELA 2: MÉDIAS UNITÁRIAS (A "MÉDIA DAS MÉDIAS") ---
                printf(">>> TEMPO UNITARIO PROCESSADO (Desempenho por busca individual) <<<\n");
                printf("------------------------------------------------------------------------------------\n");
                printf("%-15s | %-12s | %-12s | %-12s | %-15s\n", "Cenario", "Rep 1 (s)", "Rep 2 (s)", "Rep 3 (s)", "Media Unitaria");
                printf("------------------------------------------------------------------------------------\n");
                printf("%-15s | %-12.8f | %-12.8f | %-12.8f | %-15.8f\n", "Media Inicio", t_inicio[0]/333, t_inicio[1]/333, t_inicio[2]/333, m_inicio/333);
                printf("%-15s | %-12.8f | %-12.8f | %-12.8f | %-15.8f\n", "Media Meio", t_meio[0]/333, t_meio[1]/333, t_meio[2]/333, m_meio/333);
                printf("%-15s | %-12.8f | %-12.8f | %-12.8f | %-15.8f\n", "Media Fim", t_fim[0]/334, t_fim[1]/334, t_fim[2]/334, m_fim/334);
                printf("------------------------------------------------------------------------------------\n");
                break;

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
                printf("\n[SISTEMA] Liberando memoria e encerrando... Ate logo!\n");
                break;

            default:
                printf("\n[ERRO] Opcao invalida. Tente novamente.\n");
        }
    }

    free(meuVetor);
    return 0;
}