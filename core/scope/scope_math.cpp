#include "scope/scope_math.h"
#include <stdexcept>

namespace scope {

double voltsToDivisions(double volts, double voltsPerDiv, double offsetVolts) {
    if (voltsPerDiv <= 0.0) {
        throw std::invalid_argument("escala vertical deve ser positiva");
    }
    return (volts + offsetVolts) / voltsPerDiv;
}

double secondsToDivisions(double seconds, double secondsPerDiv) {
    if (secondsPerDiv <= 0.0) {
        throw std::invalid_argument("base de tempo deve ser positiva");
    }
    return seconds / secondsPerDiv;
}

double divisionsToNdc(double divisions, double totalDivisions) {
    if (totalDivisions <= 0.0) {
        throw std::invalid_argument("numero de divisoes deve ser positivo");
    }
    // Metade da tela, em divisões, corresponde a 1 em coordenada normalizada
    return divisions / (totalDivisions / 2.0);
}

}  // namespace scope