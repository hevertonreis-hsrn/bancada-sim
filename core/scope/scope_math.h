#pragma once

namespace scope {

// Tensão em divisões verticais. O deslocamento soma antes da escala,
// como no painel de canal do protótipo.
double voltsToDivisions(double volts, double voltsPerDiv, double offsetVolts);

// Tempo em divisões horizontais.
double secondsToDivisions(double seconds, double secondsPerDiv);

// Divisões para coordenada normalizada (-1 a 1), que é o formato que o OpenGL usa.
// A tela tem totalDivisions divisões, centradas em zero.
double divisionsToNdc(double divisions, double totalDivisions);

}  // namespace scope