#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "engine/ngspice_engine.h"
#include "model/circuit.h"
#include "model/component.h"

using Clock = std::chrono::steady_clock;

static double milliseconds(Clock::time_point from, Clock::time_point to) {
    return std::chrono::duration<double, std::milli>(to - from).count();
}

int main(int argc, char** argv) {
    // Uso: simcli [tempo_final_em_segundos]. Padrão: 5 ms
    const double stop = (argc > 1) ? std::atof(argv[1]) : 5e-3;
    const double tmax = (argc > 2) ? std::atof(argv[2]) : 0.0;

    Circuit circuit{"RC passa-baixas"};
    circuit.add(VoltageSource{"V1", "n1", "0", 5.0});
    circuit.add(Resistor{"R1", "n1", "n2", 1000.0});
    circuit.add(Capacitor{"C1", "n2", "0", 1e-6});
    circuit.setTransient(1e-6, stop, tmax);

    NgSpiceEngine engine;
    std::string error;

    const auto t0 = Clock::now();
    if (!engine.load(kNgspiceLibrary, error)) {
        std::printf("Erro: %s\n", error.c_str());
        return 1;
    }
    const auto t1 = Clock::now();

    if (!engine.runNetlist(circuit.toNetlist(), error)) {
        std::printf("Erro: %s\n", error.c_str());
        return 1;
    }
    const auto t2 = Clock::now();

    const auto v = engine.vector("v(n2)");
    const auto t3 = Clock::now();

    std::printf("carga: %.2f ms | simulacao: %.2f ms | leitura: %.3f ms | pontos: %zu\n",
                milliseconds(t0, t1), milliseconds(t1, t2), milliseconds(t2, t3), v.size());
    return 0;
}