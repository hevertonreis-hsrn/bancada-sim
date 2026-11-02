#pragma once
#include <string>

// Camada fina sobre a API de bibliotecas dinâmicas do sistema.
// Só este módulo conhece LoadLibrary (Windows) e dlopen (Linux).
namespace dynlib {

using Handle = void*;

Handle open(const std::string& path, std::string& error);
void* symbol(Handle lib, const char* name);
void close(Handle lib);

}  // namespace dynlib