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

std::optional<std::size_t> findRisingEdge(const std::vector<double>& samples,
                                          double level,
                                          std::size_t from) {
    for (std::size_t i = from + 1; i < samples.size(); ++i) {
        // Cruzamento: estava abaixo do nível na amostra anterior e está em cima agora
        if (samples[i - 1] < level && samples[i] >= level) {
            return i;
        }
    }
    return std::nullopt;
}

}  // namespace scope