#pragma once
#include <sstream>
#include <string>

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