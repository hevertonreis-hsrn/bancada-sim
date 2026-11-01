#pragma once

// Callback que o ngspice chama para enviar texto (mensagens, saída de "version", etc.)
using NgSpice_SendChar_Fn = int (*)(char* text, int id, void* user);

// Assinatura de ngSpice_Init: seis callbacks e um ponteiro de usuário
using NgSpice_Init_Fn = int (*)(NgSpice_SendChar_Fn, void*, void*, void*, void*, void*, void*);

// Assinatura de ngSpice_Command: recebe um comando como texto
using NgSpice_Command_Fn = int (*)(char* command);