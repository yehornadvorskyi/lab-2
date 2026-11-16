cmake_minimum_required(VERSION 3.20)
project(lab-2 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    message(WARNING "Experiments must be run in a Release build")
endif()

string(REGEX REPLACE "(/|-)O[0-9b]+" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

if (MSVC)
    add_compile_options(/W4 /EHsc)
    set(OPT_MAX /O2)
    set(OPT_NONE /Od)
else()
    add_compile_options(-Wall -Wextra)
    set(OPT_MAX -O3)
    set(OPT_NONE -O0)
endif()

add_compile_definitions(DATA_DIR="${CMAKE_SOURCE_DIR}/data")

add_executable(generate_data
    generate.cpp
    datasets.cpp datasets.h
    sequence_io.cpp sequence_io.h
)
target_compile_options(generate_data PRIVATE ${OPT_MAX})

function(add_experiments name opt label)
    add_executable(${name}
        main.cpp
        experiments.cpp experiments.h
        datasets.cpp datasets.h
        sequence_io.cpp sequence_io.h
        table.cpp table.h
        timing.h predicates.h parallel_none_of.h
    )
    target_compile_options(${name} PRIVATE ${opt})
    target_compile_definitions(${name} PRIVATE OPT_LABEL="${label}")
endfunction()

add_experiments(lab2_max  "${OPT_MAX}"  "maximum (${OPT_MAX})")
add_experiments(lab2_none "${OPT_NONE}" "none (${OPT_NONE})")