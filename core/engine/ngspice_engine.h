#pragma once
#include <string>
#include <vector>
#include "ngspice_api.h"

// Uma instância por processo: o ngspice mantém estado global.
class NgSpiceEngine {
public:
    NgSpiceEngine() = default;
    ~NgSpiceEngine();
    NgSpiceEngine(const NgSpiceEngine&) = delete;
    NgSpiceEngine& operator=(const NgSpiceEngine&) = delete;

    bool load(const std::string& dllName, std::string& error);
    bool runNetlist(const std::string& netlist, std::string& error);
    std::vector<double> vector(const std::string& name) const;

private:
    void* lib_ = nullptr;  // HMODULE, mantido opaco no header
    NgSpice_Init_Fn init_ = nullptr;
    NgSpice_Circ_Fn circ_ = nullptr;
    NgSpice_Command_Fn command_ = nullptr;
    NgGet_Vec_Info_Fn getVecInfo_ = nullptr;
};