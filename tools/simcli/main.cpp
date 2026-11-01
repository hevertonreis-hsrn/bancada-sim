#include <windows.h>
#include <cstdio>
#include "ngspice_api.h"

// Recebe o texto que o ngspice quer mostrar e imprime na tela
static int OnText(char* text, int /*id*/, void* /*user*/) {
    std::printf("%s\n", text);
    return 0;
}

int main() {
    // 1. Abre a DLL em tempo de execução
    HMODULE lib = LoadLibraryA("ngspice.dll");
    if (!lib) {
        std::printf("Falha ao carregar ngspice.dll (erro %lu)\n", GetLastError());
        return 1;
    }

    // 2. Obtém as funções pelo nome
    auto init    = reinterpret_cast<NgSpice_Init_Fn>(GetProcAddress(lib, "ngSpice_Init"));
    auto command = reinterpret_cast<NgSpice_Command_Fn>(GetProcAddress(lib, "ngSpice_Command"));
    if (!init || !command) {
        std::printf("Funções do ngspice não encontradas\n");
        FreeLibrary(lib);
        return 1;
    }

    // 3. Inicializa o motor, registrando só o callback de texto
    init(OnText, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

    // 4. Pede a versão do motor
    command("version");

    FreeLibrary(lib);
    return 0;
}