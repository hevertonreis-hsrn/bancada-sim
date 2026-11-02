cmake_minimum_required(VERSION 3.20)
project(bancada_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(copy_ngspice_dll target)
  if(WIN32)
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${CMAKE_SOURCE_DIR}/third_party/ngspice/ngspice.dll
              $<TARGET_FILE_DIR:${target}>)
  endif()
endfunction()

include(FetchContent)

# GLFW: desligamos docs, testes e exemplos, que não precisamos
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND  OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4)

FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.90.9)

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1)

FetchContent_MakeAvailable(glfw imgui Catch2)

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

add_subdirectory(app)

enable_testing()
add_subdirectory(tests)