#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash.h"

// Nosso leitor Sniper adaptado para inserir direto na Hash
void carregar_dataset_hash(const char *nomeArquivo, HashTable *ht, int total) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("[ERRO] Nao foi possivel abrir o dataset em: %s\n", nomeArquivo);
        return;
    }

    char buffer[1024];
    fgets(buffer, sizeof(buffer), arquivo); // Pula cabeçalho

    Produto p;
    for (int i = 0; i < total; i++) {
        if (fgets(buffer, sizeof(buffer), arquivo)) {
            // 1. Limpa quebras de linha invisíveis
            buffer[strcspn(buffer, "\r")] = '\0';
            buffer[strcspn(buffer, "\n")] = '\0';

            // PASSO 1: Pega o Preço pela ÚLTIMA vírgula
            char *virgula_preco = strrchr(buffer, ','); 
            if (virgula_preco) {
                p.valor = atof(virgula_preco + 1);
                *virgula_preco = '\0';
            }

            // PASSO 2: Pega o ID pela PRIMEIRA vírgula
            char *virgula_id = strchr(buffer, ',');
            if (virgula_id) {
                *virgula_id = '\0';
                p.id = atoi(buffer);
                
                // PASSO 3: Pega o Nome
                char *inicio_nome = virgula_id + 1;
                char *virgula_nome = strchr(inicio_nome, ',');
                
                if (virgula_nome) {
                    *virgula_nome = '\0';
                    strcpy(p.nome, inicio_nome);
                    
                    // PASSO 4: Categoria
                    char *inicio_categoria = virgula_nome + 1;
                    int len = strlen(inicio_categoria);
                    if (inicio_categoria[0] == '"' && inicio_categoria[len-1] == '"') {
                        inicio_categoria[len-1] = '\0';
                        strcpy(p.categoria, inicio_categoria + 1);
                    } else {
                        strcpy(p.categoria, inicio_categoria);
                    }
                }
            }
            // Insere o produto estruturado direto na tabela hash
            inserirHash(ht, p);
        }
    }
    fclose(arquivo);
}

int main() {
    int total_registros = 400009;
    HashTable ht;
    
    // 1. Inicializa a Tabela
    inicializarTabela(&ht);
    
    printf("[SISTEMA] Carregando dados na Tabela Hash (Aguarde)...\n");
    // Caminho relativo apontando para a pasta fase1
    carregar_dataset_hash("../fase1/dataset4.csv", &ht, total_registros);
    
    printf("==================================================\n");
    printf("TABELA HASH CARREGADA COM SUCESSO\n");
    printf("Tamanho da Tabela: %d\n", TABLE_SIZE);
    printf("Total de Colisoes: %d\n", ht.total_colisoes);
    printf("==================================================\n\n");

    // 2. Lógica do Protocolo Experimental (Lendo os 1000 alvos salvos da Fase 1)
    printf("[SISTEMA] Iniciando Protocolo de Testes baseado em 'alvos_fase1.txt'...\n");
    
    int alvos[1000];
    FILE *f_alvos = fopen("../fase1/alvos_fase1.txt", "r");
    if (!f_alvos) {
        printf("[ERRO] Arquivo alvos_fase1.txt nao encontrado em ../fase1/\n");
        return 1;
    }
    
    for (int i = 0; i < 1000; i++) {
        if (fscanf(f_alvos, "%d", &alvos[i]) != 1) {
            alvos[i] = 99999999; // Fallback para inexistente caso falte linha
        }
    }
    fclose(f_alvos);

    // Rodando os 3 Ciclos de Medição na Tabela Hash
    double t_inicio[3], t_meio[3], t_fim[3];
    clock_t inicio_tempo, fim_tempo;
    int comparacoes = 0, total_comp = 0;

    for (int rep = 0; rep < 3; rep++) {
        // Bloco Início (Primeiros 333 IDs do log)
        inicio_tempo = clock();
        for (int k = 0; k < 333; k++) {
            buscarHash(&ht, alvos[k], &comparacoes);
        }
        fim_tempo = clock();
        t_inicio[rep] = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;

        // Bloco Meio (Próximos 333 IDs do log)
        inicio_tempo = clock();
        for (int k = 333; k < 666; k++) {
            buscarHash(&ht, alvos[k], &comparacoes);
        }
        fim_tempo = clock();
        t_meio[rep] = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;

        // Bloco Fim (Últimos 334 IDs do log)
        inicio_tempo = clock();
        for (int k = 666; k < 1000; k++) {
            buscarHash(&ht, alvos[k], &comparacoes);
        }
        fim_tempo = clock();
        t_fim[rep] = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;
    }

    // Médias
    double m_inicio = (t_inicio[0] + t_inicio[1] + t_inicio[2]) / 3.0;
    double m_meio = (t_meio[0] + t_meio[1] + t_meio[2]) / 3.0;
    double m_fim = (t_fim[0] + t_fim[1] + t_fim[2]) / 3.0;

    // --- TABELA DE RESULTADOS FASE II ---
    printf("\n======================== RESULTADOS DA TABELA HASH (3 CICLOS) ========================\n");
    printf("%-15s | %-10s | %-10s | %-10s | %-15s\n", "Cenario", "Rep 1 (s)", "Rep 2 (s)", "Rep 3 (s)", "Media Final (s)");
    printf("------------------------------------------------------------------------------------\n");
    printf("%-15s | %-10.6f | %-10.6f | %-10.6f | %-15.6f\n", "Busca Inicio", t_inicio[0], t_inicio[1], t_inicio[2], m_inicio);
    printf("%-15s | %-10.6f | %-10.6f | %-10.6f | %-15.6f\n", "Busca Meio", t_meio[0], t_meio[1], t_meio[2], m_meio);
    printf("%-15s | %-10.6f | %-10.6f | %-10.6f | %-15.6f\n", "Busca Fim", t_fim[0], t_fim[1], t_fim[2], m_fim);
    printf("====================================================================================\n\n");

    // Teste de exibição de elemento unitário para provar o funcionamento
    int comp_teste = 0;
    Produto* prod = buscarHash(&ht, alvos[0], &comp_teste);
    if(prod) {
        printf("[VALIDACAO] ID: %d | %s | %s | R$ %.2f (Acessado em %d comparacoes)\n", 
               prod->id, prod->nome, prod->categoria, prod->valor, comp_teste);
    }

    return 0;
}