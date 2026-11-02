#pragma once
#include <sstream>
#include <string>
#include <variant>
#include <vector>

struct Resistor {
    std::string name;
    std::string node1;
    std::string node2;
    double ohms;

    std::string toSpiceLine() const {
        std::ostringstream line;
        line << name << ' ' << node1 << ' ' << node2 << ' ' << ohms;
        return line.str();
    }
};

struct Capacitor {
    std::string name;
    std::string node1;
    std::string node2;
    double farads;

    std::string toSpiceLine() const {
        std::ostringstream line;
        line << name << ' ' << node1 << ' ' << node2 << ' ' << farads;
        return line.str();
    }
};

struct VoltageSource {
    std::string name;
    std::string node1;
    std::string node2;
    double volts;

    std::string toSpiceLine() const {
        std::ostringstream line;
        line << name << ' ' << node1 << ' ' << node2 << " DC " << volts;
        return line.str();
    }
};

// Qualquer componente do circuito. Para adicionar um tipo, inclua-o aqui.
using AnyComponent = std::variant<Resistor, Capacitor, VoltageSource>;

// std::visit chama a versão certa para o tipo que está dentro do variant
inline std::string componentName(const AnyComponent& c) {
    return std::visit([](const auto& comp) { return comp.name; }, c);
}

inline std::string componentToSpice(const AnyComponent& c) {
    return std::visit([](const auto& comp) { return comp.toSpiceLine(); }, c);
}

inline std::vector<std::string> componentNodes(const AnyComponent& c) {
    return std::visit([](const auto& comp) -> std::vector<std::string> {
        return {comp.node1, comp.node2};
    }, c);
}