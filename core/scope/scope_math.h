#pragma once
#include <cstddef>
#include <optional>
#include <vector>

namespace scope {

// Tensão em divisões verticais. O deslocamento soma antes da escala,
// como no painel de canal do protótipo.
double voltsToDivisions(double volts, double voltsPerDiv, double offsetVolts);

// Tempo em divisões horizontais.
double secondsToDivisions(double seconds, double secondsPerDiv);

// Divisões para coordenada normalizada (-1 a 1), que é o formato que o OpenGL usa.
// A tela tem totalDivisions divisões, centradas em zero.
double divisionsToNdc(double divisions, double totalDivisions);

// Índice da primeira amostra em que o sinal cruza o nível de baixo para cima,
// procurando a partir do índice 'from'. Devolve vazio se não houver cruzamento.
std::optional<std::size_t> findRisingEdge(const std::vector<double>& samples,
                                          double level,
                                          std::size_t from = 0);


}  // namespace scope