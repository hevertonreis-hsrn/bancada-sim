add_executable(core_tests smoke_test.cpp)
target_link_libraries(core_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(core_tests PRIVATE ${CMAKE_SOURCE_DIR}/core/engine)

include(Catch)
catch_discover_tests(core_tests)