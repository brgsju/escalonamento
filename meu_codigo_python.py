import matplotlib.pyplot as plt

class Processo:
    def __init__(self, id, tempo_chegada, tempo_execucao, prioridade=0):
        self.id = id
        self.tempo_chegada = tempo_chegada
        self.tempo_execucao = tempo_execucao
        self.tempo_restante = tempo_execucao
        self.tempo_inicio = None
        self.tempo_termino = None
        self.tempo_espera = 0
        self.tempo_resposta = None
        self.turnaround = None
        self.prioridade = prioridade

# Função ajustada para iniciar cada processo corretamente
def iniciar_processo(processo, tempo_atual):
    if processo.tempo_inicio is None:
        processo.tempo_inicio = tempo_atual
        processo.tempo_resposta = tempo_atual - processo.tempo_chegada

# Adaptação nos escalonadores
def round_robin_multicore(processos, quantum, num_nucleos):
    tempo_atual = 0
    filas_nucleos = [[] for _ in range(num_nucleos)]
    processos.sort(key=lambda x: x.tempo_chegada)
    
    while any(filas_nucleos) or processos:
        for i, fila in enumerate(filas_nucleos):
            if fila:
                processo = fila.pop(0)
                iniciar_processo(processo, tempo_atual)
                if processo.tempo_restante > quantum:
                    processo.tempo_restante -= quantum
                    tempo_atual += quantum
                    fila.append(processo)
                else:
                    tempo_atual += processo.tempo_restante
                    processo.tempo_restante = 0
                    processo.tempo_termino = tempo_atual
                    processo.turnaround = processo.tempo_termino - processo.tempo_chegada
                    processo.tempo_espera = processo.turnaround - processo.tempo_execucao

# Função para calcular métricas após a execução dos algoritmos
def calcular_metricas(processos):
    turnaround_times = [p.turnaround for p in processos if p.turnaround is not None]
    waiting_times = [p.tempo_espera for p in processos if p.tempo_espera is not None]
    response_times = [p.tempo_resposta for p in processos if p.tempo_resposta is not None]

    return {
        "turnaround_medio": sum(turnaround_times) / len(turnaround_times) if turnaround_times else 0,
        "waiting_medio": sum(waiting_times) / len(waiting_times) if waiting_times else 0,
        "response_medio": sum(response_times) / len(response_times) if response_times else 0,
    }

# Exemplo de uso:
processos = [
    Processo(1, 0, 10),
    Processo(2, 2, 5),
    Processo(3, 4, 7)
]

round_robin_multicore(processos, quantum=4, num_nucleos=2)

# Calculando métricas
metricas = calcular_metricas(processos)
print(metricas)

# Dados para o plot
algoritmos = ["Round Robin"]
turnaround = [metricas["turnaround_medio"]]
waiting = [metricas["waiting_medio"]]
response = [metricas["response_medio"]]

# Gráficos
plt.figure(figsize=(10, 5))

# Gráfico de Turnaround Médio
plt.subplot(1, 3, 1)
plt.bar(algoritmos, turnaround, color='blue')
plt.title("Turnaround Médio")
plt.ylabel("Tempo")

# Gráfico de Tempo de Espera Médio
plt.subplot(1, 3, 2)
plt.bar(algoritmos, waiting, color='green')
plt.title("Tempo de Espera Médio")

# Gráfico de Tempo de Resposta Médio
plt.subplot(1, 3, 3)
plt.bar(algoritmos, response, color='red')
plt.title("Tempo de Resposta Médio")

plt.tight_layout()
plt.show()

