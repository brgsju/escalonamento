#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// #define MAX_PROCESSOS 10
#define QUANTUM 2 // Quantum para o Round Robin

struct Processo {
    int id;
    int tempo_execucao;
    int tempo_chegada;
    int prioridade;
    int tempo_restante; //Quanto tempo de execução ainda resta para o processo.
    int tempo_espera; // Quanto tempo o processo esperou até ser executado.
    int tempo_resposta; // O tempo desde a chegada até começar a execução.
    int turnaround_time;
};

// Função para ajustar as prioridades dinamicamente (Escalonamento com Prioridade Dinâmica)
void ajustarPrioridade(struct Processo p[], int n, int tempo_atual) {
    for (int i = 0; i < n; i++) {
        if (p[i].tempo_restante > 0) { //se o processo ainda não foi concluido 
            if (tempo_atual - p[i].tempo_chegada > 5) { //
                p[i].prioridade += 1; // Aumenta a prioridade após 5 unidades de tempo
            }
            if (p[i].tempo_restante <= 2) { //se faltar 2 seg ou menos para ser concluido 
                p[i].prioridade += 2; // Prioridade maior para processos quase concluídos
            }
        }
    }
}

// Função para escalonamento com prioridade dinâmica
void escalonamentoDinamico(struct Processo p[], int n) {
    int tempo_atual = 0, processos_concluidos = 0;
    int tempo_espera_total = 0, turnaround_time_total = 0, tempo_resposta_total = 0;

    while (processos_concluidos < n) {
        ajustarPrioridade(p, n, tempo_atual);

        int idx = -1;
        int maior_prioridade = -1000;
        for (int i = 0; i < n; i++) {
            if (p[i].tempo_chegada <= tempo_atual && p[i].tempo_restante > 0 && p[i].prioridade > maior_prioridade) {
                maior_prioridade = p[i].prioridade;
                idx = i;
            }
        }

        if (idx != -1) {
            if (p[idx].tempo_resposta == -1) {
                p[idx].tempo_resposta = tempo_atual - p[idx].tempo_chegada;
                tempo_resposta_total += p[idx].tempo_resposta;
            }

            p[idx].tempo_restante--;
            tempo_atual++;

            if (p[idx].tempo_restante == 0) {
                processos_concluidos++;
                p[idx].turnaround_time = tempo_atual - p[idx].tempo_chegada;
                p[idx].tempo_espera = p[idx].turnaround_time - p[idx].tempo_execucao;

                turnaround_time_total += p[idx].turnaround_time;
                tempo_espera_total += p[idx].tempo_espera;
            }
        } else {
            tempo_atual++;
        }
    }

    printf("\nResultados do Escalonamento com Prioridade Dinâmica:\n");
    printf("ID\tTempo de Execução\tTempo de Chegada\tPrioridade\tTempo de Espera\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t\t%d\t\t%d\t\t%d\t\t%d\n",
               p[i].id, p[i].tempo_execucao, p[i].tempo_chegada, p[i].prioridade,
               p[i].tempo_espera, p[i].turnaround_time);
    }

    printf("\nMédias:\n");
    printf("Tempo de Espera Médio: %.2f\n", (float)tempo_espera_total / n);
    printf("Tempo de Resposta Médio: %.2f\n", (float)tempo_resposta_total / n);
    printf("Turnaround Time Médio: %.2f\n", (float)turnaround_time_total / n);
}

// Função para escalonamento Round Robin
void roundRobin(struct Processo p[], int n) {
    int tempo_atual = 0, processos_concluidos = 0;
    int tempo_espera_total = 0, turnaround_time_total = 0, tempo_resposta_total = 0;
    int quantum = QUANTUM;

    while (processos_concluidos < n) {
        bool processo_executado = false;
        for (int i = 0; i < n; i++) {
            if (p[i].tempo_restante > 0 && p[i].tempo_chegada <= tempo_atual) {
                if (p[i].tempo_resposta == -1) {
                    p[i].tempo_resposta = tempo_atual - p[i].tempo_chegada;
                    tempo_resposta_total += p[i].tempo_resposta;
                }

                int tempo_exec = (p[i].tempo_restante < quantum) ? p[i].tempo_restante : quantum;
                p[i].tempo_restante -= tempo_exec;
                tempo_atual += tempo_exec;
                processo_executado = true;

                if (p[i].tempo_restante == 0) {
                    processos_concluidos++;
                    p[i].turnaround_time = tempo_atual - p[i].tempo_chegada;
                    p[i].tempo_espera = p[i].turnaround_time - p[i].tempo_execucao;

                    turnaround_time_total += p[i].turnaround_time;
                    tempo_espera_total += p[i].tempo_espera;
                }
            }
        }
        if (!processo_executado) {
            tempo_atual++;
        }
    }

    printf("\nResultados do Escalonamento Round Robin:\n");
    printf("ID\tTempo de Execução\tTempo de Chegada\tTempo de Espera\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t\t%d\t\t%d\t\t%d\n",
               p[i].id, p[i].tempo_execucao, p[i].tempo_chegada,
               p[i].tempo_espera, p[i].turnaround_time);
    }

    printf("\nMédias:\n");
    printf("Tempo de Espera Médio: %.2f\n", (float)tempo_espera_total / n);
    printf("Tempo de Resposta Médio: %.2f\n", (float)tempo_resposta_total / n);
    printf("Turnaround Time Médio: %.2f\n", (float)turnaround_time_total / n);
}

// Função para escalonamento Shortest Job First (SJF)
void sjf(struct Processo p[], int n) {
    int tempo_atual = 0, processos_concluidos = 0;
    int tempo_espera_total = 0, turnaround_time_total = 0, tempo_resposta_total = 0;

    while (processos_concluidos < n) {
        int idx = -1;
        int menor_tempo_exec = 10000; // Define um valor inicial alto

        for (int i = 0; i < n; i++) {
            if (p[i].tempo_chegada <= tempo_atual && p[i].tempo_restante > 0 && p[i].tempo_execucao < menor_tempo_exec) {
                menor_tempo_exec = p[i].tempo_execucao;
                idx = i;
            }
        }

        if (idx != -1) {
            if (p[idx].tempo_resposta == -1) {
                p[idx].tempo_resposta = tempo_atual - p[idx].tempo_chegada;
                tempo_resposta_total += p[idx].tempo_resposta;
            }

            p[idx].tempo_restante--;
            tempo_atual++;

            if (p[idx].tempo_restante == 0) {
                processos_concluidos++;
                p[idx].turnaround_time = tempo_atual - p[idx].tempo_chegada;
                p[idx].tempo_espera = p[idx].turnaround_time - p[idx].tempo_execucao;

                turnaround_time_total += p[idx].turnaround_time;
                tempo_espera_total += p[idx].tempo_espera;
            }
        } else {
            tempo_atual++;
        }
    }

    printf("\nResultados do Escalonamento SJF:\n");
    printf("ID\tTempo de Execução\tTempo de Chegada\tTempo de Espera\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t\t%d\t\t%d\t\t%d\n",
               p[i].id, p[i].tempo_execucao, p[i].tempo_chegada,
               p[i].tempo_espera, p[i].turnaround_time);
    }

    printf("\nMédias:\n");
    printf("Tempo de Espera Médio: %.2f\n", (float)tempo_espera_total / n);
    printf("Tempo de Resposta Médio: %.2f\n", (float)tempo_resposta_total / n);
    printf("Turnaround Time Médio: %.2f\n", (float)turnaround_time_total / n);
}

// Função para escalonamento FCFS (First-Come, First-Served)
void fcfs(struct Processo p[], int n) {
    int tempo_atual = 0, processos_concluidos = 0;
    int tempo_espera_total = 0, turnaround_time_total = 0, tempo_resposta_total = 0;

    for (int i = 0; i < n; i++) {
        if (tempo_atual < p[i].tempo_chegada) {
            tempo_atual = p[i].tempo_chegada;
        }

        if (p[i].tempo_resposta == -1) {
            p[i].tempo_resposta = tempo_atual - p[i].tempo_chegada;
            tempo_resposta_total += p[i].tempo_resposta;
        }

        p[i].tempo_restante = 0;
        tempo_atual += p[i].tempo_execucao;

        p[i].turnaround_time = tempo_atual - p[i].tempo_chegada;
        p[i].tempo_espera = p[i].turnaround_time - p[i].tempo_execucao;

        turnaround_time_total += p[i].turnaround_time;
        tempo_espera_total += p[i].tempo_espera;
        processos_concluidos++;
    }

    printf("\nResultados do Escalonamento FCFS:\n");
    printf("ID\tTempo de Execução\tTempo de Chegada\tTempo de Espera\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t\t%d\t\t%d\t\t%d\n",
               p[i].id, p[i].tempo_execucao, p[i].tempo_chegada,
               p[i].tempo_espera, p[i].turnaround_time);
    }

    printf("\nMédias:\n");
    printf("Tempo de Espera Médio: %.2f\n", (float)tempo_espera_total / n);
    printf("Tempo de Resposta Médio: %.2f\n", (float)tempo_resposta_total / n);
    printf("Turnaround Time Médio: %.2f\n", (float)turnaround_time_total / n);
}

// Função principal para executar todos os algoritmos e comparar resultados
int main() {
    int n = 6;
    struct Processo processos[6] = {
        {1, 10, 0, 3, 10, 0, -1, 0}, 
        {2, 5, 1, 1, 5, 0, -1, 0}, 
        {3, 8, 2, 2, 8, 0, -1, 0}, 
        {4, 6, 3, 4, 6, 0, -1, 0}, 
        {5, 4, 4, 5, 4, 0, -1, 0}, 
        {6, 3, 5, 2, 3, 0, -1, 0}
    };

    printf("Comparação de Algoritmos de Escalonamento:\n");

    struct Processo processosRR[6], processosSJF[6], processosFCFS[6], processosPrioridade[6];
    memcpy(processosRR, processos, sizeof(processos));
    memcpy(processosSJF, processos, sizeof(processos));
    memcpy(processosFCFS, processos, sizeof(processos));
    memcpy(processosPrioridade, processos, sizeof(processos));

    roundRobin(processosRR, n);
    sjf(processosSJF, n);
    fcfs(processosFCFS, n);
    escalonamentoDinamico(processosPrioridade, n);

    return 0;
}
