#pragma once

// Callback de texto: o ngspice o chama para mensagens e saída de comandos
using NgSpice_SendChar_Fn = int (*)(char* text, int id, void* user);

using NgSpice_Init_Fn    = int (*)(NgSpice_SendChar_Fn, void*, void*, void*, void*, void*, void*);
using NgSpice_Command_Fn = int (*)(char* command);
using NgSpice_Circ_Fn    = int (*)(char** netlist);  // vetor de linhas terminado em NULL

// Espelho de vector_info, a estrutura que ngGet_Vec_Info devolve.
// Confira o layout com o sharedspice.h oficial quando tivermos o arquivo.
struct NgVectorInfo {
    char*   v_name;
    int     v_type;
    short   v_flags;
    double* v_realdata;
    double* v_compdata;
    int     v_length;
};

using NgGet_Vec_Info_Fn = NgVectorInfo* (*)(char* name);