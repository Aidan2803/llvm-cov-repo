cmake_minimum_required(VERSION 3.16)
project(MyProject CXX)

# Enable C++17 or adjust to your C++ version
set(CMAKE_CXX_STANDARD 17)

# Enable coverage flags in Debug mode or when a COVERAGE option is enabled
option(COVERAGE "Enable coverage reporting" ON)
if(COVERAGE)
    message("Building with coverage instrumentation...")
    add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate)
endif()

# Find and link Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Define main program executable
add_executable(my_program main.cpp my_functions.cpp)

# Define test executable
add_executable(tests test_main.cpp my_functions.cpp)
target_link_libraries(tests GTest::gtest GTest::gtest_main pthread)

# Specify output filenames for the raw and merged profile data
set(PROFRAW_MAIN "main_program.profraw")
set(PROFRAW_TESTS "tests.profraw")
set(PROFDATA_FILE "coverage.profdata")

# Define custom target to run the main program and generate main_program.profraw
add_custom_target(run_main_program
    COMMAND LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/${PROFRAW_MAIN} ./my_program
    COMMENT "Running main program to generate ${PROFRAW_MAIN}"
)

# Define custom target to run tests and generate tests.profraw
add_custom_target(run_tests
    COMMAND LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/${PROFRAW_TESTS} ./tests
    COMMENT "Running tests to generate ${PROFRAW_TESTS}"
)

# Define custom target to merge profraw files into coverage.profdata
add_custom_target(merge_coverage_data
    COMMAND llvm-profdata merge -sparse -output=${CMAKE_BINARY_DIR}/${PROFDATA_FILE} ${CMAKE_BINARY_DIR}/${PROFRAW_MAIN} ${CMAKE_BINARY_DIR}/${PROFRAW_TESTS}
    DEPENDS run_main_program run_tests
    COMMENT "Merging coverage data into ${PROFDATA_FILE}"
)

# Define custom target to generate HTML coverage report
add_custom_target(generate_coverage_report
    COMMAND llvm-cov show ./my_program -instr-profile=${CMAKE_BINARY_DIR}/${PROFDATA_FILE} -format=html -output-dir=${CMAKE_BINARY_DIR}/coverage_report
    DEPENDS merge_coverage_data
    COMMENT "Generating HTML coverage report in ${CMAKE_BINARY_DIR}/coverage_report"
)

# Add dependencies to make sure targets are built before running them
add_dependencies(run_main_program my_program)
add_dependencies(run_tests tests)
add_dependencies(generate_coverage_report merge_coverage_data)
