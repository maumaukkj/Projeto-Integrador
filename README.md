Este repositório contém o desenvolvimento da **Fase I** do Projeto Integrador, focado na criação de uma base experimental para análise de algoritmos de busca sequencial.

> **Status do Projeto:** 🛠️ Em desenvolvimento (Fase de Modelagem e Carga) 

## 📌 Cronograma de Desenvolvimento
- [x] Planejamento da estrutura de dados.
- [x] Implementação da leitura de CSV (Em andamento).
- [x] Implementação da busca sequencial.
- [x] Protocolo de testes de performance (Mín. 1.000 buscas).
- [x] Relatório técnico final.

## 📦 Modelagem de Dados
Para esta etapa inicial, estamos utilizando a seguinte estrutura para representar os produtos do dataset:

```c
typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;
```

🛠️ Tecnologias e Ferramentas

- Linguagem: C
- Compilador: GCC
- Versionamento: Git/GitHub
- Dataset: Arquivo CSV com registros de produtos.

🔬 Metodologia (Prevista)
O projeto seguirá um protocolo rigoroso de testes:

1. Carga total do vetor dinâmico.
2. Execução de 1.000 buscas para cálculo de tempo médio.
3. Testes em diferentes cenários: início, meio, fim e elementos inexistentes.

🚀 Como Compilar (Versão Atual)
```c
gcc main.c -o projeto
./projeto
```

## 🚀 Fase II - Tabela Hash com Encadeamento

Nesta segunda fase, otimizamos o motor de buscas utilizando uma **Tabela Hash** com tratamento de colisões por **Lista Encadeada**. O protocolo experimental utilizou os mesmos 1.000 IDs gerados na Fase I para garantir a paridade dos testes.

### 📊 Resultados Quantitativos
* **Tamanho da Tabela:** 400.010
* **Total de Colisões:** 174.464

### ⏱️ Tabela Comparativa de Desempenho (Média de 3 Ciclos)

| Cenário de Busca | Busca Sequencial $O(N)$ | Tabela Hash $O(1)$ | Eficiência |
| :--- | :--- | :--- | :--- |
| **Busca Início** | 0.000005 s | 0.000000 s | Absoluta |
| **Busca Meio** | 0.543827 s | 0.000000 s | > 99.9% de redução |
| **Busca Fim** | 2.570043 s | 0.000000 s | > 99.9% de redução |
| **Inexistente** | 2.513655 s | 0.000000 s | > 99.9% de redução |

Os resultados comprovam empiricamente a mudança da complexidade de $O(N)$ para $O(1)$, eliminando o gargalo de processamento no pior caso.
