cmake_minimum_required(VERSION 3.20)
project(bancada_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Copia a ngspice.dll para a pasta de saída (só necessário no Windows)
function(copy_ngspice_dll target)
  if(WIN32)
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${CMAKE_SOURCE_DIR}/third_party/ngspice/ngspice.dll
              $<TARGET_FILE_DIR:${target}>)
  endif()
endfunction()

add_library(bancada_core STATIC
  core/engine/ngspice_engine.cpp
  core/engine/dynlib.cpp)
target_include_directories(bancada_core PUBLIC
  ${CMAKE_SOURCE_DIR}/core
  ${CMAKE_SOURCE_DIR}/core/engine)
target_link_libraries(bancada_core PUBLIC ${CMAKE_DL_LIBS})

add_executable(simcli tools/simcli/main.cpp)
target_link_libraries(simcli PRIVATE bancada_core)
copy_ngspice_dll(simcli)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

enable_testing()
add_subdirectory(tests)