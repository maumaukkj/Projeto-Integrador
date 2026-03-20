📊 Projeto Integrador: Análise de Desempenho (Baseline)
Este repositório contém o desenvolvimento da Fase I do Projeto Integrador, focado na criação de uma base experimental para análise de algoritmos de busca sequencial.

Status do Projeto: 🛠️ Em desenvolvimento (Fase de Modelagem e Carga)

📌 Cronograma de Desenvolvimento
[x] Planejamento da estrutura de dados.
[/] Implementação da leitura de CSV (Em andamento).
[ ] Implementação da busca sequencial.
[ ] Protocolo de testes de performance (Mín. 1.000 buscas).
[ ] Relatório técnico final.

📦 Modelagem de Dados
Para esta etapa inicial, estamos utilizando a seguinte estrutura para representar os produtos do dataset:

typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
    } Produto;

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

gcc main.c -o pi_fase1
./pi_fase1
