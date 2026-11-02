#include <cstdio>
#include <string>
#include "engine/ngspice_engine.h"
#include "model/circuit.h"
#include "model/component.h"

int main() {
    Circuit circuit{"RC passa-baixas"};
    circuit.add(VoltageSource{"V1", "n1", "0", 5.0});
    circuit.add(Resistor{"R1", "n1", "n2", 1000.0});
    circuit.add(Capacitor{"C1", "n2", "0", 1e-6});
    circuit.setTransient(1e-5, 5e-3);

    NgSpiceEngine engine;
    std::string error;
    if (!engine.load(kNgspiceLibrary, error) || !engine.runNetlist(circuit.toNetlist(), error)) {
        std::printf("Erro: %s\n", error.c_str());
        return 1;
    }

    const auto v = engine.vector("v(n2)");
    std::printf("%zu pontos; v(n2) em 5 ms = %.4f V (esperado ~4.9663 V)\n", v.size(), v.back());
    return 0;
}