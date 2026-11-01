#include <catch2/catch_test_macros.hpp>
#include "model/component.h"
#include "model/circuit.h"

TEST_CASE("resistor gera linha SPICE") {
    Resistor r{"R1", "n1", "n2", 1000.0};
    REQUIRE(r.toSpiceLine() == "R1 n1 n2 1000");
}

TEST_CASE("capacitor gera linha SPICE") {
    Capacitor c{"C1", "n2", "0", 1e-6};
    REQUIRE(c.toSpiceLine() == "C1 n2 0 1e-06");
}

TEST_CASE("fonte DC gera linha SPICE") {
    VoltageSource v{"V1", "n1", "0", 5.0};
    REQUIRE(v.toSpiceLine() == "V1 n1 0 DC 5");
}

TEST_CASE("circuito RC gera netlist completa") {
    Circuit circuit{"RC passa-baixas"};
    circuit.add(VoltageSource{"V1", "n1", "0", 5.0});
    circuit.add(Resistor{"R1", "n1", "n2", 1000.0});
    circuit.add(Capacitor{"C1", "n2", "0", 1e-6});
    circuit.setTransient(1e-5, 5e-3);

    REQUIRE(circuit.toNetlist() ==
            "RC passa-baixas\n"
            "V1 n1 0 DC 5\n"
            "R1 n1 n2 1000\n"
            "C1 n2 0 1e-06\n"
            ".tran 1e-05 0.005\n"
            ".end\n");
}