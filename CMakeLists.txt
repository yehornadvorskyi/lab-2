cmake_minimum_required(VERSION 3.20)
project(lab-2 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    message(WARNING "Experiments must be run in a Release build")
endif()

if (MSVC)
    add_compile_options(/W4 /EHsc)
endif()

add_compile_definitions(DATA_DIR="${CMAKE_SOURCE_DIR}/data")

add_executable(generate_data
    generate.cpp
    datasets.cpp datasets.h
    sequence_io.cpp sequence_io.h
 "predicates.h" "timing.h" "table.h" "table.cpp" "experiments.h" "experiments.cpp")
add_executable(lab2
    main.cpp
    experiments.cpp experiments.h
    datasets.cpp datasets.h
    sequence_io.cpp sequence_io.h
    table.cpp table.h
    timing.h predicates.h
)