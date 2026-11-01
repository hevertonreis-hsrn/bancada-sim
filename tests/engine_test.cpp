#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include "model/circuit.h"
#include "model/component.h"
#include "engine/ngspice_engine.h"

// Teste de integração: usa o ngspice de verdade.
// Só há um engine neste processo, porque o ngspice mantém estado global.
TEST_CASE("RC simulado segue a formula analitica") {
    Circuit circuit{"RC passa-baixas"};
    circuit.add(VoltageSource{"V1", "n1", "0", 5.0});
    circuit.add(Resistor{"R1", "n1", "n2", 1000.0});
    circuit.add(Capacitor{"C1", "n2", "0", 1e-6});
    circuit.setTransient(1e-5, 5e-3);

    NgSpiceEngine engine;
    std::string error;
    const bool loaded = engine.load("ngspice.dll", error);
    INFO(error);
    REQUIRE(loaded);
    REQUIRE(engine.runNetlist(circuit.toNetlist(), error));

    const auto t = engine.vector("time");
    const auto v = engine.vector("v(n2)");
    REQUIRE(t.size() > 100);
    REQUIRE(v.size() == t.size());

    // Erro máximo em toda a curva: v(t) = 5 * (1 - e^(-t/RC)), com RC = 1 ms
    const double tau = 1000.0 * 1e-6;
    double maxError = 0.0;
    for (std::size_t i = 0; i < t.size(); ++i) {
        const double expected = 5.0 * (1.0 - std::exp(-t[i] / tau));
        maxError = std::max(maxError, std::fabs(v[i] - expected));
    }
    REQUIRE(maxError < 0.05);  // menos de 1% da fonte (5 V)

    // Critério de aceite da Fase 1: em t = 1 ms, V ≈ 3,16 V
    std::size_t idx = 0;
    for (std::size_t i = 1; i < t.size(); ++i) {
        if (std::fabs(t[i] - 1e-3) < std::fabs(t[idx] - 1e-3)) idx = i;
    }
    REQUIRE(std::fabs(v[idx] - 3.16) < 0.05);
}