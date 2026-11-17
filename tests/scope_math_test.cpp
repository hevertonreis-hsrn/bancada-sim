#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <stdexcept>
#include "scope/scope_math.h"
#include <optional>
#include <vector>

// Comparação com tolerância, para números em ponto flutuante
static bool near(double a, double b) {
    return std::fabs(a - b) < 1e-9;
}

TEST_CASE("tensao de 1 V a 500 mV/div ocupa 2 divisoes") {
    REQUIRE(near(scope::voltsToDivisions(1.0, 0.5, 0.0), 2.0));
}

TEST_CASE("deslocamento positivo sobe o traco") {
    REQUIRE(near(scope::voltsToDivisions(0.0, 1.0, 0.5), 0.5));
}

TEST_CASE("pico de 1,20 V a 500 mV/div ocupa 2,4 divisoes") {
    // Valor medido no protótipo: Vmáx = 1,20 V com escala de 500 mV/div
    REQUIRE(near(scope::voltsToDivisions(1.20, 0.5, 0.0), 2.4));
}

TEST_CASE("intervalo de 750 micro_s a 200 micro_s/div ocupa 3,75 divisoes") {
    // Valor medido no protótipo: Δt = 750 µs com base de 200 µs/div
    REQUIRE(near(scope::secondsToDivisions(750e-6, 200e-6), 3.75));
}

TEST_CASE("periodo de 1 kHz a 200 micro_s/div ocupa 5 divisoes") {
    REQUIRE(near(scope::secondsToDivisions(1.0 / 1000.0, 200e-6), 5.0));
}

TEST_CASE("divisoes viram coordenada normalizada (-1 a 1) numa tela de 8 divisoes") {
    REQUIRE(near(scope::divisionsToNdc(4.0, 8.0), 1.0));    // topo da tela
    REQUIRE(near(scope::divisionsToNdc(0.0, 8.0), 0.0));    // centro
    REQUIRE(near(scope::divisionsToNdc(-2.0, 8.0), -0.5));  // um quarto abaixo
}

TEST_CASE("escala nao positiva eh rejeitada") {
    REQUIRE_THROWS_AS(scope::voltsToDivisions(1.0, 0.0, 0.0), std::invalid_argument);
    REQUIRE_THROWS_AS(scope::secondsToDivisions(1.0, -1.0), std::invalid_argument);
    REQUIRE_THROWS_AS(scope::divisionsToNdc(1.0, 0.0), std::invalid_argument);
}

TEST_CASE("borda de subida eh encontrada no cruzamento do nivel") {
    const std::vector<double> s{0.0, 0.2, 0.6, 1.0};
    const auto idx = scope::findRisingEdge(s, 0.5);
    REQUIRE(idx.has_value());
    REQUIRE(*idx == 2);
}

TEST_CASE("borda de descida nao dispara o gatilho") {
    const std::vector<double> s{1.0, 0.6, 0.2, 0.0};
    REQUIRE_FALSE(scope::findRisingEdge(s, 0.5).has_value());
}

TEST_CASE("busca a partir de um indice ignora cruzamentos anteriores") {
    const std::vector<double> s{0.0, 1.0, 0.0, 1.0};
    const auto idx = scope::findRisingEdge(s, 0.5, 2);
    REQUIRE(idx.has_value());
    REQUIRE(*idx == 3);
}

TEST_CASE("sinal sem cruzamento nao tem gatilho") {
    const std::vector<double> s{0.0, 0.1, 0.2};
    REQUIRE_FALSE(scope::findRisingEdge(s, 0.5).has_value());
}
