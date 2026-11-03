# Desempenho

Este documento registra as medições de desempenho do simulador, com as condições de cada uma e a conclusão tirada delas. Cada medição usa a mediana de várias execuções, porque o sistema operacional introduz ruído grande em tempos pequenos.

## Condições

- **Sistema:** Windows 11, notebook (modelo da CPU e da GPU não registrado).
- **Build:** Release, MSVC (Visual Studio 2026).
- **Motor:** ngspice 42, em modo shared library (DLL do KiCad 8.0).
- **Circuito:** RC passa-baixas, com V1 = 5 V, R1 = 1 kΩ e C1 = 1 µF.
- **Medição:** `simcli.exe` com `std::chrono::steady_clock`, separando carga da DLL, simulação e leitura dos dados.

## Resultados

### Cenário de 5 ms (passo de 1 µs, 5 011 pontos)

| Build | Simulação (mediana) | Faixa observada | Execuções |
|---|---|---|---|
| Debug | 14,7 ms | 10,8 a 31,6 ms | 10 |
| Release | 13,6 ms | 11,0 a 15,6 ms | 10 |

### Cenário de 1 s (passo de 1 µs, 1 000 011 pontos)

| Build | Simulação (mediana) | Leitura (mediana) | Execuções |
|---|---|---|---|
| Debug | 1 923 ms | 4,7 ms | 10 |
| Release | 1 896 ms | 2,3 ms | 20 |

### Cenário de 1 s com `tmax` = 10 µs (100 015 pontos)

| Build | Simulação (mediana) | Leitura (mediana) | Execuções |
|---|---|---|---|
| Release | 227 ms | 0,40 ms | 10 |

A carga da DLL ficou entre 11 e 17 ms em todos os cenários, sem diferença relevante entre eles.

## Conclusões

1. **O build Release quase não muda a simulação.** A diferença fica em torno de 1%, dentro do ruído. O trabalho pesado está dentro da DLL do ngspice, que já vem otimizada. O ganho do Release aparece só no código do projeto, na leitura dos dados.

2. **O gargalo é o número de passos internos do solver.** Com passo interno máximo de 1 µs, a simulação de 1 s gera 1 milhão de passos. Com `tmax` = 10 µs, o solver dá cerca de 100 mil passos.

3. **Com `tmax` = 10 µs, a simulação ficou cerca de 8,4 vezes mais rápida** (1 896 ms para 227 ms). Como o número de pontos caiu 10 vezes, o ganho ficou um pouco abaixo disso, por causa do custo fixo de cada execução.

4. **A precisão se manteve no teste do RC.** O teste `tmax de 10 us mantém a precisão do RC` compara todos os pontos com a fórmula analítica, `v(t) = 5 · (1 − e^(−t/RC))`, e passa com tolerância de 0,05 V.

5. **A leitura dos dados também ficou mais rápida**, porque há menos pontos para copiar: de 2,3 ms para 0,40 ms.

## Limitações

- A precisão foi verificada só para um circuito suave, com constante de tempo de 1 ms. Um circuito com transientes rápidos pode exigir passos menores. Por isso o `tmax` é um parâmetro explícito, e não um valor padrão.
- O teste de 1 kHz (senoide) ainda não foi repetido com `tmax`. Precisa ser verificado antes de usar o parâmetro com o osciloscópio.
- O custo fixo por execução (carga e configuração do solver) não foi otimizado.

## Como reproduzir

```powershell
cmake --build build --config Release
.\build\Release\simcli.exe 1            # 1 s, passo automático
.\build\Release\simcli.exe 1 0.00001    # 1 s, tmax de 10 µs
```

Execute cada comando pelo menos dez vezes e use a mediana. Descarte a primeira execução, que costuma ser mais lenta por causa do cache do sistema.