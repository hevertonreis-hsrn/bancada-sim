#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include "model/circuit.h"

TEST_CASE("nome duplicado eh rejeitado") {
    Circuit c{"teste"};
    c.add(Resistor{"R1", "n1", "0", 1000.0});
    REQUIRE_THROWS_AS(c.add(Resistor{"R1", "n2", "0", 2200.0}), std::invalid_argument);
}

TEST_CASE("circuito sem terra eh reprovado na validacao") {
    Circuit c{"teste"};
    c.add(Resistor{"R1", "n1", "n2", 1000.0});
    const auto errors = c.validate();
    REQUIRE(errors.size() == 1);
    REQUIRE(errors[0] == "circuito sem no de terra (0)");
}

TEST_CASE("circuito RC valido nao tem erros") {
    Circuit c{"RC"};
    c.add(VoltageSource{"V1", "n1", "0", 5.0});
    c.add(Resistor{"R1", "n1", "n2", 1000.0});
    c.add(Capacitor{"C1", "n2", "0", 1e-6});
    REQUIRE(c.validate().empty());
}

TEST_CASE("componente com os dois terminais no mesmo no eh reprovado") {
    Circuit c{"teste"};
    c.add(VoltageSource{"V1", "n1", "0", 5.0});
    c.add(Resistor{"R1", "n1", "n1", 1000.0});
    const auto errors = c.validate();
    REQUIRE(errors.size() == 1);
    REQUIRE(errors[0] == "R1: os dois terminais estao no mesmo no");
}

TEST_CASE("remocao por nome") {
    Circuit c{"teste"};
    c.add(Resistor{"R1", "n1", "0", 1000.0});
    c.add(Capacitor{"C1", "n1", "0", 1e-6});
    REQUIRE(c.remove("R1"));
    REQUIRE(c.components().size() == 1);
    REQUIRE_FALSE(c.remove("R9"));
}