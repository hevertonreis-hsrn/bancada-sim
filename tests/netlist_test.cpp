#include <catch2/catch_test_macros.hpp>
#include "model/component.h"

TEST_CASE("resistor gera linha SPICE") {
    Resistor r{"R1", "n1", "n2", 1000.0};
    REQUIRE(r.toSpiceLine() == "R1 n1 n2 1000");
}