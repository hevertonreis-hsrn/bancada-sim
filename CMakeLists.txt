cmake_minimum_required(VERSION 3.20)
project(bancada_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simcli tools/simcli/main.cpp)
target_include_directories(simcli PRIVATE core/engine)

# No Windows, copia a ngspice.dll para a pasta do executável após cada build
if(WIN32)
  add_custom_command(TARGET simcli POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/third_party/ngspice/ngspice.dll
            $<TARGET_FILE_DIR:simcli>)
endif()