#include "ngspice_engine.h"
#include <windows.h>
#include <sstream>
#include <cstdio>

static int OnText(char* text, int, void*) {
    std::printf("[ngspice] %s\n", text);
    return 0;
}

template <typename F>
static F resolve(HMODULE lib, const char* name) {
    return reinterpret_cast<F>(GetProcAddress(lib, name));
}

NgSpiceEngine::~NgSpiceEngine() {
    if (lib_) FreeLibrary(static_cast<HMODULE>(lib_));
}

bool NgSpiceEngine::load(const std::string& dllName, std::string& error) {
    HMODULE lib = LoadLibraryA(dllName.c_str());
    if (!lib) {
        error = "falha ao carregar " + dllName + " (erro " + std::to_string(GetLastError()) + ")";
        return false;
    }
    lib_ = lib;
    init_      = resolve<NgSpice_Init_Fn>(lib, "ngSpice_Init");
    circ_      = resolve<NgSpice_Circ_Fn>(lib, "ngSpice_Circ");
    command_   = resolve<NgSpice_Command_Fn>(lib, "ngSpice_Command");
    getVecInfo_ = resolve<NgGet_Vec_Info_Fn>(lib, "ngGet_Vec_Info");
    if (!init_ || !circ_ || !command_ || !getVecInfo_) {
        error = "funções do ngspice ausentes na DLL";
        return false;
    }
    init_(OnText, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    return true;
}

bool NgSpiceEngine::runNetlist(const std::string& netlist, std::string& error) {
    // ngSpice_Circ recebe char** não-const: cada linha precisa de memória própria
    std::vector<std::vector<char>> storage;
    std::istringstream in(netlist);
    std::string line;
    while (std::getline(in, line)) {
        storage.emplace_back(line.begin(), line.end());
        storage.back().push_back('\0');
    }
    std::vector<char*> lines;
    for (auto& s : storage) lines.push_back(s.data());
    lines.push_back(nullptr);

    if (circ_(lines.data()) != 0) {
        error = "ngSpice_Circ rejeitou a netlist";
        return false;
    }
    char run[] = "run";
    command_(run);  // "run" é síncrono: retorna quando a simulação termina
    return true;
}

std::vector<double> NgSpiceEngine::vector(const std::string& name) const {
    std::vector<char> buf(name.begin(), name.end());
    buf.push_back('\0');
    NgVectorInfo* info = getVecInfo_(buf.data());
    if (!info || !info->v_realdata) return {};
    return std::vector<double>(info->v_realdata, info->v_realdata + info->v_length);
}