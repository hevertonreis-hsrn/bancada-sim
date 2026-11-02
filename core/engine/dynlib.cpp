#include "dynlib.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace dynlib {

Handle open(const std::string& path, std::string& error) {
#if defined(_WIN32)
    HMODULE lib = LoadLibraryA(path.c_str());
    if (!lib) {
        error = "LoadLibrary falhou (erro " + std::to_string(GetLastError()) + ")";
    }
    return lib;
#else
    void* lib = dlopen(path.c_str(), RTLD_NOW);
    if (!lib) {
        const char* msg = dlerror();
        error = msg ? msg : "erro desconhecido";
    }
    return lib;
#endif
}

void* symbol(Handle lib, const char* name) {
#if defined(_WIN32)
    return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(lib), name));
#else
    return dlsym(lib, name);
#endif
}

void close(Handle lib) {
    if (!lib) return;
#if defined(_WIN32)
    FreeLibrary(static_cast<HMODULE>(lib));
#else
    dlclose(lib);
#endif
}

}  // namespace dynlib