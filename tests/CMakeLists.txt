add_executable(core_tests smoke_test.cpp netlist_test.cpp circuit_test.cpp engine_test.cpp)
target_link_libraries(core_tests PRIVATE bancada_core Catch2::Catch2WithMain)
copy_ngspice_dll(core_tests)

include(Catch)
catch_discover_tests(core_tests)